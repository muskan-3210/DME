#include "recommendation_engine.h"
#include <algorithm>

namespace DME {

RecommendationEngine::RecommendationEngine() {
    load_sample_jobs();
}

void RecommendationEngine::load_sample_jobs() {
    matcher_.add_job({1, "Software Engineer",      "Google",    {"C++","algorithms","systems"},      150000, 0.0});
    matcher_.add_job({2, "Data Scientist",          "Meta",      {"Python","ML","statistics"},         130000, 0.0});
    matcher_.add_job({3, "Backend Engineer",        "Amazon",    {"C++","distributed systems","AWS"},  140000, 0.0});
    matcher_.add_job({4, "ML Engineer",             "OpenAI",    {"Python","C++","ML","CUDA"},         170000, 0.0});
    matcher_.add_job({5, "Systems Engineer",        "Apple",     {"C++","OS","networking"},            145000, 0.0});
    matcher_.add_job({6, "Frontend Engineer",       "Netflix",   {"JavaScript","React","CSS"},          120000, 0.0});
    matcher_.add_job({7, "DevOps Engineer",         "Microsoft", {"Docker","Kubernetes","Python"},      135000, 0.0});
    matcher_.add_job({8, "Security Engineer",       "Cloudflare",{"C","networking","cryptography"},     155000, 0.0});
}

std::vector<Job> RecommendationEngine::recommend(const Candidate& candidate, size_t top_n) const {
    auto results = matcher_.match(candidate);
    if (results.size() > top_n) results.resize(top_n);
    return results;
}

} // namespace DME
