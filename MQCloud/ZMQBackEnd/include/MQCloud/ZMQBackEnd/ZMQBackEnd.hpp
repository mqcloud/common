#ifndef ZMQBACKEND_HPP
#define ZMQBACKEND_HPP

#ifndef ZMQ_CPP11
#define ZMQ_CPP11
#endif //ZMQ_CPP11


#include <MQCloud/BackEndFactory.hpp>
#include <zmq.hpp>

namespace ZMQBackEnd {

struct Socket : MQCloud::Socket {

	std::shared_ptr<zmq::socket_t> rawSocket;

	Socket(std::shared_ptr<zmq::context_t> ctx, zmq::socket_type type) {
		rawSocket = std::make_shared< zmq::socket_t>(*ctx, type);
	}

	void Connect() override {
		rawSocket->connect()
	}

	void AddDisconnectHandler(std::shared_ptr<MQCloud::OnError> handler) override {
	
	}
};

struct SocketConnection : MQCloud::SocketConnection {
	std::shared_ptr<ZMQBackEnd::Socket> socket;
	std::string url;

	SocketConnection(std::shared_ptr<Socket> socket, std::string url ): socket(socket), url(url){}
	
	void Connect() override{
		socket->rawSocket->bind(url);
	}
	
	void AddDisconnectHandler(std::shared_ptr<MQCloud::OnError> handler) override{}
	
	~SocketConnection() {
		socket->rawSocket->unbind(url);
	}
};

struct PublishingSocket : MQCloud::PublishingSocket {
	std::shared_ptr<zmq::context_t> ctx;
	PublishingSocket(std::shared_ptr<zmq::context_t>ctx) : ctx(ctx){
	}

	std::shared_ptr<MQCloud::Socket> CreatePublishingSocket() override {
	}

	void PublishMessage(std::shared_ptr<MQCloud::Socket> socket, const MQCloud::Message& msg) override {
	}
};

struct SubscriberSocket : MQCloud::SubscriberSocket {
	std::shared_ptr<zmq::context_t> ctx;
	SubscriberSocket(std::shared_ptr<zmq::context_t> ctx) : ctx(ctx){
	}

	std::shared_ptr<MQCloud::Socket> CreateSubscribingSocket() override {
	}

	std::shared_ptr<MQCloud::SocketConnection> ConnectSubscribingSocket(std::shared_ptr<MQCloud::Socket> socket, const std::string& addr, std::shared_ptr<MQCloud::OnMessageAction> action) override {
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