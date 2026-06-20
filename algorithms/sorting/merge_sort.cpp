#include "sorting.h"

static void merge(std::vector<int>& arr, int l, int m, int r) {
    std::vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    std::vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < (int)left.size() && j < (int)right.size())
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < (int)left.size())  arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

static void merge_sort_helper(std::vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort_helper(arr, l, m);
    merge_sort_helper(arr, m + 1, r);
    merge(arr, l, m, r);
}

void merge_sort(std::vector<int>& arr) {
    if (!arr.empty())
        merge_sort_helper(arr, 0, static_cast<int>(arr.size()) - 1);
}
