#include "task_schedular.h"
#include "../logger/logger.h"

namespace DME {

void TaskScheduler::submit(Task task) {
    std::lock_guard<std::mutex> lock(mtx_);
    LOG_INFO("Scheduler: submitted task [" + task.name + "] priority=" + std::to_string(task.priority));
    queue_.push(std::move(task));
}

bool TaskScheduler::cancel(int task_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    cancelled_[task_id] = true;
    LOG_INFO("Scheduler: cancelled task id=" + std::to_string(task_id));
    return true;
}

bool TaskScheduler::run_next() {
    std::lock_guard<std::mutex> lock(mtx_);
    while (!queue_.empty()) {
        Task task = queue_.top();
        queue_.pop();
        if (cancelled_.count(task.id)) {
            LOG_INFO("Scheduler: skipping cancelled task [" + task.name + "]");
            continue;
        }
        LOG_INFO("Scheduler: running task [" + task.name + "]");
        if (task.work) task.work();
        return true;
    }
    return false;
}

void TaskScheduler::run_all() {
    while (run_next()) {}
}

size_t TaskScheduler::pending_count() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.size();
}

bool TaskScheduler::empty() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.empty();
}

} // namespace DME
