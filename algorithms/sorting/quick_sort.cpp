#include "sorting.h"

static int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i     = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

static void quick_sort_helper(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_helper(arr, low, pi - 1);
        quick_sort_helper(arr, pi + 1, high);
    }
}

void quick_sort(std::vector<int>& arr) {
    if (!arr.empty())
        quick_sort_helper(arr, 0, static_cast<int>(arr.size()) - 1);
}
