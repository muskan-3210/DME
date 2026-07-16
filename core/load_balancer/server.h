#pragma once
// Include guard: ensures this header is compiled only once per translation unit.
// server.h is a shared dependency — both round_robin.h and least_connection.h include it.

#include <string>
// Provides std::string for the server's network address field (e.g., "10.0.0.1" or "app-host-1").

#include <cstddef>
// Provides the portable unsigned type size_t, used for the active_connections counter.

namespace DME {
// Opens the DME project namespace, shared by all load balancer and cache components.

struct Server {
// Plain data struct representing a single backend server in the load balancer pool.
// Used by RoundRobinBalancer (round_robin.h) and LeastConnectionBalancer (least_connection.h).
// A struct is used instead of a class because all fields are public by default — no invariants to hide.

    int         id;
    // Unique numeric identifier for this server instance.
    // Used by mark_unhealthy() (round_robin.cpp) and release() (least_connection.cpp) to target
    // a specific server without scanning the pool by address.

    std::string address;
    // Network address or hostname of this server (e.g., "192.168.1.10").
    // Stored as std::string for flexibility; could be an IP or a DNS name.

    int         port;
    // TCP port on which this server accepts incoming connections (e.g., 8080, 443).

    size_t      active_connections;
    // Count of connections currently routed to this server and not yet released.
    // LeastConnectionBalancer reads and increments this to pick the least-loaded server.

    bool        healthy;
    // Whether this server is currently considered available for routing.
    // Both RoundRobinBalancer and LeastConnectionBalancer skip servers where healthy == false.

    Server(int id, std::string addr, int port)
    // Constructor declaration: takes an id, an address string, and a port number.
    // 'addr' is taken by value (not const ref) so it can be moved into the 'address' member below.

        : id(id), address(std::move(addr)), port(port),
        // Member initializer list. std::move(addr) transfers ownership of the string buffer
        // into 'address' without copying, which is efficient for long address strings.
        // 'id' and 'port' are cheap integers — copied directly.

          active_connections(0), healthy(true) {}
        // New servers start with zero active connections and are assumed healthy until told otherwise.
        // The empty body '{}' means all construction work is done in the initializer list above.

};
// Closes the Server struct body.

} // namespace DME
// Closes the DME namespace.
