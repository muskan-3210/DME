#pragma once
#include <deque>
#include <cstddef>
#include <string>
#include <vector>

namespace DME {

struct Metric {
    long long timestamp_ms;
    double    value;
};

class SlidingWindowMetrics {
public:
    explicit SlidingWindowMetrics(long long window_ms);

    void   record(long long timestamp_ms, double value);
    double average() const;
    double max()     const;
    double min()     const;
    size_t count()   const;
    void   evict(long long current_time_ms);

private:
    long long           window_ms_;
    std::deque<Metric>  window_;
};

} // namespace DME
