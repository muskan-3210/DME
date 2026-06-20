#include "job_matcher.h"
#include <algorithm>
#include <unordered_set>

namespace DME {

void JobMatcher::add_job(const Job& job) {
    jobs_.push_back(job);
}

std::vector<Job> JobMatcher::match(const Candidate& candidate) const {
    std::vector<Job> results;
    for (const auto& job : jobs_) {
        double score = compute_match(job, candidate);
        if (score > 0.0 && job.salary >= candidate.min_salary) {
            Job j         = job;
            j.match_score = score;
            results.push_back(j);
        }
    }
    std::sort(results.begin(), results.end(),
              [](const Job& a, const Job& b){ return a.match_score > b.match_score; });
    return results;
}

size_t JobMatcher::job_count() const {
    return jobs_.size();
}

double JobMatcher::compute_match(const Job& job, const Candidate& candidate) const {
    std::unordered_set<std::string> candidate_skills(
        candidate.skills.begin(), candidate.skills.end());

    int matched = 0;
    for (const auto& skill : job.required_skills)
        if (candidate_skills.count(skill)) ++matched;

    if (job.required_skills.empty()) return 0.0;
    return static_cast<double>(matched) / job.required_skills.size();
}

} // namespace DME
