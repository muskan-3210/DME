#include "sorting.h"
#include <algorithm>

static void counting_pass(std::vector<int>& arr, int exp) {
    int n = static_cast<int>(arr.size());
    std::vector<int> output(n);
    std::vector<int> count(10, 0);
    for (int x : arr) ++count[(x / exp) % 10];
    for (int i = 1; i < 10; ++i) count[i] += count[i-1];
    for (int i = n - 1; i >= 0; --i) {
        int d = (arr[i] / exp) % 10;
        output[--count[d]] = arr[i];
    }
    arr = output;
}

void radix_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = *std::max_element(arr.begin(), arr.end());
    for (int exp = 1; max_val / exp > 0; exp *= 10)
        counting_pass(arr, exp);
}
