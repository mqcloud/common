//MQCloud C API 
//A way to create communication system that is not dependent on MQ socket provider!
//Requires Exchange node to be online!

extern "C" {

	struct CString {
		const char * data;
		int length;
	};

	typedef struct CString CString;
	typedef struct CString CoreNodeAddress;
	typedef struct CString Topic;
	typedef struct CString ServiceId;

	struct CoreMessage {
		int messaageId;

		Topic topic;

		CString data;

		CString serviceId;

		CString PatternName;

		CString PatternData;
	};

	typedef struct CoreMessage CoreMessage;

	//// Core ////
	// Publish socket (one to many)
	struct CorePublishingSocketInterface {
		void * (*CoreCreatePublishingSocket)();
		void (*CoreBindPublishingSocket)(int socketId, const CoreNodeAddress * addr);
		void (*CorePublishMessage)(int socketId, const CoreMessage * msg);
		void (*CoreDeletePublishingSocket)(void * socketPtr);
	};

	typedef struct CorePublishingSocketInterface CorePublishingSocketInterface;

	// Subscribe socket (one to many)
	struct CoreSubscriberSocketInterface {
		int (*CoreCreateSubscribingSocket)(void (*callback)(const CoreMessage * msg));
		void (*CoreConnectSubscribingSocket)(int socketId, const CoreNodeAddress * addr);
		void (*CoreDeleteSubscribingSocket)(int socketId);
	};

	typedef struct CoreSubscriberSocketInterface CoreSubscriberSocketInterface;

	// Threading (for callbacks execution and core logic)
	struct CoreThreadManagementInterface {
		int (*CoreGetAllowedThreadPoolMaxSize)();
		int (*CoreCreateThread)();
		void (*CoreDeleteThread)(int threadId);
		void (*AddTask)(int threadId, void (*task)(), void (*callback)());
	};

	struct CoreConfiguration {
		CoreThreadManagementInterface * ThreadingInterface;
		CoreSubscriberSocketInterface * SubscriberSocketInterface;
		CorePublishingSocketInterface * PublishingSocketInterface;
	};

	typedef struct CoreConfiguration CoreConfiguration;

	// Configuration
	struct EventsHandler {
		void (*OnSendError)(int errorCode, int messageId);
		void (*OnReadError)(int errorCode);
		void (*OnExchangeError)(int errorCode);
		void (*OnConnectedToExchange)();
		void (*OnServiceRegistred)(ServiceId * uid);
		void (*OnNewSubscriber)(struct Topic * topic, ServiceId * subscriber);
	};

	typedef struct Message Message;

	struct MessageUtilities {
		void (*SetMessageTopic)(Message *, const CString &);
		void (*SetMessageData)(Message *, const CString &);
		void (*SetFreeMessage)(Message *);
		Message * (*GetNewMessage)();
		CString * (*GetMessageTopic)(Message *);
		CString * (*GetMessageData)(Message *);
		int (*GetMessageId)(Message *);
		int (*GetMessageSenderServiceId)(Message *);
	};

	typedef struct MessageUtilities MessageUtilities;

	struct BackEnd {
		CoreConfiguration * (*CreateContext)();
	};

	typedef struct BackEnd BackEnd;

	struct ExtensiabiletyEventsHandler {
		void (*OnNodeConnectedToOther)(const ServiceId * node);
		void (*OnNodeConnectedToThis)(const ServiceId * node);
		void (*OnMessage)(const CoreMessage * in);
	};

	// Extensiabilety //
	struct FrontEnd {
		int (*AddExtensiabiletyEventsHandler)(const CoreConfiguration * ctx, ExtensiabiletyEventsHandler * handler);
		void (*RemoveExtensiabiletyEventsHandler)(const CoreConfiguration * ctx, int handlerId);

		void (*AdvertiseTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnMessage)(const CoreMessage * in));
		void (*SubscribeToTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSubscribed)(const ServiceId * nodes, int count), void (*OnMessage)(struct Message * in));

		void (*PublishMessageToAnyNode)(const CoreConfiguration * ctx, const Topic * topic, const CoreMessage out);
		void (*PublishMessageToNode)(const CoreConfiguration * ctx, ServiceId * node, const CoreMessage out);
		void (*PublishMessageToNodes)(const CoreConfiguration * ctx, ServiceId * nodes, int nodesCount, const CoreMessage out);

		void (*GetAllSubscribedNodes)(const CoreConfiguration * ctx, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));
		void (*GetAllPublishingNodes)(const CoreConfiguration * ctx, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));

		//Load balancing
		int (*SetGeneralNodeIdSelectionAlgorithm)(const CoreConfiguration * ctx, const ServiceId * (*algorithm)(const CoreMessage * in));
		int (*SetTopicNodeIdSelectionAlgorithm)(const CoreConfiguration * ctx, const Topic * topic, ServiceId * (*algorithm)(const CoreMessage * in));
	};

	typedef struct FrontEnd FrontEnd;

	//// UserSpace - basic Messaging sockets ////
	struct API {
		CoreConfiguration * (*CreateContext)(BackEnd *);
		void (*SetEventsHandler)(const CoreConfiguration * ctx, EventsHandler * handler);
		void (*SetServiceName)(const CoreConfiguration * ctx, const CString * name);
		void (*SetExchengeAdress)(const CoreConfiguration * ctx, const CoreNodeAddress * addr);

		// Request-Reply, Request-Reply continuos *(nonblocking)
		void (*Request)(const CoreConfiguration * ctx, const Message * out, void (*OnReply)(const Message * in));
		void (*RequestTarget)(const CoreConfiguration * ctx, const Message * out, const ServiceId * target, void (*OnReply)(const Message * in));

		void (*AdvertizeReplysOnTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnRequest)(const Message * in));

		// Publish-Subscribe *(nonblocking)
		void (*AdvertizePublishingOnTopic)(const CoreConfiguration * ctx, const Topic * topic);
		void (*PublishMessage)(const CoreConfiguration * ctx, const Message * out);

		void (*Subscribe)(const CoreConfiguration * ctx, const Topic * topic, void (*OnMessage)(const Message * in));

		// Survey-Respondent *(nonblocking, nonforcing)
		void (*RequestMany)(const CoreConfiguration * ctx, int MaxRespondents, int timeout, const Message * out, void OnReplyFromMany(const Message * replys, int replysCount));
	};

	typedef struct API API;

	struct _MQCloud {
		const MessageUtilities MessageUtilities;
		const API API;
		const FrontEnd FrontEnd;
	};

	extern const struct _MQCloud MQCloud;

}
