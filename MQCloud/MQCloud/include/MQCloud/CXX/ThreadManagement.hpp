#include <MQCloud/CXX/Delegates.hpp>
#include <memory>

#ifndef THREADMANAGEMENT_HPP
#define THREADMANAGEMENT_HPP

namespace MQCloud {

	struct TaskId {
		virtual ~TaskId();
	};

	// Threading (for callbacks execution and core logic)
	struct CoreThreadManagement {
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

		virtual ~CoreThreadManagement();
	};
}

#endif // !THREADMANAGEMENT_HPP
