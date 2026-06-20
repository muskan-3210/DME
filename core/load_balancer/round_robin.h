#pragma once
#include "server.h"
#include <vector>
#include <mutex>

namespace DME {

class RoundRobinBalancer {
public:
    RoundRobinBalancer();
    void    add_server(const Server& server);
    Server* next_server();
    size_t  server_count() const;
    void    mark_unhealthy(int server_id);

private:
    std::vector<Server> servers_;
    size_t              current_index_;
    mutable std::mutex  mtx_;
};

} // namespace DME
