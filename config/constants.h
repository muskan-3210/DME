#pragma once
#include <cstddef>

namespace DME {
    // Cache
    constexpr size_t DEFAULT_CACHE_CAPACITY  = 100;
    constexpr size_t MAX_CACHE_CAPACITY      = 10000;

    // Rate Limiter
    constexpr double DEFAULT_TOKEN_RATE      = 10.0;   // tokens per second
    constexpr size_t DEFAULT_BUCKET_CAPACITY = 50;

    // Load Balancer
    constexpr size_t MAX_SERVERS             = 32;

    // Scheduler
    constexpr int    MAX_PRIORITY            = 10;
    constexpr size_t MAX_TASKS               = 1000;

    // Search Engine
    constexpr size_t MAX_SEARCH_RESULTS      = 20;
    constexpr size_t MAX_DOCUMENTS           = 100000;

    // Analytics
    constexpr size_t SLIDING_WINDOW_SIZE     = 60;     // seconds
}
