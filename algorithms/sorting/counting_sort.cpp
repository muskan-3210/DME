#include "sorting.h"
#include <algorithm>

void counting_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = *std::max_element(arr.begin(), arr.end());
    int min_val = *std::min_element(arr.begin(), arr.end());
    int range   = max_val - min_val + 1;

    std::vector<int> count(range, 0);
    for (int x : arr) ++count[x - min_val];

    int idx = 0;
    for (int i = 0; i < range; ++i)
        while (count[i]-- > 0) arr[idx++] = i + min_val;
}
