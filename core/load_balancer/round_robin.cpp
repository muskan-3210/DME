#include "round_robin.h"
// Includes the RoundRobinBalancer class declaration and transitively pulls in
// server.h, <vector>, and <mutex> — all the types used in this file.

#include <stdexcept>
// Provides standard exception types such as std::runtime_error.
// Included here for potential future error-throwing guard conditions.

namespace DME {
// Opens the DME namespace so all method definitions below match the class declared in round_robin.h.

RoundRobinBalancer::RoundRobinBalancer()
// Constructor definition for RoundRobinBalancer.
// No parameters: the server pool starts empty and the index starts at the beginning.

    : current_index_(0) {}
// Member initializer list: sets current_index_ to 0.
// Empty body '{}' because servers_ (std::vector) default-constructs to an empty container.

void RoundRobinBalancer::add_server(const Server& server) {
// Purpose: register a new backend server under mutex protection so concurrent adds are safe.

    std::lock_guard<std::mutex> lock(mtx_);
// RAII lock: acquires mtx_ on construction, automatically releases it when 'lock' goes out of scope.
// Prevents data races when two threads call add_server() simultaneously.

    servers_.push_back(server);
// Append a copy of 'server' to the end of the servers_ vector.
// Copying is acceptable here because add_server() is typically called infrequently at setup time.

}
// Closes add_server() body. 'lock' destructs here, releasing the mutex.

Server* RoundRobinBalancer::next_server() {
// Purpose: return a pointer to the next healthy server in round-robin order; nullptr if none are healthy.

    std::lock_guard<std::mutex> lock(mtx_);
// Acquire the mutex because both servers_ and current_index_ are read and modified below.

    if (servers_.empty()) return nullptr;
// Guard: if no servers have been registered yet, there is nothing to return.

    size_t start = current_index_;
// Save the starting index so the do-while loop can detect when it has wrapped all the way around.
// Without this sentinel, the loop could run forever if all servers are unhealthy.

    do {
// do-while ensures at least one server is examined before checking the wrap-around condition.

        Server& s = servers_[current_index_];
// Obtain a reference to the server at the current index. A reference avoids copying a Server object.

        current_index_ = (current_index_ + 1) % servers_.size();
// Advance current_index_ by one, wrapping back to 0 when it reaches the end of the pool.
// Modulo arithmetic creates the circular (round-robin) traversal pattern.

        if (s.healthy) return &s;
// If this server is healthy, return its address. The next call will start from the already-advanced index.

    } while (current_index_ != start);
// Continue looping until we have visited every server exactly once.
// If current_index_ equals 'start' again, we've gone full circle and all servers are unhealthy.

    return nullptr;
// All registered servers have been checked and none are healthy; signal failure to the caller.

}
// Closes next_server() body.

size_t RoundRobinBalancer::server_count() const {
// Purpose: return the total number of servers registered in this balancer's pool.

    return servers_.size();
// std::vector::size() is O(1). Note: for strict thread safety under concurrent add_server() calls,
// this should also acquire mtx_. Currently a benign data race on read-only size access.

}
// Closes server_count() body.

void RoundRobinBalancer::mark_unhealthy(int server_id) {
// Purpose: flag a specific server as unhealthy so next_server() skips it in future rounds.

    std::lock_guard<std::mutex> lock(mtx_);
// Acquire the mutex before iterating over and modifying servers_.

    for (auto& s : servers_)
// Range-based for loop iterating over all servers by reference for in-place modification.

        if (s.id == server_id) { s.healthy = false; break; }
// When the matching server is found, set its healthy flag to false and stop searching.
// 'break' is safe here because server ids are unique — no need to continue after a match.

}
// Closes mark_unhealthy() body.

} // namespace DME
// Closes the DME namespace.
