//MQCloud C API 
//A way to create communication system that is not dependent on MQ socket provider!
//Requires Exchange node to be online!
extern "C" {
	//// General ////
	struct ServiceId {
		char* name;
		int nameLength;
	};

	struct Message {
		int messaageId;

		char * topic;
		int topicLength;

		void * data;
		int dataLength;

		char * senderId;
		int senderIdLength;
	};

	struct Topic {
		char * topic;
		int topicLength;
	};

	//// Core ////
	struct CoreNodeAddress {
		char * addr;
		int addrLength;
	};

	// Publish socket (one to many)
	struct CorePublishingSocketInterface {
		void * (*CoreCreatePublishingSocket)();
		void (*CoreBindPublishingSocket)(void* socketPtr, struct CoreNodeAddress * addr);
		void (*CorePublishMessage)(void * socketPtr, struct Message * msg);
		void (*CoreDeletePublishingSocket)(void * socketPtr);
	};

	// Subscribe socket (one to many)
	struct CoreSubscriberSocketInterface {
		void * (*CoreCreateSubscribingSocket)(void (*callback)(struct Message * msg));
		void (*CoreConnectSubscribingSocket)(void* socketPtr, struct CoreNodeAddress* addr);
		void (*CoreDeleteSubscribingSocket)(void * socketPtr);
	};

	// Threading (for callbacks execution and core logic)
	struct CoreThreadManagementInterface {
		  int (*CoreGetAllowedThreadPoolMaxSize)();
		  void * (*CoreCreateThread)();
		  void (*CoreDeleteThread)(void * threadPtr);
		  void * (*CoreGetNativeThreadPtr)(void * threadPtr);
	};

	struct CoreConfiguration {
		void * ThreadingInterface;
		void * SubscriberSocketInterface;
		void * PublishingSocketInterface;
	};

	//// UserSpace - basic Messaging sockets ////

	//Message from messages pool
	struct Message * NewMessage(char * topic, int topicLength, void * data, int dataLength);
	void FreeMessage(struct Message * msg);

	//ServiceId from Message
	struct ServiceId * GetSenderId(struct Message * msg);

	// Configuration
	struct ServiceName {
		char* name;
		int nameLength;
	};

	struct EventsHandler {
		void (*OnSendError)(int errorCode, int messageId);
		void (*OnReadError)(int errorCode);
		void (*OnExchangeError)(int errorCode);
		void (*OnConnectedToExchange)();
		void (*OnServiceRegistred)(ServiceId * uid);
		void (*OnNewSubscriber)(struct Topic * topic, ServiceId * subscriber);
	};

	void SetEventsHandler(EventsHandler * handler);
	void SetServiceName(struct ServiceName * name);
	void SetExchengeAdress(char* addr, int length);
      
	// Request-Reply, Request-Reply continuos *(nonblocking)
	void Request(struct Message * out, void (*OnReply)(struct Message * in));// out targetId
	void RequestTarget( struct Message * out, struct ServiceId * target, void (*OnReply)(struct Message * in));

	void AdvertizeReplysOnTopic(struct Topic * topic, void (*OnRequest)(struct Message * in));

	// Publish-Subscribe *(nonblocking)
	void AdvertizePublishingOnTopic(struct Topic * topic);
	void PublishMessage(struct Message * out);

	void Subscribe(struct Topic * topic, void (*OnMessage)(struct Message * in));
      
	// Survey-Respondent *(nonblocking, nonforcing)
	void RequestMany(int MaxRespondents, int timeout, struct Message * out, void OnReplyFromMany(struct Message * replys, int replysCount)); // out targetId


	// Extensiabilety //
	//Messaging
	struct ExtensiabiletyEventsHandler {
		void (*OnNodeConnectedToOther)(struct ServiceId * node);
		void (*OnNodeConnectedToThis)(struct ServiceId * node);
		void (*OnMessage)(struct Message * in);
	};

	int AddExtensiabiletyEventsHandler(ExtensiabiletyEventsHandler * handler);
	void RemoveExtensiabiletyEventsHandler(int handlerId);

	void AdvertiseTopic(struct Topic * topic, void (*OnMessage)(struct Message * in));
	void SubscribeToTopic(struct Topic * topic, void (*OnSubscribed)(struct ServiceId * nodes, int count), void (*OnMessage)(struct Message * in));

	void PublishMessageToAnyNode(struct Topic * topic, struct Message out);
	void PublishMessageToNode(ServiceId * node, struct Message out);
	void PublishMessageToNodes(ServiceId * nodes, int nodesCount, struct Message out);

	void GetAllSubscribedNodes(struct Topic * topic, void (*OnResult)(struct ServiceId * nodes, int count));
	void GetAllPublishingNodes(struct Topic * topic, void (*OnResult)(struct ServiceId * nodes, int count));

	//Load balancing
	int SetGeneralNodeIdSelectionAlgorithm(struct ServiceId * (*algorithm)(struct Message * in));
	int SetTopicNodeIdSelectionAlgorithm(struct Topic * topic, struct ServiceId * (*algorithm)(struct Message * in));
}