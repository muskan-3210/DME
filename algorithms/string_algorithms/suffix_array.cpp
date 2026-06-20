#include "string_algo.h"
#include <algorithm>
#include <numeric>

std::vector<int> build_suffix_array(const std::string& s) {
    int n=(int)s.size();
    std::vector<int> sa(n);
    std::iota(sa.begin(),sa.end(),0);
    std::sort(sa.begin(),sa.end(),[&](int a,int b){ return s.substr(a)<s.substr(b); });
    return sa;
}
