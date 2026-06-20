#pragma once
#include <string>
#include <functional>
#include <chrono>

namespace DME {

enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELLED };

struct Task {
    int         id;
    std::string name;
    int         priority;   // higher = more urgent
    TaskStatus  status;
    std::function<void()> work;

    Task(int id, std::string name, int priority, std::function<void()> work)
        : id(id), name(std::move(name)), priority(priority),
          status(TaskStatus::PENDING), work(std::move(work)) {}

    bool operator<(const Task& other) const {
        return priority < other.priority;
    }
};

} // namespace DME
