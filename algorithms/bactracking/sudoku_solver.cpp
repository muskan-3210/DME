#include "backtracking.h"

static bool is_valid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) return false;
        int br = 3*(row/3)+i/3, bc = 3*(col/3)+i%3;
        if (board[br][bc] == num) return false;
    }
    return true;
}

bool solve_sudoku(std::vector<std::vector<int>>& board) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == 0) {
                for (int num = 1; num <= 9; ++num) {
                    if (is_valid(board, r, c, num)) {
                        board[r][c] = num;
                        if (solve_sudoku(board)) return true;
                        board[r][c] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
