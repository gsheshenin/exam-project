#include "task.h"
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>



class TaskExecutionStrategy {
public:
    virtual void executeTasks(std::vector<Task>& tasks) = 0;
};

class SequentialExecution : public TaskExecutionStrategy {
public:
    void executeTasks(std::vector<Task>& tasks) override {
        for (auto& task : tasks) {
            task.execute();
        }
    }
};

class ParallelExecution : public TaskExecutionStrategy {
public:
    void executeTasks(std::vector<Task>& tasks) override {
        std::vector<std::thread> threads;
        for (auto& task : tasks) {
            threads.emplace_back([&task]() { task.execute(); });
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }
};

template <typename T>
class TaskManager {
public:
    void addTask(const T& task) {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks.push_back(task);
    }

    void removeTask(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
            [&name](const T& task) { return task.getName() == name; }), tasks.end());
    }

    void executeTasks() {
        strategy->executeTasks(tasks);
    }

    void showTasks() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& task : tasks) {
            std::cout << "Task: " << task.getName() << " Status: " << task.getStatus() << std::endl;
        }
    }

    void set_strategy(std::unique_ptr<TaskExecutionStrategy> new_strategy){
        strategy = std::move(new_strategy);
    }

    std::vector<Task> getTasks() {
        std::lock_guard<std::mutex> lock(mutex_);
        return tasks;
    }

private:
    std::vector<T> tasks;
    std::mutex mutex_;
    std::unique_ptr<TaskExecutionStrategy> strategy;
};
