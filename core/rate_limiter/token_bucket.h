#pragma once
#include <chrono>
#include <mutex>
#include <cstddef>

namespace DME {

class TokenBucket {
public:
    TokenBucket(double rate, size_t capacity);

    bool consume(size_t tokens = 1);
    void refill();
    double available_tokens() const;

private:
    double   rate_;       // tokens per second
    size_t   capacity_;
    double   tokens_;
    std::chrono::steady_clock::time_point last_refill_;
    mutable std::mutex mtx_;

    void refill_internal();
};

} // namespace DME
