#include "least_connection.h"
#include <algorithm>

namespace DME {

void LeastConnectionBalancer::add_server(const Server& server) {
    std::lock_guard<std::mutex> lock(mtx_);
    servers_.push_back(server);
}

Server* LeastConnectionBalancer::next_server() {
    std::lock_guard<std::mutex> lock(mtx_);
    Server* best = nullptr;
    for (auto& s : servers_) {
        if (!s.healthy) continue;
        if (!best || s.active_connections < best->active_connections)
            best = &s;
    }
    if (best) ++best->active_connections;
    return best;
}

void LeastConnectionBalancer::release(int server_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& s : servers_)
        if (s.id == server_id && s.active_connections > 0) {
            --s.active_connections;
            break;
        }
}

size_t LeastConnectionBalancer::server_count() const {
    return servers_.size();
}

} // namespace DME
