#include <MQCloud/CXX/BackEndConfiguration.hpp>

#ifndef BACKENDFACTORY_HPP
#define BACKENDFACTORY_HPP

namespace MQCloud {
		//Each core can work as async and/or sync.
		//default assumption is that it  works as sync
		struct BackEndFactory {
		virtual std::shared_ptr<CoreConfiguration> CreateContext();

		virtual ~BackEndFactory() {}
	};
}

#endif // !BACKENDFACTORY_HPP
