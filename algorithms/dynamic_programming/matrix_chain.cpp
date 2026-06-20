#include "dp.h"
#include <climits>

int matrix_chain(const std::vector<int>& dims) {
    int n = static_cast<int>(dims.size()) - 1;
    if (n <= 0) return 0;
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n-len; ++i) {
            int j = i+len-1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = dp[i][k] + dp[k+1][j] + dims[i]*dims[k+1]*dims[j+1];
                if (cost < dp[i][j]) dp[i][j] = cost;
            }
        }
    }
    return dp[0][n-1];
}
