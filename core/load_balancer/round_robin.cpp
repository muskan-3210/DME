#include "round_robin.h"
#include <stdexcept>

namespace DME {

RoundRobinBalancer::RoundRobinBalancer()
    : current_index_(0) {}

void RoundRobinBalancer::add_server(const Server& server) {
    std::lock_guard<std::mutex> lock(mtx_);
    servers_.push_back(server);
}

Server* RoundRobinBalancer::next_server() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (servers_.empty()) return nullptr;

    size_t start = current_index_;
    do {
        Server& s = servers_[current_index_];
        current_index_ = (current_index_ + 1) % servers_.size();
        if (s.healthy) return &s;
    } while (current_index_ != start);

    return nullptr;
}

size_t RoundRobinBalancer::server_count() const {
    return servers_.size();
}

void RoundRobinBalancer::mark_unhealthy(int server_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& s : servers_)
        if (s.id == server_id) { s.healthy = false; break; }
}

} // namespace DME
