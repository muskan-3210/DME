#pragma once
#include "server.h"
#include <vector>
#include <mutex>

namespace DME {

class LeastConnectionBalancer {
public:
    void    add_server(const Server& server);
    Server* next_server();
    void    release(int server_id);
    size_t  server_count() const;

private:
    std::vector<Server> servers_;
    mutable std::mutex  mtx_;
};

} // namespace DME
