#pragma once
// Include guard: prevents duplicate compilation of the TaskScheduler declaration.

#include "task.h"
// Includes the Task struct and TaskStatus enum defined in task.h.
// Cross-file dependency: TaskScheduler stores Task objects in its priority queue
// and invokes their 'work' callables.

#include <queue>
// Provides std::priority_queue — the max-heap container used to order tasks by priority.
// The top of the queue is always the task with the highest priority value.

#include <vector>
// Provides std::vector, which is the underlying storage container for std::priority_queue.
// std::priority_queue<T, Container, Compare> defaults to vector as its container.

#include <mutex>
// Provides std::mutex and std::lock_guard for thread-safe access to the queue and cancellation map.
// Multiple threads may call submit(), cancel(), or run_next() concurrently.

#include <unordered_map>
// Provides std::unordered_map for O(1) lookup of cancelled task ids.
// Lazy cancellation: tasks stay in the queue but are skipped when dequeued.

namespace DME {
// Opens the DME namespace, consistent with task.h and all other DME headers.

class TaskScheduler {
// Implements a thread-safe priority-based task scheduler.
// Tasks submitted with higher priority values are executed before lower-priority ones.
// Cancelled tasks are removed lazily: they remain in the queue but are skipped at dequeue time.

public:
// Public API for submitting, cancelling, and running tasks.

    void   submit(Task task);
// Enqueue a task into the priority queue under mutex protection.
// Logs the submission via LOG_INFO (defined in logger/logger.h, used in the .cpp).

    bool   cancel(int task_id);
// Mark task_id as cancelled by inserting it into the cancelled_ map.
// The task is not removed from queue_ immediately (lazy removal). Returns true always.

    bool   run_next();
// Dequeue and execute the highest-priority non-cancelled task.
// Returns true if a task ran, false if the queue is empty or only cancelled tasks remain.

    void   run_all();
// Drain the entire queue by calling run_next() in a loop until it returns false.

    size_t pending_count() const;
// Return the number of tasks currently in the queue, including any cancelled ones not yet dequeued.

    bool   empty()         const;
// Return true if the priority queue contains no tasks (the queue is fully drained).

private:
// Private implementation details.

    struct Compare {
    // Custom comparator functor for std::priority_queue.
    // Passed as the third template argument to make the queue a max-heap ordered by priority.

        bool operator()(const Task& a, const Task& b) {
        // Overloads the function-call operator so Compare instances are callable like functions.
        // std::priority_queue calls this to determine heap order during push and pop.

            return a.priority < b.priority; // max-heap: highest priority first
            // Returns true if 'a' should rank lower than 'b' in the heap.
            // std::priority_queue uses this to ensure queue_.top() always holds the highest-priority task.

        }
        // Closes operator() body.

    };
    // Closes the Compare struct body.

    std::priority_queue<Task, std::vector<Task>, Compare> queue_;
    // The max-heap priority queue storing all submitted, not-yet-dequeued Task objects.
    // Template parameters: element type Task, underlying container std::vector<Task>, heap comparator Compare.
    // queue_.top() always returns the task with the highest priority value.

    std::unordered_map<int, bool> cancelled_;
    // Tracks which task ids have been cancelled via cancel().
    // O(1) average-case lookup at dequeue time: cancelled_.count(task.id) > 0 means skip this task.
    // Lazy cancellation is used because std::priority_queue does not support random removal.

    mutable std::mutex mtx_;
    // Mutex protecting queue_ and cancelled_ from concurrent access.
    // 'mutable' allows locking in const member functions (pending_count(), empty()).

};
// Closes the TaskScheduler class body.

} // namespace DME
// Closes the DME namespace.
