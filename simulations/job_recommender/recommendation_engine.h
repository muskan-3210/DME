#pragma once
#include "job_matcher.h"
#include <string>
#include <vector>

namespace DME {

class RecommendationEngine {
public:
    RecommendationEngine();
    void load_sample_jobs();
    std::vector<Job> recommend(const Candidate& candidate, size_t top_n = 5) const;

private:
    JobMatcher matcher_;
};

} // namespace DME
