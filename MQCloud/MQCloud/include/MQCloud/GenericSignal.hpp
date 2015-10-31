#ifndef GENERICSIGNAL_HPP
#define GENERICSIGNAL_HPP

#include <MQCloud/GenericFunc.hpp>
#include <mutex>
#include <memory>
#include <vector>
#include <algorithm>

namespace MQCloud {
#if _MSC_VER >= 1700 || SWIG
	template<class T1 = _VS_EmptyType, class T2 = _VS_EmptyType, class T3 = _VS_EmptyType>
	struct GenericSignalHandler :
			GenericAction<T1, T2, T3> {

		explicit GenericSignalHandler()
			: single(false) {}

		virtual void OnAction(T1 r1, T2 r2, T3 r3) override {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			if(single) {
				handlers[0]->OnAcation(r1, r2, r3);
			} else {
				for(auto & h : handlers) {
					h->OnAction(r1, r2, r3);
				}	
			}
		}

		void AddHandler(std::shared_ptr<GenericAction<T1, T2, T3>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			handlers.push_back(handler);
			Update();
		}

		void RemoveHandler(std::shared_ptr<GenericAction<T1, T2, T3>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			auto it = std::find(handlers.begin(), handlers.end(), handler);
			if(it != handlers.end()) {
				handlers.erase(it);
				Update();
			}
		}

	private:
		std::vector<std::shared_ptr<GenericAction<T1, T2, T3>>> handlers;
		std::mutex mutex;
		bool single;
		
		void Update() {
			if(handlers.size() == 1) {
				single = true;
			} else {
				single = false;
			}
		}
	};


	template<class T1>
	struct GenericSignalHandler<T1, _VS_EmptyType, _VS_EmptyType> :
			GenericAction<T1> {

		explicit GenericSignalHandler()
			: single(false) {}

		virtual void OnAction(T1 r1) override {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			if(single) {
				handlers[0]->OnAcation(r1);
			} else {
				for(auto & h : handlers) {
					h->OnAction(r1);
				}	
			}
		}

		void AddHandler(std::shared_ptr<GenericAction<T1>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			handlers.push_back(handler);
			Update();
		}

		void RemoveHandler(std::shared_ptr<GenericAction<T1>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			auto it = std::find(handlers.begin(), handlers.end(), handler);
			if(it != handlers.end()) {
				handlers.erase(it);
				Update();
			}
		}

	private:
		std::vector<std::shared_ptr<GenericAction<T1>>> handlers;
		std::mutex mutex;
		bool single;
		
		void Update() {
			if(handlers.size() == 1) {
				single = true;
			} else {
				single = false;
			}
		}
	};

	template<class T1, class T2>
	struct GenericSignalHandler<T1, T2, _VS_EmptyType> :
			GenericAction<T1, T2> {

		explicit GenericSignalHandler()
			: single(false) {}

		virtual void OnAction(T1 r1, T2 r2) override {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			if(single) {
				handlers[0]->OnAcation(r1, r2);
			} else {
				for(auto & h : handlers) {
					h->OnAction(r1, r2);
				}	
			}
		}

		void AddHandler(std::shared_ptr<GenericAction<T1, T2>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			handlers.push_back(handler);
			Update();
		}

		void RemoveHandler(std::shared_ptr<GenericAction<T1, T2>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			auto it = std::find(handlers.begin(), handlers.end(), handler);
			if(it != handlers.end()) {
				handlers.erase(it);
				Update();
			}
		}

	private:
		std::vector<std::shared_ptr<GenericAction<T1, T2>>> handlers;
		std::mutex mutex;
		bool single;
		
		void Update() {
			if(handlers.size() == 1) {
				single = true;
			} else {
				single = false;
			}
		}
	};
	
#else

    template<class ... Types>
    struct GenericSignalHandler :
            GenericAction<Types...> {
        virtual void OnAction(Types ... result) override {
            std::lock_guard<std::mutex> lockHandlers(mutex);
			if(single) {
				handlers[0]->OnAction(result...);
			} else {
				for(auto & h : handlers) {
					h->OnAction(result...);
				}	
			}
        }

        void AddHandler(std::shared_ptr<GenericAction<Types...>> handler) {
            std::lock_guard<std::mutex> lockHandlers(mutex);
            handlers.push_back(handler);
			Update();
        }

		void RemoveHandler(std::shared_ptr<GenericAction<Types...>> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			auto it = find(handlers.begin(), handlers.end(), handler);
			if(it != handlers.end()) {
				handlers.erase(it);
				Update();
			}
		}

    private:
        std::vector<std::shared_ptr<GenericAction<Types...>>> handlers;
        std::mutex                                            mutex;
		bool single;
		
		void Update() {
			if(handlers.size() > 1) {
				single = false;
			}
		}
    };

#endif
}
#endif // GENERICSIGNAL_HPP

