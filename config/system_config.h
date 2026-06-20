#pragma once
#include <string>
#include "constants.h"

namespace DME {
    struct SystemConfig {
        // Logger
        std::string log_file     = "dme_engine.log";
        bool        log_to_file  = false;
        bool        log_to_cout  = true;

        // Cache
        size_t lru_capacity      = DEFAULT_CACHE_CAPACITY;
        size_t lfu_capacity      = DEFAULT_CACHE_CAPACITY;

        // Rate Limiter
        double token_rate        = DEFAULT_TOKEN_RATE;
        size_t bucket_capacity   = DEFAULT_BUCKET_CAPACITY;

        // Scheduler
        int    max_priority      = MAX_PRIORITY;
    };

    inline SystemConfig& get_config() {
        static SystemConfig instance;
        return instance;
    }
}
