#pragma once
// Include guard: prevents duplicate inclusion of this header within one translation unit.

#include "server.h"
// Includes the DME::Server struct definition from server.h.
// Cross-file dependency: Server is the element type of servers_ and the return type of next_server().

#include <vector>
// Provides std::vector for a resizable, index-accessible array of Server objects.
// Index-based access is needed for round-robin traversal via current_index_.

#include <mutex>
// Provides std::mutex (the lock primitive) and std::lock_guard (RAII wrapper) for thread safety.
// Multiple threads may call add_server(), next_server(), or mark_unhealthy() concurrently.

namespace DME {
// Opens the DME namespace, consistent with server.h and all other DME headers.

class RoundRobinBalancer {
// Implements the Round Robin load balancing algorithm.
// Each call to next_server() advances to the next healthy server in a circular sequence,
// distributing requests evenly across all healthy backends over time.

public:
// Public API for registering servers and requesting the next one to serve a request.

    RoundRobinBalancer();
// Default constructor declaration; initializes current_index_ to 0 (start of the pool).

    void    add_server(const Server& server);
// Register a new server by appending a copy of it to the pool. Thread-safe via mtx_.

    Server* next_server();
// Return a pointer to the next healthy server in round-robin order.
// Returns nullptr if the pool is empty or all servers are unhealthy.
// Advances current_index_ and wraps using modulo to create a circular sequence.

    size_t  server_count() const;
// Return the total number of registered servers (healthy and unhealthy combined).

    void    mark_unhealthy(int server_id);
// Set the 'healthy' flag to false on the server whose id matches server_id.
// next_server() will skip unhealthy servers when cycling through the pool.

private:
// Private implementation details not exposed to callers.

    std::vector<Server> servers_;
// Stores all registered Server objects by value in insertion order.
// std::vector is chosen over std::list because index-based access (servers_[current_index_])
// is O(1) and required for efficient round-robin traversal.

    size_t              current_index_;
// The index of the server to be considered on the next call to next_server().
// Advances by 1 modulo servers_.size() on each call, creating the circular round-robin pattern.

    mutable std::mutex  mtx_;
// Mutex protecting servers_ and current_index_ from concurrent reads and writes.
// 'mutable' allows the mutex to be locked even inside const methods (e.g., server_count()).

};
// Closes the RoundRobinBalancer class body.

} // namespace DME
// Closes the DME namespace.
