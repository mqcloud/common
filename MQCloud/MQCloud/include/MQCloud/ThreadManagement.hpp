#include <MQCloud/Delegates.hpp>
#include <memory>

#ifndef THREADMANAGEMENT_HPP
#define THREADMANAGEMENT_HPP

#include <thread>
#include <future>
#include "TaskManagerThread.hpp"

namespace MQCloud {

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

}

#endif // !THREADMANAGEMENT_HPP

