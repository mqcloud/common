#include "MQCloud.h"

//// CORE C INTERFACE WRAPPER

//// MessageUtilities ////

void SetMessageTopic(Message *, const CString *, void (*OnTopicRemoval)());
void SetMessageData(Message *, const CString *, void (*OnTopicRemoval)());
void SetFreeMessage(Message *);
Message * GetNewMessage();
const CString * GetMessageTopic(Message *);
const CString * GetMessageData(Message *);
int GetMessageId(Message *);
const CString * GetMessageSenderServiceId(Message *);

const struct MessageUtilities _MessageUtilities = {
	                                                  SetMessageTopic, SetMessageData, SetFreeMessage, GetNewMessage, GetMessageTopic, GetMessageData, GetMessageId, GetMessageSenderServiceId
                                                  };

//// API ////

CoreConfiguration * CreateContext(BackEnd *);

void SetEventsHandler(const CoreConfiguration * ctx, EventsHandler * handler);
void SetServiceName(const CoreConfiguration * ctx, const CString * name);
void SetExchengeAdress(const CoreConfiguration * ctx, const CoreNodeAddress * addr);

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
	                        CreateContext, SetEventsHandler, SetServiceName, SetExchengeAdress, Request, RequestTarget, AdvertizeReplysOnTopic, CloseReplysOnTopic,
	                        AdvertizePublishingOnTopic, PublishMessage, ClosePublishingOnTopic, Subscribe, UnSubscribe, RequestMany
                        };

//// FrontEnd ////

int AddExtensiabiletyEventsHandler(const CoreConfiguration * ctx, ExtensiabiletyEventsHandler * handler);
void RemoveExtensiabiletyEventsHandler(const CoreConfiguration * ctx, int handlerId);

void AdvertiseTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnMessage)(const CoreMessage * in));
void SubscribeToTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnSubscribed)(const ServiceId * nodes, int count), void (*OnMessage)(const Message * in));

void PublishMessageToAnyNode(const CoreConfiguration * ctx, const Topic * topic, const CoreMessage * out);
void PublishMessageToNode(const CoreConfiguration * ctx, const ServiceId * node, const CoreMessage * out);
void PublishMessageToNodes(const CoreConfiguration * ctx, const ServiceId * nodes, int nodesCount, const CoreMessage * out);

void GetAllSubscribedNodes(const CoreConfiguration * ctx, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));
void GetAllPublishingNodes(const CoreConfiguration * ctx, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));

//Load balancing
int SetGeneralNodeIdSelectionAlgorithm(const CoreConfiguration * ctx, const ServiceId * (*algorithm)(const CoreMessage * in));
int SetTopicNodeIdSelectionAlgorithm(const CoreConfiguration * ctx, const Topic * topic, ServiceId * (*algorithm)(const CoreMessage * in));

const struct FrontEnd _FrontEnd = {
	                                  AddExtensiabiletyEventsHandler, RemoveExtensiabiletyEventsHandler, AdvertiseTopic, SubscribeToTopic, PublishMessageToAnyNode, PublishMessageToNode, PublishMessageToNodes,
	                                  GetAllSubscribedNodes, GetAllPublishingNodes, SetGeneralNodeIdSelectionAlgorithm, SetTopicNodeIdSelectionAlgorithm
                                  };

//// MQCloud ////

const struct _MQCloud MQCloud = {
	                                _MessageUtilities, _API, _FrontEnd
                                };
