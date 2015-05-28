#include <MQCloud/MQCloud.hpp>

//// CORE C INTERFACE WRAPPER

//// MessageUtilities ////
typedef MQCloud::Message Message;
//using namespace MQCloud;

void SetMessageTopic(Message * m, const CString * s, void (*OnTopicRemoval)()) {
	m->data.SetData(s, OnTopicRemoval);
}

void SetMessageData(Message * m, const CString * s, void (*OnDataRemoval)()) {
	m->data.SetData(s, OnDataRemoval);
}

void SetFreeMessage(Message * m) {
	delete m;
}

Message * GetNewMessage() {
	return new Message();
}

const CString * GetMessageTopic(Message * m) {
	return m->topic.GetData();
}

const CString * GetMessageData(Message * m) {
	return m->data.GetData();
}

int GetMessageId(Message * m) {
	return m->GetMessageId();
}

const CString * GetMessageSenderServiceId(Message * m) {
	return m->serviceId.GetData();
}

const struct MessageUtilities _MessageUtilities = {
	                                                  SetMessageTopic,
	                                                  SetMessageData,
	                                                  SetFreeMessage,
	                                                  GetNewMessage,
	                                                  GetMessageTopic,
	                                                  GetMessageData,
	                                                  GetMessageId,
	                                                  GetMessageSenderServiceId
                                                  };

//// API ////

CoreConfiguration * CreateContext(BackEnd *);
void SetNodesManagerContext(const CoreConfiguration * ctx, BackEnd *);

void SetEventsHandler(const CoreConfiguration * ctx, EventsHandler * handler);
void SetServiceName(const CoreConfiguration * ctx, const CString * name);
void SetExchengeAdress(const CoreConfiguration * ctx, const CoreNodeAddress * addr);
void Connect();

// Request-Reply, Request-Reply continuos *(nonblocking)
void Request(const CoreConfiguration * ctx, const Message * out, void (*OnSent)(), void (*OnReply)(const Message * in));
void RequestTarget(const CoreConfiguration * ctx, const Message * out, const ServiceId * target, void (*OnSent)(), void (*OnReply)(const Message * in));

void AdvertizeReplysOnTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)(), void (*OnRequest)(const Message * in));
void CloseReplysOnTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());

// Publish-Subscribe *(nonblocking)
void AdvertizePublishingOnTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());
void PublishMessage(const CoreConfiguration * ctx, const Message * out, void (*OnSent)());
void ClosePublishingOnTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());

void Subscribe(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)(), void (*OnMessage)(const Message * in));
void UnSubscribe(const CoreConfiguration * ctx, const Topic * topic, void (*OnSent)());
// Survey-Respondent *(nonblocking, nonforcing)
void RequestMany(const CoreConfiguration * ctx, int MaxRespondents, int timeout, const Message * out, void (*OnSent)(), void (*OnReplyFromMany)(const Message * replys, int replysCount));

// API 
const struct API _API = {
	                        CreateContext,
	                        SetNodesManagerContext,
	                        SetEventsHandler,
	                        SetServiceName,
	                        SetExchengeAdress,
	                        Connect,
	                        Request,
	                        RequestTarget,
	                        AdvertizeReplysOnTopic,
	                        CloseReplysOnTopic,
	                        AdvertizePublishingOnTopic,
	                        PublishMessage,
	                        ClosePublishingOnTopic,
	                        Subscribe,
	                        UnSubscribe,
	                        RequestMany
                        };

//// FrontEnd ////

int AddExtensiabiletyEventsHandler(const CoreConfiguration * ctx, ExtensiabiletyEventsHandler * handler);
void RemoveExtensiabiletyEventsHandler(const CoreConfiguration * ctx, int handlerId);

void AdvertiseTopic(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnMessage)(const CoreMessage * in));
void RejectTopic(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic);
void Subscribe(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnSubscribed)(const ServiceId * nodes, int count), void (*OnMessage)(const Message * in));
void Unsubscribe(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic);

void PublishMessageToAnyNode(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, const CoreMessage * out);
void PublishMessageToAllNodes(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, const CoreMessage * out);
void PublishMessageToNode(const CoreConfiguration * ctx, const ServiceId * node, const CoreMessage * out);
void PublishMessageToNodes(const CoreConfiguration * ctx, const ServiceId * nodes, int nodesCount, const CoreMessage * out);

void GetAllSubscribedNodes(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));
void GetAllPublishingNodes(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));

//Load balancing
int SetGeneralNodeIdSelectionAlgorithm(const CoreConfiguration * ctx, const ServiceId * (*algorithm)(const CoreMessage * in));
int SetTopicNodeIdSelectionAlgorithm(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, ServiceId * (*algorithm)(const CoreMessage * in));

const struct FrontEnd _FrontEnd = {
	                                  AddExtensiabiletyEventsHandler,
	                                  RemoveExtensiabiletyEventsHandler,
	                                  AdvertiseTopic,
	                                  RejectTopic,
	                                  Subscribe,
	                                  Unsubscribe,
	                                  PublishMessageToAnyNode,
	                                  PublishMessageToAllNodes,
	                                  PublishMessageToNode,
	                                  PublishMessageToNodes,
	                                  GetAllSubscribedNodes,
	                                  GetAllPublishingNodes,
	                                  SetGeneralNodeIdSelectionAlgorithm,
	                                  SetTopicNodeIdSelectionAlgorithm
                                  };

//// MQCloud ////

const struct _MQCloud MQCloud = {
	                                _MessageUtilities,
	                                _API,
	                                _FrontEnd
                                };
