#ifndef ZMQBACKEND_HPP
#define ZMQBACKEND_HPP

#ifndef ZMQ_CPP11
#define ZMQ_CPP11
#endif //ZMQ_CPP11	


#include <MQCloud/BackEndFactory.hpp>
#include <MQCloud/ZMQBackEnd/Message.pb.h> // TODO: Hide in SRC!!!

#include <zmq.hpp>
#include <sstream>
#include <atomic>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <MQCloud/GenericSignal.hpp>


namespace ZMQBackEnd {
	class PublisherSocketWrapper {
		std::shared_ptr<zmq::socket_t> rawSocket;
		std::vector<std::string> publishingEndpoints;
		unsigned short port;
	public:

		int Bind( const std::string baseUrl= "tcp://*:" ) {
			port = 4773;
			auto connected = false;
			std::string id;
			while(!connected) {
				try {
					std::ostringstream url;
					url << baseUrl << ++port;
					id = url.str();
					rawSocket->bind(id);
				} catch (zmq::error_t & e) {
					continue;
				}
				connected = true;
			}
			publishingEndpoints.push_back(id);
			return port;
		}

		void UnBind() {
			for( auto & address : publishingEndpoints) {
				rawSocket->unbind(address);
			}
		}

		std::string GetSocketId() const {
			auto result = "out-" + std::to_string(port);
			return result;
		}

		void Publish(const std::shared_ptr<MQCloud::Message> message) {
			static MQCloud::BackEnd::Protocol::Message protocol;
			protocol.set_pattern(message->PatternName);
			protocol.set_topic(message->topic);
			protocol.set_state(static_cast<MQCloud::BackEnd::Protocol::MessageState>(message->State));
			protocol.set_patterndata(message->PatternData);
			protocol.set_errordata(message->ErrorData);
			protocol.set_data(message->data);

			std::string fullTopicName = message->PatternName+message->topic;

			rawSocket->send(fullTopicName.data(), fullTopicName.size(), ZMQ_SNDMORE);

			auto size = protocol.ByteSize();
			zmq::message_t packet(size);
			protocol.SerializeToArray(packet.data(), size);
			rawSocket->send(packet);
		}
	};

	class SubscriberSocketWrapper {
		std::shared_ptr<zmq::socket_t> rawSocket;
		std::unordered_map<std::string, std::vector<std::string> > subscriptions;

		void SubscribeToTopic(const std::string & topic) {
			rawSocket->setsockopt( ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
		}

	public:
		std::shared_ptr<zmq::socket_t> GetRawSocket() const {
			return rawSocket;
		}

		std::string GetSocketId() const {
			auto result = "in-" + std::to_string(reinterpret_cast<int>(rawSocket.get()));
			return result;
		}

		SubscriberSocketWrapper(std::shared_ptr<zmq::context_t> ctx) {
			rawSocket = std::make_shared<zmq::socket_t>(*ctx, zmq::socket_type::sub);
		}

		void Connect( const std::string & url, const std::string & pattern, const std::string & topic) {
			auto urlIterator = subscriptions.find(url);
			auto fullTopicName = pattern+topic;
			if(urlIterator == subscriptions.end()) {
				rawSocket->connect(url);
				SubscribeToTopic(fullTopicName);
			} else {
				auto & topics = urlIterator->second;
				auto topicIterator = std::find(topics.begin(), topics.end(), fullTopicName);
				if(topicIterator == topics.end()) {
					SubscribeToTopic(fullTopicName);
				}
			}
		}

		std::shared_ptr<MQCloud::Message> Read() {
			static zmq::message_t message;
			rawSocket->recv(&message);

			static MQCloud::BackEnd::Protocol::Message protocol;
			auto packetParsed = protocol.ParseFromArray(message.data(), message.size());
			assert(packetParsed);

			auto result = std::shared_ptr<MQCloud::Message>(
				new MQCloud::Message( // TODO: fix make_shared!
				protocol.id(), 
				protocol.data(), 
				protocol.serviceid(),
				protocol.pattern(), 
				protocol.patterndata(),
				static_cast<const MQCloud::MessageState>(protocol.state()), 
				protocol.errordata()));
			return result;
		}

		void Disconnect(const std::string & url) {
			rawSocket->disconnect(url);
		}	
	};

