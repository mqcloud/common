#include "MQCloud.h"

//// CORE C INTERFACE WRAPPER

//// MessageUtilities ////

void SetMessageTopic(Message *, const CString &);
void SetMessageData(Message *, const CString &);
void SetFreeMessage(Message *);
Message * GetNewMessage();
CString * GetMessageTopic(Message *);
CString * GetMessageData(Message *);
int GetMessageId(Message *);
int GetMessageSenderServiceId(Message *);

const struct MessageUtilities _MessageUtilities = {
	                                                  SetMessageTopic, SetMessageData, SetFreeMessage, GetNewMessage, GetMessageTopic, GetMessageData, GetMessageId, GetMessageSenderServiceId
                                                  };

//// API ////

CoreConfiguration * CreateContext(BackEnd *);

void SetEventsHandler(const CoreConfiguration * ctx, EventsHandler * handler);
void SetServiceName(const CoreConfiguration * ctx, const CString * name);
void SetExchengeAdress(const CoreConfiguration * ctx, const CoreNodeAddress * addr);

// Request-Reply, Request-Reply continuos *(nonblocking)
void Request(const CoreConfiguration * ctx, const Message * out, void (*OnReply)(const Message * in));
void RequestTarget(const CoreConfiguration * ctx, const Message * out, const ServiceId * target, void (*OnReply)(const Message * in));

void AdvertizeReplysOnTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnRequest)(const Message * in));

// Publish-Subscribe *(nonblocking)
void AdvertizePublishingOnTopic(const CoreConfiguration * ctx, const Topic * topic);
void PublishMessage(const CoreConfiguration * ctx, const Message * out);

void Subscribe(const CoreConfiguration * ctx, const Topic * topic, void (*OnMessage)(const Message * in));

// Survey-Respondent *(nonblocking, nonforcing)
void RequestMany(const CoreConfiguration * ctx, int MaxRespondents, int timeout, const Message * out, void OnReplyFromMany(const Message * replys, int replysCount));

// API 
const struct API _API = {
	                        CreateContext, SetEventsHandler, SetServiceName, SetExchengeAdress, Request, RequestTarget, AdvertizeReplysOnTopic, AdvertizePublishingOnTopic, PublishMessage, Subscribe,
	                        RequestMany
                        };

//// FrontEnd ////

int AddExtensiabiletyEventsHandler(const CoreConfiguration * ctx, ExtensiabiletyEventsHandler * handler);
void RemoveExtensiabiletyEventsHandler(const CoreConfiguration * ctx, int handlerId);

void AdvertiseTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnMessage)(const CoreMessage * in));
void SubscribeToTopic(const CoreConfiguration * ctx, const Topic * topic, void (*OnSubscribed)(const ServiceId * nodes, int count), void (*OnMessage)(struct Message * in));

void PublishMessageToAnyNode(const CoreConfiguration * ctx, const Topic * topic, const CoreMessage out);
void PublishMessageToNode(const CoreConfiguration * ctx, ServiceId * node, const CoreMessage out);
void PublishMessageToNodes(const CoreConfiguration * ctx, ServiceId * nodes, int nodesCount, const CoreMessage out);

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
