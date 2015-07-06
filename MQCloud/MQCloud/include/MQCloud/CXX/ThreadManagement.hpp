#include <MQCloud/CXX/Delegates.hpp>
#include <memory>

#ifndef THREADMANAGEMENT_HPP
#define THREADMANAGEMENT_HPP
#include <thread>
#include <future>

namespace MQCloud {
	struct TaskId {
		virtual ~TaskId(){}
	};

	// Threading (for callbacks execution and core logic)
	struct ThreadManagementInterface {
		/*
	@param task to execute
	*/
		virtual void AddTask(std::shared_ptr<Task> action);


		/*
	@return task
	@param repeat each time
	@param task to execute
	*/
		virtual std::shared_ptr<TaskId> RepeatTask(int delayMilliseconds, std::shared_ptr<Task> action);

		virtual ~ThreadManagementInterface();
	};

	struct TaskManagerThread : TaskId {
	private:
		std::shared_ptr<Task> action;
		std::shared_ptr<std::thread> t;
		std::atomic<bool> stop;
		int delayMilliseconds;
	public:
		virtual ~TaskManagerThread() override {
			stop = true;
			t->join();
		}
		TaskManagerThread(const std::shared_ptr<Task>& action, int delay_milliseconds)
			: action(action),
			  delayMilliseconds(delay_milliseconds) {
			stop = false;
			t = std::make_shared<std::thread>([=, &stop]() {
				while (!stop) {
					action->OnAction();
					std::this_thread::sleep_for(std::chrono::milliseconds(delay_milliseconds));
				}
			});
		}

	};

	struct ThreadManager : ThreadManagementInterface {

		virtual void AddTask(std::shared_ptr<Task> action) override {
			auto f =std::async(std::launch::async, [=]()
			{
				action->OnAction();
			});
		}

		virtual std::shared_ptr<TaskId> RepeatTask(int delayMilliseconds, std::shared_ptr<Task> action) override {
			return std::make_shared<TaskManagerThread>(action, delayMilliseconds);
		}

		virtual ~ThreadManager() override {}
	};
}

#endif // !THREADMANAGEMENT_HPP

