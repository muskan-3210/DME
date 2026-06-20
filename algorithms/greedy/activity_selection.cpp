#include "greedy.h"
#include <algorithm>

std::vector<Activity> activity_selection(std::vector<Activity> activities) {
    std::sort(activities.begin(), activities.end(),
              [](const Activity& a, const Activity& b){ return a.end < b.end; });
    std::vector<Activity> selected;
    int last_end = -1;
    for (const auto& a : activities) {
        if (a.start >= last_end) {
            selected.push_back(a);
            last_end = a.end;
        }
    }
    return selected;
}
