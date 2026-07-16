#pragma once
// Include guard: ensures task.h is processed only once per translation unit.
// Included by task_schedular.h which builds the scheduler machinery on top of Task.

#include <string>
// Provides std::string for the task's human-readable name field.
// Used in log messages inside task_schedular.cpp via LOG_INFO.

#include <functional>
// Provides std::function<void()> — a type-erased callable wrapper.
// Allows lambdas, free function pointers, or functors to be stored uniformly in the 'work' field.

#include <chrono>
// Provides time utilities from the C++ standard library (clocks, durations, time_points).
// Included here for potential future features such as task deadlines or scheduled delays.

namespace DME {
// Opens the DME project namespace; Task and TaskStatus live here alongside cache and load balancer types.

enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELLED };
// Scoped enum defining the four lifecycle states a Task can be in.
// 'enum class' (a C++11 scoped enum) prevents name pollution: PENDING won't clash with other enums.
// PENDING = submitted but not yet started; RUNNING = currently executing;
// COMPLETED = finished; CANCELLED = marked for skipping by TaskScheduler::cancel().

struct Task {
// Plain data struct representing a single unit of schedulable work.
// Instances are stored in TaskScheduler's std::priority_queue (see task_schedular.h).
// A struct is used because all fields are intentionally public — Task is a transparent data holder.

    int         id;
    // Unique integer identifier for this task instance.
    // TaskScheduler::cancel() uses this id to look up the task in the cancelled_ map in O(1).

    std::string name;
    // Human-readable label for this task (e.g., "compute-fibonacci", "flush-cache").
    // Used in LOG_INFO calls in task_schedular.cpp to make scheduler output meaningful.

    int         priority;   // higher = more urgent
    // Scheduling priority: tasks with higher values run before tasks with lower values.
    // TaskScheduler uses a max-heap so the task with the highest priority is always dequeued first.

    TaskStatus  status;
    // Tracks the current lifecycle state of this task. Initialized to PENDING in the constructor.
    // Could be updated to RUNNING/COMPLETED by the executor (not yet done in the current implementation).

    std::function<void()> work;
    // The callable payload — the actual work this task performs when invoked.
    // std::function<void()> accepts any zero-argument callable returning void.
    // In task_schedular.cpp, this is called as: if (task.work) task.work();

    Task(int id, std::string name, int priority, std::function<void()> work)
    // Constructor declaration: takes all four fields needed to fully initialize a Task.
    // 'name' and 'work' are taken by value (not const ref) so they can be moved into members below.

        : id(id), name(std::move(name)), priority(priority),
        // Member initializer list. std::move(name) transfers the string's heap buffer into the member
        // without copying. id and priority are cheap integers copied by value.

          status(TaskStatus::PENDING), work(std::move(work)) {}
        // All new tasks start in PENDING state. std::move(work) efficiently transfers the std::function.
        // Empty body '{}' — all initialization is done in the initializer list above.

    bool operator<(const Task& other) const {
    // Comparison operator enabling Task objects to be ordered by priority.
    // std::priority_queue requires this operator (or a custom comparator) to maintain heap order.

        return priority < other.priority;
        // Returns true if 'this' task has lower priority than 'other'.
        // std::priority_queue uses this to build a max-heap: the highest-priority task rises to the top.

    }
    // Closes operator< body.

};
// Closes the Task struct body.

} // namespace DME
// Closes the DME namespace.
