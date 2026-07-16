#pragma once
// Include guard: prevents duplicate inclusion of this header within a single translation unit.

#include "server.h"
// Includes the DME::Server struct defined in server.h.
// Cross-file dependency: Server is the element type of servers_; active_connections on Server
// is the metric this balancer reads and writes.

#include <vector>
// Provides std::vector for a dynamic array of Server objects.
// Linear scan over the vector is acceptable here since pool sizes are typically small.

#include <mutex>
// Provides std::mutex and std::lock_guard for thread-safe access to the server pool.
// Multiple threads may call next_server() and release() concurrently.

namespace DME {
// Opens the DME namespace, consistent with server.h, round_robin.h, and all other DME components.

class LeastConnectionBalancer {
// Implements the Least Connections load balancing algorithm.
// Each request is routed to the healthy server with the fewest currently active connections,
// preventing any single server from being overwhelmed while others sit idle.

public:
// Public API for managing the server pool and routing requests.

    void    add_server(const Server& server);
// Register a new backend server in the pool. Thread-safe via the internal mutex.

    Server* next_server();
// Return a pointer to the healthy server with the fewest active connections.
// Increments that server's active_connections count before returning so the count stays accurate.
// Returns nullptr if the pool is empty or all servers are unhealthy.

    void    release(int server_id);
// Decrement the active_connections count of the server with the given id.
// Must be called when a request finishes; failure to call release() causes connection counts to drift.

    size_t  server_count() const;
// Return the total number of servers registered in the pool (healthy and unhealthy).

private:
// Private implementation details not visible to callers.

    std::vector<Server> servers_;
// Stores all registered Server objects by value.
// The active_connections field inside each Server is updated in-place by next_server() and release().

    mutable std::mutex  mtx_;
// Protects servers_ from concurrent reads and writes.
// 'mutable' allows locking in const methods (server_count()).

};
// Closes the LeastConnectionBalancer class body.

} // namespace DME
// Closes the DME namespace.
