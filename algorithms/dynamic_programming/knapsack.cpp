#include "dp.h"

int knapsack(int W, const std::vector<int>& weights, const std::vector<int>& values) {
    int n = static_cast<int>(weights.size());
    std::vector<std::vector<int>> dp(n+1, std::vector<int>(W+1, 0));
    for (int i = 1; i <= n; ++i)
        for (int w = 0; w <= W; ++w) {
            dp[i][w] = dp[i-1][w];
            if (weights[i-1] <= w)
                dp[i][w] = std::max(dp[i][w], dp[i-1][w-weights[i-1]] + values[i-1]);
        }
    return dp[n][W];
}
