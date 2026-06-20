#include "dp.h"
#include <algorithm>

int lis(const std::vector<int>& arr) {
    if (arr.empty()) return 0;
    std::vector<int> tails;
    for (int x : arr) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else                   *it = x;
    }
    return static_cast<int>(tails.size());
}
