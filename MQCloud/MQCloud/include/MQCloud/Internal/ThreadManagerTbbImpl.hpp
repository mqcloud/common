//
// Created by Oleg Jakushkin on 06.07.15.
//

#ifndef MQCLOUD_THREADMANAGERTBBIMPL_HPP
#define MQCLOUD_THREADMANAGERTBBIMPL_HPP

#include <memory>
#include <future>
#include <MQCloud/ThreadManagement.hpp>

namespace MQCloud {
    namespace Internal {
        struct ThreadManager :
                ThreadManagementInterface {

            virtual void AddTask(std::shared_ptr<::MQCloud::GenericAction> action);

            virtual std::shared_ptr<TaskId> RepeatTask(int delayMilliseconds,
                                                       std::shared_ptr<::MQCloud::GenericAction> action);

            virtual ~ThreadManager() {
            }
        };
    }
}
#endif //MQCLOUD_THREADMANAGERTBBIMPL_HPP
