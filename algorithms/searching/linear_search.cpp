#include "searching.h"

int linear_search(const std::vector<int>& arr, int target) {
    for (int i = 0; i < (int)arr.size(); ++i)
        if (arr[i] == target) return i;
    return -1;
}
