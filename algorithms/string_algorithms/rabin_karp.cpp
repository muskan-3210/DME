#include "string_algo.h"

std::vector<int> rabin_karp_search(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n=(int)text.size(), m=(int)pattern.size();
    if (m>n) return matches;
    const long long base=31, mod=1e9+7;
    long long ph=0, th=0, power=1;
    for (int i=0;i<m-1;++i) power=power*base%mod;
    for (int i=0;i<m;++i) {
        ph=(ph*base+(pattern[i]-'a'+1))%mod;
        th=(th*base+(text[i]-'a'+1))%mod;
    }
    for (int i=0;i<=n-m;++i) {
        if (ph==th && text.substr(i,m)==pattern) matches.push_back(i);
        if (i<n-m) {
            th=(th-(text[i]-'a'+1)*power%mod+mod)%mod;
            th=(th*base+(text[i+m]-'a'+1))%mod;
        }
    }
    return matches;
}
