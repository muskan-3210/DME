#include "string_algo.h"

static std::vector<int> build_lps(const std::string& p) {
    int n = static_cast<int>(p.size());
    std::vector<int> lps(n, 0);
    int len = 0, i = 1;
    while (i < n) {
        if (p[i] == p[len]) { lps[i++] = ++len; }
        else if (len) { len = lps[len-1]; }
        else { lps[i++] = 0; }
    }
    return lps;
}

std::vector<int> kmp_search(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    if (pattern.empty()) return matches;
    auto lps = build_lps(pattern);
    int i=0, j=0;
    int n=(int)text.size(), m=(int)pattern.size();
    while (i < n) {
        if (text[i]==pattern[j]) { ++i; ++j; }
        if (j==m) { matches.push_back(i-j); j=lps[j-1]; }
        else if (i<n && text[i]!=pattern[j]) {
            if (j) j=lps[j-1]; else ++i;
        }
    }
    return matches;
}
