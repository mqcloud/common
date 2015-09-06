//
// Created by Oleg Jakushkin on 12.07.15.
//

#include <MQCloud/GenericFunc.hpp>
#include <MQCloud/TaskManagerThread.hpp>

TaskManagerThread::TaskManagerThread(const std::shared_ptr<::MQCloud::GenericAction> &action, int delay_milliseconds) :
        action(action), delayMilliseconds(delay_milliseconds) {
    stop = false;
    t    = std::make_shared<std::thread>([=, &stop]() {
        while (!stop) {
            action->OnAction();
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_milliseconds));
        }
    });
}

virtual TaskManagerThread::~TaskManagerThread() {
    stop = true;
    t->join();
}