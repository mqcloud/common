#ifndef ZMQBACKEND_HPP
#define ZMQBACKEND_HPP

#ifndef ZMQ_CPP11
#define ZMQ_CPP11
#endif //ZMQ_CPP11	


#include <MQCloud/BackEndFactory.hpp>
#include <zmq.hpp>
#include <sstream>
#include <map>
#include <atomic>
#include <chrono>

namespace ZMQBackEnd {
	 


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
				for(int i = 0; i < size; i++) {
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
	
class PublisherSocketWrapper {
	std::shared_ptr<zmq::socket_t> rawSocket;
	 std::vector<std::string> publishingEndpoints;

public:

	int Bind( const std::string baseUrl= "tcp://*:" ) {
		unsigned short port = 4773;
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
};

class SubscriberSocketWrapper {
	std::shared_ptr<zmq::socket_t> rawSocket;
	std::map<std::string, std::vector<std::string> > subscriptions;

	void SubscribeToTopic(const std::string & topic) {
			rawSocket->setsockopt( ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
	}

public:
	void Connect( const std::string & url, const std::string & topic) {
		auto urlIterator = subscriptions.find(url);
		if(urlIterator == subscriptions.end()) {
			rawSocket->connect(url);
			SubscribeToTopic(topic);
		} else {
			auto & topics = urlIterator->second;
			auto topicIterator = std::find(topics.begin(), topics.end(), topic);
			if(topicIterator == topics.end()) {
				SubscribeToTopic(topic);
			}
		}
	}
	
	void Disconnect(const std::string & url) {
		rawSocket->disconnect(url);
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

struct SubscriberSocket : MQCloud::SubscriberSocketFactory<Socket>  {
	std::shared_ptr<zmq::context_t> ctx;
	std::shared_ptr<Socket> socket;
	SubscriberSocket(std::shared_ptr<zmq::context_t> ctx) : ctx(ctx){
		socket = std::make_shared<Socket>(ctx, zmq::socket_type::sub);
	}

	std::shared_ptr<MQCloud::IInputSocket > ConnectSubscribingSocket(std::shared_ptr<MQCloud::Socket> socket, const std::string& addr, std::shared_ptr<MQCloud::OnMessageAction> action) override {
		
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