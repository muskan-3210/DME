#include "searching.h"

int exponential_search(const std::vector<int>& arr, int target) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return -1;
    if (arr[0] == target) return 0;
    int i = 1;
    while (i < n && arr[i] <= target) i *= 2;
    int lo = i / 2, hi = std::min(i, n - 1);
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target)  lo = mid + 1;
        else                    hi = mid - 1;
    }
    return -1;
}