	class IncomingEventDispatcher {
		std::atomic<bool> stop;
		std::mutex pollerLock;
		std::mutex gcLock;
		std::shared_ptr<std::thread> poolerLoop;

		std::vector<std::shared_ptr<zmq::socket_t>> sockets;
		std::vector<zmq::pollitem_t> pollItems;
		std::vector<std::shared_ptr<std::function<void()>>> callbacks;

		void PoolerLoop() {
			while(!stop) {
				std::lock_guard<std::mutex> lock(pollerLock);	
				auto count = zmq::poll(pollItems, std::chrono::milliseconds(1));
				if(count > 0 ) {
					auto size = pollItems.size();
					for(auto i = 0; i < size; i++) {
						auto & item = pollItems[i];
						if(item.revents & ZMQ_POLLIN) {
							item.revents = 0;
							(*callbacks[i])();
						}
					}
				}
			}
		}

	public:
		IncomingEventDispatcher() : stop(false) {
			poolerLoop = std::make_shared<std::thread>([&]() {
				PoolerLoop();
			});
		}

		~IncomingEventDispatcher() {
			stop = true; 
			poolerLoop->join();
		}

		void AddSocket(std::shared_ptr<zmq::socket_t> socket, std::shared_ptr<std::function<void()>> OnData) {
			zmq::pollitem_t item;
			item.socket = socket.get();
			item.events = ZMQ_POLLIN;
			item.fd = 0;
			item.revents = 0;
			std::unique_lock<std::mutex> gcLockGuard(gcLock);
			sockets.push_back(socket);

			std::unique_lock<std::mutex> lock(pollerLock);	
			pollItems.push_back(item);
			callbacks.push_back(OnData);
			lock.unlock();
			gcLockGuard.unlock();

		}

		void RemoveSocket(std::shared_ptr<zmq::socket_t> socket) {
			std::lock_guard<std::mutex> gcLockGuard(gcLock);
			auto it = std::find(sockets.begin(), sockets.end(), socket);
			if(it != sockets.end()) {
				auto id = it - sockets.begin();
				sockets.erase(it);
				std::lock_guard<std::mutex> lock(pollerLock);	
				pollItems.erase(pollItems.begin()+ id);
				callbacks.erase(callbacks.begin()+ id);
			}
		}
	};

	enum struct MonitorEvants : uint16_t {
		connected = ZMQ_EVENT_CONNECTED,
		connect_delayed =  ZMQ_EVENT_CONNECT_DELAYED,
		connect_retried = ZMQ_EVENT_CONNECT_RETRIED, 
		listening = ZMQ_EVENT_LISTENING,
		bind_failed = ZMQ_EVENT_BIND_FAILED,
		accepted = ZMQ_EVENT_ACCEPTED,
		accept_failed = ZMQ_EVENT_ACCEPT_FAILED,
		closed = ZMQ_EVENT_CLOSED,
		close_failed = ZMQ_EVENT_CLOSE_FAILED, 
		disconnected = ZMQ_EVENT_DISCONNECTED
	};

	inline std::string MonitorEvantToString(MonitorEvants key) {
		switch(key) {
		case MonitorEvants::connect_retried:
			return "connect_retried";
			case MonitorEvants::bind_failed:
			return "bind_failed";
			case MonitorEvants::accept_failed:
			return "accept_failed";
			case MonitorEvants::closed:
			return "closed";
			case MonitorEvants::close_failed:
			return "close_failed";
			case MonitorEvants::disconnected:
			return "disconnected";
			case MonitorEvants::connected: 
			return "connected";
			case MonitorEvants::connect_delayed: 
			return "connect_delayed";
			case MonitorEvants::listening:
			return "listening";
			case MonitorEvants::accepted: 
			return "accepted";
			default:
			return "unknown!";
		}
	}

