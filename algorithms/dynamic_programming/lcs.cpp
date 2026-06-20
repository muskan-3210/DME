#include "dp.h"

int lcs(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size()), n = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            dp[i][j] = (a[i-1]==b[j-1]) ? dp[i-1][j-1]+1
                                         : std::max(dp[i-1][j], dp[i][j-1]);
    return dp[m][n];
}
