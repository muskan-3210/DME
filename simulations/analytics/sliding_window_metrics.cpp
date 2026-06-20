#include "sliding_window_metrics.h"
#include <stdexcept>
#include <limits>

namespace DME {

SlidingWindowMetrics::SlidingWindowMetrics(long long window_ms)
    : window_ms_(window_ms) {}

void SlidingWindowMetrics::record(long long timestamp_ms, double value) {
    window_.push_back({timestamp_ms, value});
}

void SlidingWindowMetrics::evict(long long current_time_ms) {
    long long cutoff = current_time_ms - window_ms_;
    while (!window_.empty() && window_.front().timestamp_ms < cutoff)
        window_.pop_front();
}

double SlidingWindowMetrics::average() const {
    if (window_.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& m : window_) sum += m.value;
    return sum / static_cast<double>(window_.size());
}

double SlidingWindowMetrics::max() const {
    if (window_.empty()) return 0.0;
    double m = std::numeric_limits<double>::lowest();
    for (const auto& metric : window_) if (metric.value > m) m = metric.value;
    return m;
}

double SlidingWindowMetrics::min() const {
    if (window_.empty()) return 0.0;
    double m = std::numeric_limits<double>::max();
    for (const auto& metric : window_) if (metric.value < m) m = metric.value;
    return m;
}

size_t SlidingWindowMetrics::count() const {
    return window_.size();
}

} // namespace DME
