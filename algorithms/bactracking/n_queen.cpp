#include "backtracking.h"

static bool is_safe(const std::vector<int>& placement, int row, int col) {
    for (int r = 0; r < row; ++r) {
        if (placement[r] == col) return false;
        if (std::abs(placement[r] - col) == std::abs(r - row)) return false;
    }
    return true;
}

static void solve(int n, int row, std::vector<int>& placement,
                  std::vector<std::vector<int>>& results) {
    if (row == n) { results.push_back(placement); return; }
    for (int col = 0; col < n; ++col) {
        if (is_safe(placement, row, col)) {
            placement[row] = col;
            solve(n, row+1, placement, results);
        }
    }
}

std::vector<std::vector<int>> solve_n_queens(int n) {
    std::vector<std::vector<int>> results;
    std::vector<int> placement(n, -1);
    solve(n, 0, placement, results);
    return results;
}
