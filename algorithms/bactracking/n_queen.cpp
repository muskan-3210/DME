#include "backtracking.h"
// Provides declarations for solve_n_queens and solve_sudoku, and imports std::vector; std::abs used below requires <cstdlib> or <cmath> pulled in transitively.

// (blank line — separates the include from the first helper function)
static bool is_safe(const std::vector<int>& placement, int row, int col) {
// File-private validator: checks whether placing a queen at (row, col) conflicts with any queen already placed in rows 0 through row-1.
    for (int r = 0; r < row; ++r) {
    // Iterate over every previously filled row; placement[r] is the column where the queen in row r sits.
        if (placement[r] == col) return false;
        // Column conflict: another queen already occupies column 'col' — two queens in the same column attack each other.
        if (std::abs(placement[r] - col) == std::abs(r - row)) return false;
        // Diagonal conflict: if the horizontal distance equals the vertical distance, the two queens share a diagonal and attack each other.
    }
    // Closes the for loop; no conflict found with any previously placed queen.
    return true;
    // No column or diagonal conflict: (row, col) is a safe position for the next queen.
}
// Closes is_safe; O(row) per call — checks all previously placed queens; called for every column candidate inside solve.

// (blank line — visual separator between helper functions)
static void solve(int n, int row, std::vector<int>& placement,
// Recursive backtracking engine; 'n' is the board size, 'row' is the row currently being filled (0-indexed).
                  std::vector<std::vector<int>>& results) {
// 'placement' maps row index → chosen column (modified in-place during recursion); 'results' accumulates all complete valid solutions.
    if (row == n) { results.push_back(placement); return; }
    // Base case: all n rows have been assigned a queen — 'placement' is a complete valid solution; save it and return.
    for (int col = 0; col < n; ++col) {
    // Try every column position (0 to n-1) as a candidate for the queen in 'row'.
        if (is_safe(placement, row, col)) {
        // Only descend into this branch if placing at (row, col) does not attack any existing queen.
            placement[row] = col;
            // Tentatively assign column 'col' to this row in the working placement array.
            solve(n, row+1, placement, results);
            // Recurse to the next row; if this path yields no solutions, the recursion simply returns here.
        }
        // Implicit backtrack: the for loop advances to the next column, which will overwrite placement[row] on the next valid trial.
        // No explicit undo is needed because placement[row] is always set before recursing, never left as a committed value.
    }
    // Closes the for loop; all n column candidates for this row have been tried and either accepted or rejected.
}
// Closes solve; backtracking happens naturally — placement[row] is overwritten on the next iteration, undoing the previous assignment.

// (blank line — separates the recursive engine from the public entry point)
std::vector<std::vector<int>> solve_n_queens(int n) {
// Public entry point declared in backtracking.h; initialises state and triggers the recursive search; returns all valid solutions.
    std::vector<std::vector<int>> results;
    // Will collect every valid queen configuration; each inner vector holds n column indices, one per row.
    std::vector<int> placement(n, -1);
    // Working array of size n; placement[r] = column of queen in row r; initialised to -1 meaning no queen placed yet.
    solve(n, 0, placement, results);
    // Launch the recursive backtracker starting from row 0 with an empty placement; populates 'results' in-place.
    return results;
    // Return all found solutions; for n=8 this yields 92 distinct configurations.
}
// Closes solve_n_queens; 'placement' is destroyed here; 'results' is returned via NRVO (copy elision) or move semantics.