	struct SocketDisconnectEventsMonitor {
	private:
		zmq::monitor_t monitor;
		IncomingEventDispatcher reciever;
		std::shared_ptr<zmq::context_t> ctx;
		void OnEvent() {
		}
	public:
		SocketDisconnectEventsMonitor(std::shared_ptr<zmq::context_t> ctx) : ctx(ctx){}

		std::shared_ptr<zmq::socket_t> AddSocket(std::shared_ptr<zmq::socket_t> socket, std::string socketId, std::shared_ptr<std::function<void()>> onDisconnected) {
			std::shared_ptr<zmq::socket_t> monitorSocket;
			auto monitorUrl = std::string("inproc://zmq-monitor-") + socketId;
			zmq_socket_monitor (socket.get(), monitorUrl.c_str() , 
				ZMQ_EVENT_DISCONNECTED | 
				ZMQ_EVENT_CLOSED | 
				ZMQ_EVENT_ACCEPT_FAILED |
				ZMQ_EVENT_BIND_FAILED |
				ZMQ_EVENT_CONNECT_RETRIED // NOTE: we treat first connection failer as failer (no retries policy)
				);

			auto monitor = std::make_shared<zmq::socket_t>(*ctx, zmq::socket_type::pair);
			reciever.AddSocket(monitor, onDisconnected);

			return monitor;
		}

		void RomoveSocket(std::shared_ptr<zmq::socket_t> socket) {
			reciever.RemoveSocket(socket);
		}
	};

	struct InputSocket : MQCloud::IInputSocket {
	private:
		SubscriberSocketWrapper socket;
		std::shared_ptr<MQCloud::GenericSignalHandler<const MQCloud::Message &>> handler;

		std::shared_ptr<zmq::socket_t> socketMonitor;

		std::shared_ptr<SocketDisconnectEventsMonitor> eventsMonitor;
		std::shared_ptr<IncomingEventDispatcher> eventPooler;

		std::shared_ptr<MQCloud::GenericSignalHandler<const std::string &>> disconnectHandler;

		std::string url;
	public:
		InputSocket(std::shared_ptr<zmq::context_t> ctx,
			std::shared_ptr<SocketDisconnectEventsMonitor> eventsMonitor,
			std::shared_ptr<IncomingEventDispatcher> eventPooler,
			const std::string & url) : eventsMonitor(eventsMonitor) , eventPooler(eventPooler), socket(ctx), url(url) {
				disconnectHandler = std::make_shared<MQCloud::GenericSignalHandler<std::string>>();
				handler = std::make_shared<MQCloud::GenericSignalHandler<const MQCloud::Message &>>();

				auto errorHandler = std::make_shared<std::function<void()>>([=]() {
					zmq_msg_t eventMsg;
					zmq_msg_init (&eventMsg);
					auto rc = zmq_recvmsg (socketMonitor.get(), &eventMsg, 0);
					if (rc == -1 || zmq_errno() == ETERM) {
						disconnectHandler->OnAction("socket disconnected. Context termination is performed.");
						return;
					}
#if ZMQ_VERSION_MAJOR >= 4
					auto data = static_cast<const char*>(zmq_msg_data(&eventMsg));
					zmq_event_t msgEvent;
					memcpy(&msgEvent.event, data, sizeof(uint16_t)); data += sizeof(uint16_t);
					memcpy(&msgEvent.value, data, sizeof(int32_t));
					auto zmqEvent = &msgEvent;
#else
					zmq_event_t* zmqEvent = static_cast<zmq_event_t*>(zmq_msg_data(&eventMsg));
#endif
					zmq_msg_close (&eventMsg);

#ifdef ZMQ_NEW_MONITOR_EVENT_LAYOUT
					zmq_msg_t addrMsg;
					zmq_msg_init (&addrMsg);
					rc = zmq_recvmsg (socketMonitor.get(), &addrMsg, 0);
					if (rc == -1 || zmq_errno() == ETERM) {
						disconnectHandler->OnAction("socket disconnected. Context termination is performed.");
						return;
					}
					auto str = static_cast<const char*>(zmq_msg_data (&addrMsg));
					std::string address(str, str + zmq_msg_size(&addrMsg));
					zmq_msg_close (&addrMsg);
#else
					std::string address = zmqEvent->data.connected.addr;
#endif
					auto reson =  MonitorEvants(zmqEvent->event);
					disconnectHandler->OnAction("socket disconnected: " + address + " with " + MonitorEvantToString(reson));

				});

				auto monitor = eventsMonitor->AddSocket(socket.GetRawSocket(), socket.GetSocketId(), errorHandler);
				socketMonitor.swap(monitor);

				auto messageHandler = std::make_shared<std::function<void()>>([=]() {
					auto message = socket.Read();
					handler->OnAction(*message);
				});

				eventPooler->AddSocket(socket.GetRawSocket(), messageHandler);
		}

