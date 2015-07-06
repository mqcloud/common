//
// Created by Oleg Jakushkin on 06.07.15.
//

#include <MQCloud/ThreadManagement.hpp>
#include <MQCloud/Internal/ThreadManagerTbbImpl.hpp>

void virtual MQCloud::Internal::ThreadManager::AddTask(std::shared_ptr<MQCloud::GenericAction> action) {
    auto f = async(std::launch::async, [=]() {
        action->OnAction();
    });
}

std::shared_ptr<TaskId> virtual MQCloud::Internal::ThreadManager::RepeatTask(int delayMilliseconds,
                                                                             std::shared_ptr<::MQCloud::GenericAction> action) {
    return std::make_shared<TaskManagerThread>(action, delayMilliseconds);
}