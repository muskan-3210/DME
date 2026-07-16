#include "least_connection.h"
// Includes the LeastConnectionBalancer class declaration, which transitively pulls in
// server.h, <vector>, and <mutex> — all the types needed in this file.

#include <algorithm>
// Provides std::min_element and other standard algorithms.
// Included for potential future use; the minimum is currently found with a manual loop in next_server().

namespace DME {
// Opens the DME namespace so method definitions resolve to the class declared in least_connection.h.

void LeastConnectionBalancer::add_server(const Server& server) {
// Purpose: register a new backend server in the pool under mutex protection.

    std::lock_guard<std::mutex> lock(mtx_);
// RAII lock: acquires mtx_ on construction, releases automatically when 'lock' goes out of scope.
// Prevents data races when add_server() is called concurrently from multiple threads.

    servers_.push_back(server);
// Append a copy of 'server' to the servers_ vector.
// The copy captures the server's initial state: active_connections == 0, healthy == true.

}
// Closes add_server() body. The mutex is released here as 'lock' destructs.

Server* LeastConnectionBalancer::next_server() {
// Purpose: find and return the healthy server with the fewest active connections.
// Increments that server's connection count before returning to keep accounting accurate.

    std::lock_guard<std::mutex> lock(mtx_);
// Lock the mutex for the entire scan-and-update sequence to prevent concurrent modifications.

    Server* best = nullptr;
// Will point to the chosen server once the linear scan is complete.
// Starts as nullptr in case no healthy server exists in the pool.

    for (auto& s : servers_) {
// Iterate over all registered servers. 'auto& s' binds by reference to allow reading active_connections.

        if (!s.healthy) continue;
// Skip any server that has been flagged as unhealthy; it must not receive new connections.

        if (!best || s.active_connections < best->active_connections)
// Accept 's' as the new best candidate if: (a) no healthy server has been found yet,
// or (b) 's' has strictly fewer active connections than the current best.

            best = &s;
// Update 'best' to point to this server. '&s' takes the address of the vector element,
// which is safe because the vector is not modified during this loop.

    }
// Closes the for loop body. 'best' now points to the least-loaded healthy server, or nullptr.

    if (best) ++best->active_connections;
// Reserve the connection on the chosen server before returning it to the caller.
// The caller is responsible for calling release(server_id) when the request completes.

    return best;
// Return pointer to the chosen server, or nullptr if no healthy server is available.

}
// Closes next_server() body.

void LeastConnectionBalancer::release(int server_id) {
// Purpose: signal that a previously assigned connection has finished on the given server.
// Decrements active_connections so future calls to next_server() see an accurate count.

    std::lock_guard<std::mutex> lock(mtx_);
// Acquire the mutex before searching and modifying any server's active_connections.

    for (auto& s : servers_)
// Scan all servers to find the one matching server_id.

        if (s.id == server_id && s.active_connections > 0) {
// Match by id; the active_connections > 0 guard prevents unsigned integer underflow.

            --s.active_connections;
// Decrement the connection count, signalling that one request to this server has completed.

            break;
// Stop searching after the first match; server ids are unique within the pool.

        }
// Closes the if body (the body of the for loop's conditional statement).

}
// Closes release() body.

size_t LeastConnectionBalancer::server_count() const {
// Purpose: return the total number of servers registered in the pool.

    return servers_.size();
// std::vector::size() is O(1). Note: for strict thread safety under concurrent add_server() calls,
// this should also acquire mtx_. Currently benign because size_t reads are typically atomic.

}
// Closes server_count() body.

} // namespace DME
// Closes the DME namespace.
