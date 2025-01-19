#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>


class Observer {
public:
    virtual void update(const std::string& taskName, const std::string& status) = 0;
};

class TaskStatusObserver : public Observer {
public:
    void update(const std::string& taskName, const std::string& status) override {
        std::cout << "Task '" << taskName << "' status changed to: " << status << std::endl;
    }
};

class Task {
public:
    enum class Status { Pending, InProgress, Completed };

    Task(std::string name, std::string description)
        : name(std::move(name)), description(std::move(description)), status(Status::Pending) {}

    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void notifyObservers() {
        for (auto observer : observers) {
            observer->update(name, statusToString());
        }
    }

    void execute();

    std::string getStatus() const { return statusToString(); }
    std::string getName() const { return name; }
    std::string getDescription() const { return description;}

private:
    std::string statusToString() const {
        switch (status) {
            case Status::Pending: return "Pending";
            case Status::InProgress: return "InProgress";
            case Status::Completed: return "Completed";
        }
        return "Unknown";
    }
    std::string name;
    std::string description;
    Status status;
    std::vector<Observer*> observers;
};
