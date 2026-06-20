#pragma once
#include "task.h"
#include <queue>
#include <vector>
#include <mutex>
#include <unordered_map>

namespace DME {

class TaskScheduler {
public:
    void   submit(Task task);
    bool   cancel(int task_id);
    bool   run_next();
    void   run_all();
    size_t pending_count() const;
    bool   empty()         const;

private:
    struct Compare {
        bool operator()(const Task& a, const Task& b) {
            return a.priority < b.priority; // max-heap: highest priority first
        }
    };

    std::priority_queue<Task, std::vector<Task>, Compare> queue_;
    std::unordered_map<int, bool> cancelled_;
    mutable std::mutex mtx_;
};

} // namespace DME
