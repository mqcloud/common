#include "SimplifiedAPI.h"
#include "MQCloud/MQCloud.hpp"

CoreConfiguration * CreateContext() {
	return new CoreConfigurationBase();
}

void RemoveContext(CoreConfiguration * ptr) {
	delete static_cast<CoreConfigurationBase*>(ptr);
}

const struct BackEnd ZMQBackEnd = {
	                                  CreateContext, RemoveContext
                                  };
