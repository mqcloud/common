//
// Created by Oleg Jakushkin on 12.07.15.
//

#ifndef MQCLOUD_TASKMANAGERTHREAD_HPP
#define MQCLOUD_TASKMANAGERTHREAD_HPP

#include <MQCloud/Delegates.hpp>
#include <MQCloud/TaskId.hpp>
#include <thread>
#include <future>

struct TaskManagerThread :
        TaskId {
private:
    std::shared_ptr<::MQCloud::GenericAction>        action;
    std::shared_ptr<std::thread> t;
    std::atomic<bool>            stop;
    int                          delayMilliseconds;
public:
    virtual ~TaskManagerThread();

    TaskManagerThread(const std::shared_ptr<::MQCloud::GenericAction> &action, int delay_milliseconds);

};

#endif //MQCLOUD_TASKMANAGERTHREAD_HPP
