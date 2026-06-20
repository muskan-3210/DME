#include "string_algo.h"

std::vector<int> z_algorithm(const std::string& s) {
    int n=(int)s.size();
    std::vector<int> z(n,0);
    z[0]=n;
    int l=0,r=0;
    for (int i=1;i<n;++i) {
        if (i<r) z[i]=std::min(r-i,z[i-l]);
        while (i+z[i]<n && s[z[i]]==s[i+z[i]]) ++z[i];
        if (i+z[i]>r) { l=i; r=i+z[i]; }
    }
    return z;
}
