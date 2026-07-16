#include "backtracking.h"
// Provides the solve_sudoku declaration and std::vector; the 9×9 Sudoku board is represented as std::vector<std::vector<int>>.

// (blank line — separates the include from the constraint-validator helper)
static bool is_valid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
// File-private validator: returns true only if placing 'num' at (row, col) violates none of the three Sudoku constraints.
    for (int i = 0; i < 9; ++i) {
    // A single loop of 9 iterations simultaneously checks the entire row, entire column, and the 3×3 box containing (row, col).
        if (board[row][i] == num || board[i][col] == num) return false;
        // Row conflict: 'num' already appears somewhere in row 'row' (board[row][i]).
        // Column conflict: 'num' already appears somewhere in column 'col' (board[i][col]).
        int br = 3*(row/3)+i/3, bc = 3*(col/3)+i%3;
        // Computes the board coordinates of the i-th cell inside the 3×3 sub-grid that contains (row, col).
        // row/3 identifies which band (0/1/2); *3 scales it to the top-left row; i/3 steps through 0, 0, 0, 1, 1, 1, 2, 2, 2.
        // col/3 identifies which stack (0/1/2); *3 scales it to the top-left column; i%3 steps through 0, 1, 2, 0, 1, 2, 0, 1, 2.
        if (board[br][bc] == num) return false;
        // Box conflict: 'num' already appears somewhere in the 3×3 sub-grid that contains (row, col).
    }
    // Closes the for loop; all 9 simultaneous checks passed.
    return true;
    // 'num' can legally be placed at (row, col) — no row, column, or box conflict exists.
}
// Closes is_valid; O(1) time — always exactly 9 iterations regardless of board state.

// (blank line — separates the validator from the recursive solver)
bool solve_sudoku(std::vector<std::vector<int>>& board) {
// Public function declared in backtracking.h; solves the board in-place via recursive backtracking; returns true if a solution exists.
    for (int r = 0; r < 9; ++r) {
    // Scan rows top to bottom, looking for the next unfilled cell.
        for (int c = 0; c < 9; ++c) {
        // Scan columns left to right within each row.
            if (board[r][c] == 0) {
            // 0 is the sentinel for an empty cell — this is the next cell that needs a digit.
                for (int num = 1; num <= 9; ++num) {
                // Try each digit 1 through 9 as a candidate for this empty cell.
                    if (is_valid(board, r, c, num)) {
                    // Only proceed if placing 'num' at (r, c) satisfies all three Sudoku constraints.
                        board[r][c] = num;
                        // Tentatively write 'num' onto the board.
                        if (solve_sudoku(board)) return true;
                        // Recurse: if the remaining board can be completed with this choice, propagate success upward.
                        board[r][c] = 0;
                        // Backtrack: 'num' led to a contradiction deeper in the recursion; erase it and try the next digit.
                    }
                    // Closes the is_valid if; digits that violate a constraint leave the board unchanged.
                }
                // Closes the num loop; all nine digits failed for cell (r, c) — this branch has no solution.
                return false;
                // Signal failure so the caller can backtrack to the previous cell and try a different digit there.
            }
            // Closes the empty-cell if; cells with non-zero values are already filled and are skipped.
        }
        // Closes the column loop for row r; advances to the next row.
    }
    // Closes the row loop; every cell in the 9×9 grid has been visited and is non-zero — the board is completely solved.
    return true;
    // All 81 cells are filled consistently with Sudoku rules; propagate success to the original caller.
}
// Closes solve_sudoku; the solved board is available to the caller through the passed reference — no copy is made.
