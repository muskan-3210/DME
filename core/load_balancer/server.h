#pragma once
#include <string>
#include <cstddef>

namespace DME {

struct Server {
    int         id;
    std::string address;
    int         port;
    size_t      active_connections;
    bool        healthy;

    Server(int id, std::string addr, int port)
        : id(id), address(std::move(addr)), port(port),
          active_connections(0), healthy(true) {}
};

} // namespace DME
