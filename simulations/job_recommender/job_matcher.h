#pragma once
#include <string>
#include <vector>

namespace DME {

struct Job {
    int                      id;
    std::string              title;
    std::string              company;
    std::vector<std::string> required_skills;
    int                      salary;
    double                   match_score;
};

struct Candidate {
    std::string              name;
    std::vector<std::string> skills;
    int                      min_salary;
};

class JobMatcher {
public:
    void add_job(const Job& job);
    std::vector<Job> match(const Candidate& candidate) const;
    size_t job_count() const;

private:
    std::vector<Job> jobs_;
    double compute_match(const Job& job, const Candidate& candidate) const;
};

} // namespace DME
