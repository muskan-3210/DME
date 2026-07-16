#include "task_schedular.h"
// Includes the TaskScheduler class declaration, which transitively pulls in
// task.h, <queue>, <vector>, <mutex>, and <unordered_map>.

#include "../logger/logger.h"
// Includes the DME logger utility. Path goes up one directory from 'schedular/' to 'core/logger/'.
// Provides the LOG_INFO macro used throughout this file for observability and debugging.

namespace DME {
// Opens the DME namespace so all method definitions below resolve to the TaskScheduler class
// declared in task_schedular.h.

void TaskScheduler::submit(Task task) {
// Purpose: push a new task onto the priority queue under mutex protection.

    std::lock_guard<std::mutex> lock(mtx_);
// RAII lock: acquires mtx_ on construction; the mutex is automatically released when
// 'lock' goes out of scope at the end of this function.

    LOG_INFO("Scheduler: submitted task [" + task.name + "] priority=" + std::to_string(task.priority));
// Log the submission event with the task's name and numeric priority for debugging.
// std::to_string(int) converts the integer priority to a std::string for concatenation.
// LOG_INFO is a macro from logger.h — it typically prefixes a timestamp and severity level.

    queue_.push(std::move(task));
// Move 'task' into the priority queue to avoid copying the std::string name and std::function work.
// std::move transfers ownership; 'task' is in a valid-but-unspecified state after this line.
// The heap property is restored automatically by push() using the Compare functor.

}
// Closes submit() body. 'lock' destructs here, releasing the mutex.

bool TaskScheduler::cancel(int task_id) {
// Purpose: mark a task as cancelled so run_next() will skip it when it reaches the top of the queue.

    std::lock_guard<std::mutex> lock(mtx_);
// Acquire the mutex before writing to cancelled_, which is shared state.

    cancelled_[task_id] = true;
// Insert or overwrite the entry for task_id in the cancellation map. O(1) average via hash table.
// The task remains in queue_; it will be silently discarded when run_next() dequeues it.

    LOG_INFO("Scheduler: cancelled task id=" + std::to_string(task_id));
// Log the cancellation for audit trails and debugging. The id is converted to string for the message.

    return true;
// Returns true unconditionally. This is best-effort: the task may have already run or been dequeued.
// A more robust API might return false if the task id was not found in the queue.

}
// Closes cancel() body.

bool TaskScheduler::run_next() {
// Purpose: dequeue and execute the highest-priority non-cancelled task; return whether one ran.

    std::lock_guard<std::mutex> lock(mtx_);
// Lock for the full duration of the pop + execute sequence to prevent concurrent modifications.

    while (!queue_.empty()) {
// Loop because the top task might be cancelled — keep popping until a runnable task is found
// or the queue is exhausted.

        Task task = queue_.top();
// Copy the highest-priority task from the top of the max-heap.
// Copying (not moving) keeps the queue valid until we confirm the task is not cancelled.

        queue_.pop();
// Remove the top element from the priority queue. The heap is restructured internally.
// After pop(), 'task' holds the only copy of this task's data.

        if (cancelled_.count(task.id)) {
// Check the cancellation map in O(1). count() returns 1 if the key exists, 0 otherwise.
// Using count() avoids inserting a default value the way operator[] would.

            LOG_INFO("Scheduler: skipping cancelled task [" + task.name + "]");
// Log that a cancelled task was encountered and discarded, providing visibility into scheduler behavior.

            continue;
// Discard this task and loop back to examine the next highest-priority task in the queue.

        }
// Closes the "task is cancelled" branch.

        LOG_INFO("Scheduler: running task [" + task.name + "]");
// Log just before execution so timing and ordering can be observed in the logs.

        if (task.work) task.work();
// Null-check the std::function before calling it — a default-constructed or moved-from
// std::function is empty, and calling an empty std::function throws std::bad_function_call.
// task.work() invokes whatever callable was stored (lambda, functor, or function pointer).

        return true;
// A task was successfully executed. Signal success to the caller (used by run_all() to keep looping).

    }
// Closes the while loop. The queue was exhausted without finding a runnable task.

    return false;
// No runnable task was found: either the queue was empty or all remaining tasks are cancelled.

}
// Closes run_next() body.

void TaskScheduler::run_all() {
// Purpose: drain the entire queue by running tasks one by one until none remain.

    while (run_next()) {}
// run_next() returns true after executing a task, false when the queue is empty or exhausted.
// The empty loop body '{}' is intentional — all the work happens inside run_next().
// This pattern is called "drain loop" and is a common idiom for consuming a work queue.

}
// Closes run_all() body.

size_t TaskScheduler::pending_count() const {
// Purpose: return the number of tasks currently sitting in the priority queue.

    std::lock_guard<std::mutex> lock(mtx_);
// Lock even for a read because a concurrent push or pop could corrupt the internal heap state.

    return queue_.size();
// Returns the total number of tasks in the queue, including any cancelled tasks not yet dequeued.
// The count includes cancelled tasks because lazy removal does not erase them from the queue.

}
// Closes pending_count() body.

bool TaskScheduler::empty() const {
// Purpose: check whether the priority queue contains any tasks at all.

    std::lock_guard<std::mutex> lock(mtx_);
// Acquire the mutex to safely read queue_.empty() without concurrent modification interference.

    return queue_.empty();
// Returns true if no tasks are enqueued. Note: a queue containing only cancelled tasks is NOT empty
// by this definition — those tasks still occupy slots until run_next() drains them.

}
// Closes empty() body.

} // namespace DME
// Closes the DME namespace.
