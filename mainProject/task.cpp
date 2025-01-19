#include "task.h"



void Task::execute() {
        status = Status::InProgress;
        notifyObservers();
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Симуляция выполнения
        status = Status::Completed;
        notifyObservers();
    }