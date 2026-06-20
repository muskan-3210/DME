#include "divide_conquer.h"

static void dc_merge(std::vector<int>& arr, int l, int m, int r) {
    std::vector<int> L(arr.begin()+l, arr.begin()+m+1);
    std::vector<int> R(arr.begin()+m+1, arr.begin()+r+1);
    int i=0, j=0, k=l;
    while (i<(int)L.size() && j<(int)R.size())
        arr[k++] = (L[i]<=R[j]) ? L[i++] : R[j++];
    while (i<(int)L.size()) arr[k++]=L[i++];
    while (j<(int)R.size()) arr[k++]=R[j++];
}

static void dc_helper(std::vector<int>& arr, int l, int r) {
    if (l>=r) return;
    int m = l+(r-l)/2;
    dc_helper(arr,l,m); dc_helper(arr,m+1,r);
    dc_merge(arr,l,m,r);
}

void merge_sort_dc(std::vector<int>& arr) {
    if (!arr.empty()) dc_helper(arr, 0, (int)arr.size()-1);
}

static int max_cross(const std::vector<int>& arr, int l, int m, int r) {
    int ls=INT_MIN, s=0;
    for (int i=m; i>=l; --i) { s+=arr[i]; ls=std::max(ls,s); }
    int rs=INT_MIN; s=0;
    for (int i=m+1; i<=r; ++i) { s+=arr[i]; rs=std::max(rs,s); }
    return ls+rs;
}

static int mss(const std::vector<int>& arr, int l, int r) {
    if (l==r) return arr[l];
    int m=l+(r-l)/2;
    return std::max({mss(arr,l,m), mss(arr,m+1,r), max_cross(arr,l,m,r)});
}

int max_subarray_sum(const std::vector<int>& arr) {
    if (arr.empty()) return 0;
    return mss(arr, 0, (int)arr.size()-1);
}
