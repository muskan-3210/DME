#include "searching.h"

int binary_search_idx(const std::vector<int>& arr, int target) {
    int lo = 0, hi = static_cast<int>(arr.size()) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target)  lo = mid + 1;
        else                    hi = mid - 1;
    }
    return -1;
}