		~InputSocket() {
			eventPooler->RemoveSocket(socket.GetRawSocket());
			eventsMonitor->RomoveSocket(socketMonitor);
		}

		std::string GetSocketId() override {
			return socket.GetSocketId();
		}
		//TODO: remove!!!
		virtual void Subscribe(std::string pattern, std::string topic) override {
			socket.Connect(url, pattern, topic);
		};


		void Connect() override {
		}

		void AddDisconnectHandler(std::shared_ptr<MQCloud::OnError> handler) override {
			disconnectHandler->AddHandler(handler);
		}

		void OnMessage(std::shared_ptr<MQCloud::OnMessageAction> action) override {
			handler->AddHandler(action);
		}
	};

	struct SubscriberSocket : MQCloud::SubscriberSocketFactory {
		std::shared_ptr<zmq::context_t> ctx;
		SubscriberSocket(std::shared_ptr<zmq::context_t> ctx) : ctx(ctx){
			socket = std::make_shared<MQCloud::IInputSocket>(ctx, zmq::socket_type::sub);
		}

		std::shared_ptr<MQCloud::IInputSocket > ConnectSubscribingSocket(std::shared_ptr<MQCloud::Socket> socket, const std::string& addr, std::shared_ptr<MQCloud::OnMessageAction> action) override {

		}
	};

	struct InputSocket : MQCloud::IInputSocket {
		std::string url;


		InputSocket(std::shared_ptr<BaseSocketWrapper> socket, std::string url ): rawSocket(socket), url(url) {
			SocketId = url + "@" + socket->SocketId;
		}

		void Connect() override{
			rawSocket->bind(url);
		}

		void AddDisconnectHandler(std::shared_ptr<MQCloud::OnError> handler) override{}

		~InputSocket() {
			rawSocket->unbind(url);
		}

		void OnMessage(std::shared_ptr<MQCloud::OnMessageAction> action) override {

		}
	};

	struct OutputSocket : MQCloud::IOutputSocket {

		OutputSocket() {

		}
		void Connect() override {

		}

		virtual void AddDisconnectHandler(std::shared_ptr<MQCloud::OnError> handler) override;
		virtual void PublishMessage(const MQCloud::Message& msg) override;

		OutputSocket() {
			if()
		}
	};

	struct PublishingSocket : MQCloud::PublishingSocketFactory<Socket>  {
		std::shared_ptr<zmq::context_t> ctx;
		PublishingSocket(std::shared_ptr<zmq::context_t>ctx) : ctx(ctx){
		}

		std::shared_ptr<MQCloud::Socket> CreatePublishingSocket() override {
			return std::make_shared<Socket>(ctx, zmq::socket_type::pub);
		}
	};




	struct BackEndConfiguration : MQCloud::BackEndConfiguration {
		std::shared_ptr<zmq::context_t> ctx;

		BackEndConfiguration() {
			ctx = std::make_shared<zmq::context_t>(4);
			subscriberSocket = std::make_shared<SubscriberSocket>(ctx);
			publishingSocket = std::make_shared<PublishingSocket>(ctx);
		}

	};

	struct BackEndFactory : MQCloud::BackEndFactory {
		std::shared_ptr<MQCloud::BackEndConfiguration> CreateContext() override {

		}
	};
}

#endif //ZMQBACKEND_HPP