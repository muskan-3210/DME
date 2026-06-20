#include "token_bucket.h"

namespace DME {

TokenBucket::TokenBucket(double rate, size_t capacity)
    : rate_(rate), capacity_(capacity), tokens_(static_cast<double>(capacity)),
      last_refill_(std::chrono::steady_clock::now()) {}

bool TokenBucket::consume(size_t tokens) {
    std::lock_guard<std::mutex> lock(mtx_);
    refill_internal();
    if (tokens_ < static_cast<double>(tokens)) return false;
    tokens_ -= static_cast<double>(tokens);
    return true;
}

void TokenBucket::refill() {
    std::lock_guard<std::mutex> lock(mtx_);
    refill_internal();
}

double TokenBucket::available_tokens() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return tokens_;
}

void TokenBucket::refill_internal() {
    auto now     = std::chrono::steady_clock::now();
    double secs  = std::chrono::duration<double>(now - last_refill_).count();
    tokens_      = std::min(static_cast<double>(capacity_), tokens_ + secs * rate_);
    last_refill_ = now;
}

} // namespace DME
