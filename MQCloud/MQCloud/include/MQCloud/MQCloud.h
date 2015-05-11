//MQCloud C API 
//A way to create communication system that is not dependent on MQ socket provider!
//Requires Exchange node to be online!
//If X is given as const to a function - all memory obligations are on caller
//To help memory management we provide callbacks when we stop
//In C API we provide only zero-copy based approach
#ifndef MQCloudC
#define MQCloudC

extern "C" {

	//Warning: use brains to deduce who will clean each string up - library does not do cleaning for you!
	struct CString {
		const void * data;
		int length;
	};

	typedef struct CString CString;
	typedef struct CString CoreNodeAddress;
	typedef struct CString Topic;
	typedef struct CString Pattern;
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

	typedef void * Identifier;

	//// Core ////
	// operations will de treated as sync thus after CorePublishMessage call CoreMessage message deallocation will begin
	// calls will be performed from one thread per socket

	// Publish socket (one to many)
	struct CorePublishingSocketInterface {
		void * (*CoreCreatePublishingSocket)();
		void (*CoreBindPublishingSocket)(CorePublishingSocketInterface * instance, int socketId, const CoreNodeAddress * addr);
		void (*CorePublishMessage)(CorePublishingSocketInterface * instance, int socketId, const CoreMessage * msg);
		void (*CoreDeletePublishingSocket)(CorePublishingSocketInterface * instance);
	};

	typedef struct CorePublishingSocketInterface CorePublishingSocketInterface;

	// Subscribe socket (one to many)
	struct CoreSubscriberSocketInterface {
		CoreSubscriberSocketInterface * (*CoreCreateSubscribingSocket)(void (*callback)(const CoreMessage * msg));
		void (*CoreConnectSubscribingSocket)(CoreSubscriberSocketInterface * instance, int socketId, const CoreNodeAddress * addr);
		void (*CoreDeleteSubscribingSocket)(CoreSubscriberSocketInterface * instance);
	};

	typedef struct CoreSubscriberSocketInterface CoreSubscriberSocketInterface;

	// Threading (for callbacks execution and core logic)
	struct CoreThreadManagementInterface {
		int (*CoreGetAllowedThreadPoolMaxSize)(CoreThreadManagementInterface *);
		int (*CoreCreateThread)(CoreThreadManagementInterface *);
		void (*CoreDeleteThread)(CoreThreadManagementInterface *, int threadId);
		void (*AddTask)(CoreThreadManagementInterface *, int threadId, void (*task)(), void (*callback)());
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

	struct Message;
	typedef struct Message Message;

	struct MessageUtilities {
		void (*SetMessageTopic)(Message *, const CString *, void (*OnTopicRemoval)()); // Clear CString datain OnTopicRemoval
		void (*SetMessageData)(Message *, const CString *, void (*OnDataRemoval)()); // Clear CString datain OnTopicRemoval
		void (*SetFreeMessage)(Message *);
		Message * (*GetNewMessage)();
		const CString * (*GetMessageTopic)(Message *);
		const CString * (*GetMessageData)(Message *);
		int (*GetMessageId)(Message *);
		const CString * (*GetMessageSenderServiceId)(Message *);
	};

	typedef struct MessageUtilities MessageUtilities;

	struct BackEnd {
		CoreConfiguration * (*CreateContext)();
		void (*RemoveContext)(CoreConfiguration *);
		const CString BackEndName;
	};

	typedef struct BackEnd BackEnd;

	struct ExtensiabiletyEventsHandler {
		void (*OnNodeConnectedToOther)(const ServiceId * node);
		void (*OnNodeConnectedToThis)(const ServiceId * node);
		void (*OnMessage)(const CoreMessage * in);
		void (*OnAdvertisedTopic)(const Pattern * pattern, const Topic * topic, const ServiceId * node);
		void (*OnNodeRejectedTopic)(const Pattern * pattern, const Topic * topic, const ServiceId * node);
	};

	// Extensiabilety //
	// operations will de treated as sync thus after CorePublishMessage call CoreMessage message deallocation will begin
	// calls will be performed from one thread per socket
	struct FrontEnd {
		int (*AddExtensiabiletyEventsHandler)(const CoreConfiguration * ctx, ExtensiabiletyEventsHandler * handler);
		void (*RemoveExtensiabiletyEventsHandler)(const CoreConfiguration * ctx, int handlerId);

		void (*AdvertiseTopic)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnMessage)(const CoreMessage * in));
		void (*RejectTopic)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic);

		void (*Subscribe)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnSubscribed)(const ServiceId * nodes, int count), void (*OnMessage)(const Message * in));
		void (*Unsubscribe)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic);

		void (*PublishMessageToAnyNode)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, const CoreMessage * out);
		void (*PublishMessageToAllNodes)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, const CoreMessage * out);
		void (*PublishMessageToNode)(const CoreConfiguration * ctx, const ServiceId * node, const CoreMessage * out);
		void (*PublishMessageToNodes)(const CoreConfiguration * ctx, const ServiceId * nodes, int nodesCount, const CoreMessage * out);

		void (*GetAllSubscribedNodes)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));
		void (*GetAllPublishingNodes)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));

		//Load balancing
		int (*SetGeneralNodeIdSelectionAlgorithm)(const CoreConfiguration * ctx, const ServiceId * (*algorithm)(const CoreMessage * in));
		int (*SetTopicNodeIdSelectionAlgorithm)(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, ServiceId * (*algorithm)(const CoreMessage * in));
	};

	typedef struct FrontEnd FrontEnd;

	//// UserSpace - basic Messaging sockets ////
	// Use OnSent to clean up resources\mesure send times
	struct API {
		CoreConfiguration * (*CreateContext)(BackEnd *);
		void (*SetNodesManagerContext)(const CoreConfiguration * ctx, BackEnd *);
		void (*SetEventsHandler)(const CoreConfiguration * ctx, EventsHandler * handler);
		void (*SetServiceName)(const CoreConfiguration * ctx, const CString * name);
		void (*SetExchengeAdress)(const CoreConfiguration * ctx, const CoreNodeAddress * addr);
		void (*Connect)();

		// Request-Reply, Request-Reply continuos *(nonblocking)
		void (*Request)(const CoreConfiguration * ctx, const Message * out, void (*OnSent)(), void (*OnReply)(const Message * in));
		void (*RequestTarget)(const CoreConfiguration * ctx, const Message * out, const ServiceId * target, void (*OnSent)(), void (*OnReply)(const Message * in));

		void (*AdvertizeReplysOnTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)(), void (*OnRequest)(const Message * in));
		void (*CloseReplysOnTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());

		// Publish-Subscribe *(nonblocking)
		void (*AdvertizePublishingOnTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());
		void (*PublishMessage)(const CoreConfiguration * ctx, const Message * out, void (*OnSent)());
		void (*ClosePublishingOnTopic)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());


		void (*Subscribe)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)(), void (*OnMessage)(const Message * in));
		void (*UnSubscribe)(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());

		// Survey-Respondent *(nonblocking, nonforcing)
		void (*RequestMany)(const CoreConfiguration * ctx, int MaxRespondents, int timeout, const Message * out, void (*OnSent)(), void (*OnReplyFromMany)(const Message * replys, int replysCount));
	};

	typedef struct API API;

	struct _MQCloud {
		const MessageUtilities MessageUtilities;
		const API API;
		const FrontEnd FrontEnd;
	};

	extern const struct _MQCloud MQCloud;

}
#endif // MQCloudC
