//
// Created by Oleg Jakushkin on 06.07.15.
//

#ifndef MQCLOUD_THREADMANAGERTBBIMPL_HPP
#define MQCLOUD_THREADMANAGERTBBIMPL_HPP

#include <memory>
#include <future>
#include <MQCloud/ThreadManagement.hpp>
#include <tbb/task_group.h>

namespace MQCloud {
    namespace Internal {
        struct ThreadManagerTbbImpl :
                ThreadManagementInterface {
        private:
            tbb::task_group tasks;
        public:
            virtual void AddTask(std::shared_ptr<::MQCloud::GenericAction> action);

            virtual std::shared_ptr<TaskId> RepeatTask(int delayMilliseconds,
                                                       std::shared_ptr<::MQCloud::GenericAction> action);

            virtual ~ThreadManager() {
            }
        };
    }
}
#endif //MQCLOUD_THREADMANAGERTBBIMPL_HPP
