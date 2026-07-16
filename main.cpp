#include <iostream>
// Provides std::cout, used throughout every demo function for console output.

#include <vector>
// Provides std::vector, used in sorting, searching, graph, and simulation demos.

#include <string>
// Provides std::string, used for labels, keys, and demo data throughout.

#include <iomanip>
// Provides std::setw, std::left, std::fixed, std::setprecision for formatted table output in demos.

// --- Config ---
#include "config/constants.h"
// Project-wide compile-time constants (e.g., default capacities, magic numbers).

#include "config/system_config.h"
// Runtime configuration struct or singleton for the DSA Master Engine.

// --- Core ---
#include "core/logger/logger.h"
// DME::Logger singleton; set to WARN in main() to suppress INFO-level noise during the demo.

#include "core/memory_manager/lru_cache.h"
// DME::LRUCache<K,V>: doubly-linked-list + hash-map cache that evicts the least-recently-used entry.

#include "core/memory_manager/lfu_cache.h"
// DME::LFUCache<K,V>: evicts the least-frequently-used entry; demonstrated in demo_lfu_cache().

#include "core/rate_limiter/token_bucket.h"
// DME::TokenBucket: refills tokens at a fixed rate; consume() returns false when the bucket is empty.

#include "core/load_balancer/server.h"
// DME::Server struct: holds id, address, port, and active-connection count for load balancer demos.

#include "core/load_balancer/round_robin.h"
// DME::RoundRobinBalancer: cycles through servers in order; demonstrated in demo_load_balancer().

#include "core/load_balancer/least_connection.h"
// DME::LeastConnectionBalancer: picks the server with the fewest active connections.

#include "core/schedular/task.h"
// DME::Task: wraps a callable with an id, description, and priority for the scheduler.

#include "core/schedular/task_schedular.h"
// DME::TaskScheduler: priority-queue-based dispatcher; run_all() executes tasks highest-priority first.

// --- Data Structures ---
#include "data_structures/array/dynamic_array.h"
// DynamicArray<T>: resizable array (like std::vector) implemented from scratch with manual reallocation.

#include "data_structures/linked_list/singly_ll.h"
// SinglyLinkedList<T>: each node has one next pointer; supports insert/delete/search.

#include "data_structures/linked_list/doubly_ll.h"
// DoublyLinkedList<T>: each node has next and prev pointers; enables O(1) removal given a pointer.

#include "data_structures/linked_list/circular_ll.h"
// CircularLinkedList<T>: tail's next points back to head; useful for round-robin structures.

#include "data_structures/stack/stack_array.h"
// Stack<T>: LIFO structure backed by a dynamic array; push/pop/top are all O(1).

#include "data_structures/queue/simple_queue.h"
// SimpleQueue<T>: FIFO queue backed by a linked list; enqueue/dequeue are O(1).

#include "data_structures/queue/circular_queue.h"
// CircularQueue<T>: fixed-capacity ring buffer; wraps around to reuse space without shifting.

#include "data_structures/queue/deque.h"
// Deque<T>: double-ended queue; push/pop at both front and back are O(1).

#include "data_structures/hashmap/hashmap_chaining.h"
// HashMapChaining<K,V>: collision resolution via separate chaining (linked lists per bucket).

#include "data_structures/hashmap/hashmap_open_addressing.h"
// HashMapOpenAddressing<K,V>: collision resolution by probing; all entries stored in one flat array.

#include "data_structures/tree/bst.h"
// BST<T>: unbalanced binary search tree; O(log n) average, O(n) worst-case for insert/search/remove.

#include "data_structures/tree/avl_tree.h"
// AVLTree<T>: self-balancing BST using height-based rotations; guarantees O(log n) all operations.

#include "data_structures/tree/heap.h"
// MaxHeap<T> and MinHeap<T>: binary heap implementations; top() is O(1), push/pop are O(log n).

#include "data_structures/tree/segment_tree.h"
// SegmentTree<T>: range query tree with a user-supplied merge function; query/update are O(log n).

#include "data_structures/tree/fenwick_tree.h"
// FenwickTree: binary indexed tree for prefix-sum queries; more space-efficient than segment tree.

#include "data_structures/trie/trie.h"
// Trie: prefix tree for string storage; supports search, prefix check, and autocomplete.

#include "data_structures/graph/graph.h"
// Graph: adjacency-list weighted directed/undirected graph used by graph algorithm demos.

#include "data_structures/graph/disjoint_set.h"
// DisjointSet: union-find with path compression and union-by-rank; used for Kruskal's MST.

// --- Algorithms ---
#include "algorithms/sorting/sorting.h"
// Declares bubble_sort, insertion_sort, selection_sort, merge_sort, quick_sort, heap_sort, counting_sort, radix_sort.

#include "algorithms/searching/searching.h"
// Declares linear_search, binary_search_idx, exponential_search operating on std::vector<int>.

#include "algorithms/greedy/greedy.h"
// Declares activity_selection (interval scheduling) and huffman_coding (prefix-free encoding).

#include "algorithms/divide_conquer/divide_conquer.h"
// Declares divide-and-conquer algorithms (merge sort variant, binary search, etc.).

#include "algorithms/bactracking/backtracking.h"
// Declares solve_n_queens and solve_sudoku; note the intentional typo in the directory name "bactracking".

#include "algorithms/dynamic_programming/dp.h"
// Declares knapsack, lcs, lis, matrix_chain — classical DP problems with memoized/tabulated solutions.

#include "algorithms/string_algorithms/string_algo.h"
// Declares kmp_search, rabin_karp_search, z_algorithm for pattern matching in strings.

#include "algorithms/graph_algorithms/graph_algo.h"
// Declares bfs, dfs, dijkstra, kruskal, topological_sort operating on the Graph class.

// --- Simulations ---
#include "simulations/search_engine/search_engine.h"
// DME::SearchEngine: inverted-index full-text search engine defined in simulations/search_engine/.

#include "simulations/job_recommender/job_matcher.h"
// DME::Job, DME::Candidate, DME::JobMatcher: skill-based job scoring engine.

#include "simulations/job_recommender/recommendation_engine.h"
// DME::RecommendationEngine: wraps JobMatcher with pre-loaded sample jobs and a top-N interface.

#include "simulations/traffic_router/traffic_router.h"
// DME::TrafficRouter: Dijkstra-based minimum-latency network router simulation.

#include "simulations/analytics/sliding_window_metrics.h"
// DME::SlidingWindowMetrics: time-bounded rolling statistics (average, max, min) over a deque.

// ============================================================
static void print_header(const std::string& title) {
// Static helper: prints a prominent boxed section header to stdout; static limits visibility to this file.
    std::cout << "\n";
    // Blank line before the header box for visual separation.
    std::cout << "========================================\n";
    // Top border of the header box.
    std::cout << "  " << title << "\n";
    // The section title indented two spaces inside the box.
    std::cout << "========================================\n";
    // Bottom border of the header box.
}
// End of print_header().

static void print_section(const std::string& name) {
// Static helper: prints a lighter sub-section divider; used before each individual demo function's output.
    std::cout << "\n--- " << name << " ---\n";
    // Dashes flank the name; \n before and after create visual breathing room between demos.
}
// End of print_section().

static void print_vec(const std::string& label, const std::vector<int>& v) {
// Static helper: prints a labeled integer vector in "[ x y z ]" format; currently unused but available.
    std::cout << label << ": [ ";
    // Print the label followed by the opening bracket.
    for (int x : v) std::cout << x << " ";
    // Print each element followed by a space; range-based for iterates v by value.
    std::cout << "]\n";
    // Closing bracket and newline.
}
// End of print_vec().

// ============================================================
//  DEMO: Data Structures
// ============================================================

void demo_dynamic_array() {
// Demonstrates DynamicArray<int>: push_back, size, capacity, index access, and pop_back.
    print_section("DynamicArray<int>");
    // Print sub-header so the user knows which data structure is being demonstrated.
    DynamicArray<int> arr;
    // Default-constructs an empty dynamic array; internal buffer starts small and grows on demand.
    for (int i = 1; i <= 6; ++i) arr.push_back(i * 10);
    // Inserts 10, 20, 30, 40, 50, 60; the array may reallocate (typically doubles capacity) during growth.
    std::cout << "After 6 push_backs — size=" << arr.size()
              << " capacity=" << arr.capacity() << "\n";
    // Shows that size (used slots) and capacity (allocated slots) can differ due to doubling strategy.
    std::cout << "arr[2] = " << arr[2] << " (O(1) access)\n";
    // Demonstrates random-access via operator[]; arr[2] should be 30 (0-indexed).
    arr.pop_back();
    // Removes the last element (60); size decreases by 1 but capacity typically stays the same.
    std::cout << "After pop_back  — size=" << arr.size() << "\n";
    // Confirms size is now 5.
}
// End of demo_dynamic_array().

void demo_singly_ll() {
// Demonstrates SinglyLinkedList<int>: insert at tail/head, size, search, and delete by value.
    print_section("SinglyLinkedList<int>");
    // Print sub-header.
    SinglyLinkedList<int> list;
    // Default-constructs an empty singly linked list with head = nullptr.
    list.insert_tail(10);
    // Appends 10 at the end; O(n) if tail pointer not stored, O(1) if it is.
    list.insert_tail(20);
    // List is now: 10 -> 20.
    list.insert_tail(30);
    // List is now: 10 -> 20 -> 30.
    list.insert_head(5);
    // Prepends 5; O(1) operation — new node points to old head. List: 5 -> 10 -> 20 -> 30.
    std::cout << "List size=" << list.size() << "\n";
    // size() traverses the list counting nodes; should output 4.
    std::cout << "Search 20: " << (list.search(20) ? "found" : "not found") << "\n";
    // search() does a linear scan; O(n). Should print "found".
    list.delete_by_value(20);
    // Finds and unlinks the node containing 20; O(n) scan to locate the predecessor node.
    std::cout << "After delete_by_value(20) size=" << list.size() << "\n";
    // Should now be 3.
    std::cout << "Search 20: " << (list.search(20) ? "found" : "not found") << "\n";
    // Confirms 20 is gone; should print "not found".
}
// End of demo_singly_ll().

void demo_stack() {
// Demonstrates Stack<int>: push, top, size, and pop operations.
    print_section("Stack<int>");
    // Print sub-header.
    Stack<int> stk;
    // Default-constructs an empty stack backed by a DynamicArray (or similar).
    stk.push(1); stk.push(2); stk.push(3);
    // Pushes three integers; internal array grows if needed. Stack state (top to bottom): 3, 2, 1.
    std::cout << "Top=" << stk.top() << " size=" << stk.size() << "\n";
    // top() returns the most recently pushed element (3) without removing it.
    stk.pop();
    // Removes the top element (3); O(1) operation.
    std::cout << "After pop: Top=" << stk.top() << "\n";
    // top() should now return 2.
}
// End of demo_stack().

void demo_queue() {
// Demonstrates SimpleQueue, CircularQueue, and Deque in one function.
    print_section("SimpleQueue + CircularQueue + Deque");
    // Print sub-header combining all three queue variants.
    SimpleQueue<int> q;
    // Default-constructs an empty FIFO queue backed by a linked list.
    q.enqueue(10); q.enqueue(20); q.enqueue(30);
    // Enqueues 10, 20, 30 in order; 10 is at the front (FIFO).
    std::cout << "SimpleQueue front=" << q.front() << " size=" << q.size() << "\n";
    // front() peeks at the oldest element (10); size should be 3.
    q.dequeue();
    // Removes the front element (10); O(1) for a linked-list queue.
    std::cout << "After dequeue: front=" << q.front() << "\n";
    // front() should now return 20.

    CircularQueue<int> cq(5);
    // Constructs a ring buffer with capacity 5; wraps around when full.
    cq.enqueue(1); cq.enqueue(2); cq.enqueue(3);
    // Fills the first three slots of the ring buffer.
    std::cout << "CircularQueue front=" << cq.front() << " size=" << cq.size() << "\n";
    // front() returns 1; size should be 3.

    Deque<int> dq;
    // Default-constructs an empty double-ended queue.
    dq.push_back(10); dq.push_front(5); dq.push_back(20);
    // Sequence of operations: push_back(10) → [10], push_front(5) → [5,10], push_back(20) → [5,10,20].
    std::cout << "Deque front=" << dq.front() << " back=" << dq.back() << " size=" << dq.size() << "\n";
    // front() = 5, back() = 20, size = 3.
}
// End of demo_queue().

void demo_hashmap() {
// Demonstrates both HashMap implementations: chaining (linked lists) and open addressing (probing).
    print_section("HashMap (Chaining + Open Addressing)");
    // Print sub-header.
    HashMapChaining<std::string, int> hmc;
    // Constructs a chaining hash map with default bucket count; buckets hold linked lists of (key,value) pairs.
    hmc.insert("alice", 95);
    // Hashes "alice", finds its bucket, appends the pair to the chain.
    hmc.insert("bob",   82);
    // Hashes "bob" and inserts similarly.
    hmc.insert("carol", 91);
    // Hashes "carol" and inserts similarly.
    int score = 0;
    // Output variable for get(); passed by reference so get() can fill it.
    std::cout << "Chaining: alice=" << (hmc.get("alice", score) ? std::to_string(score) : "?") << "\n";
    // get() returns true and sets score=95 if found; ternary selects the value or "?" on miss.
    hmc.remove("bob");
    // Locates "bob"'s bucket and unlinks its node from the chain.
    std::cout << "After remove(bob): contains bob=" << hmc.contains("bob") << "\n";
    // contains() should return false (0) after removal.

    HashMapOpenAddressing<std::string, int> hmo;
    // Constructs an open-addressing hash map; all entries share one flat array with probe sequences.
    hmo.insert("x", 100); hmo.insert("y", 200);
    // Inserts two entries; if a collision occurs, linear/quadratic probing finds the next empty slot.
    std::cout << "OpenAddr: x=" << (hmo.get("x", score) ? std::to_string(score) : "?") << "\n";
    // get() should return true and set score=100 for key "x".
}
// End of demo_hashmap().

void demo_bst() {
// Demonstrates BST<int> and AVLTree<int>: insert, search, and remove.
    print_section("BST + AVLTree");
    // Print sub-header.
    BST<int> bst;
    // Default-constructs an empty binary search tree with root = nullptr.
    for (int x : {5, 3, 7, 1, 4, 6, 8}) bst.insert(x);
    // Initializer-list range-for: inserts 7 values; BST property: left < parent < right.
    std::cout << "BST: search(4)=" << bst.search(4) << " search(9)=" << bst.search(9) << "\n";
    // search() walks left/right based on comparisons; 4 is present (1), 9 is not (0).
    bst.remove(4);
    // Removes 4; if it has two children, replaces it with the in-order successor.
    std::cout << "After remove(4): search(4)=" << bst.search(4) << "\n";
    // Should return 0 (false).

    AVLTree<int> avl;
    // Default-constructs an empty AVL tree; rotations will keep it balanced.
    for (int x : {10, 20, 30, 40, 50}) avl.insert(x);  // would be right-heavy without rotations
    // Inserting in ascending order would create a right-skewed BST; AVL applies left-rotations to rebalance.
    std::cout << "AVL size=" << avl.size() << " search(30)=" << avl.search(30) << "\n";
    // size() = 5, search(30) = 1; AVL guarantees O(log n) even after skewed insertions.
}
// End of demo_bst().

void demo_heap() {
// Demonstrates MaxHeap and MinHeap: push elements and verify the top invariant.
    print_section("MaxHeap + MinHeap");
    // Print sub-header.
    MaxHeap<int> maxh;
    // Constructs an empty max-heap backed by a vector; the largest element is always at top.
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) maxh.push(x);
    // Inserts 8 values using sift-up; heap property is maintained after each insertion.
    std::cout << "MaxHeap top=" << maxh.top() << " (should be 9)\n";
    // top() reads the root of the heap without removing it; should be 9 (the maximum).

    MinHeap<int> minh;
    // Constructs an empty min-heap; the smallest element is always at the root.
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) minh.push(x);
    // Same values as above; min-heap arranges them so the smallest is at the root.
    std::cout << "MinHeap top=" << minh.top() << " (should be 1)\n";
    // top() should return 1 (the minimum value in the set).
}
// End of demo_heap().

void demo_segment_fenwick() {
// Demonstrates SegmentTree (generic range queries) and FenwickTree (prefix sums).
    print_section("SegmentTree + FenwickTree");
    // Print sub-header.
    std::vector<int> data = {1, 3, 5, 7, 9, 11};
    // Input data array shared by both tree types; indices 0–5.
    SegmentTree<int> seg(data, [](int a, int b){ return a + b; }, 0);
    // Builds a sum segment tree from data; the lambda is the merge function; 0 is the identity element.
    std::cout << "SegTree sum[1..4]=" << seg.query(1, 4) << " (expect 24)\n";
    // Range sum [1..4] = 3+5+7+9 = 24; query() traverses the tree in O(log n).
    seg.update(2, 10);
    // Point update: sets index 2 to 10 (was 5); propagates change up the tree in O(log n).
    std::cout << "After update(2,10) sum[1..4]=" << seg.query(1, 4) << " (expect 29)\n";
    // New sum [1..4] = 3+10+7+9 = 29.

    FenwickTree fen(6);
    // Constructs a Fenwick tree for 6 elements; internal array indexed 1..6 (1-based internally).
    for (int i = 0; i < 6; ++i) fen.update(i, data[i]);
    // Loads all data values into the Fenwick tree via point updates; uses original data (not updated seg).
    std::cout << "Fenwick prefix_sum[0..4]=" << fen.prefix_sum(4) << " (expect 25)\n";
    // prefix_sum(4) = sum of data[0..4] = 1+3+5+7+9 = 25; O(log n) via bit manipulation.
    std::cout << "Fenwick range_sum[2..4]=" << fen.range_sum(2, 4) << " (expect 21)\n";
    // range_sum(2,4) = prefix_sum(4) - prefix_sum(1) = 25 - 4 = 21 (data[2]+data[3]+data[4]=5+7+9).
}
// End of demo_segment_fenwick().

void demo_trie() {
// Demonstrates Trie: insert, search, prefix check, and autocomplete.
    print_section("Trie");
    // Print sub-header.
    Trie trie;
    // Default-constructs an empty trie with a root node having 26 child pointers (one per letter).
    trie.insert("apple"); trie.insert("app"); trie.insert("application");
    // Inserts three words sharing the "app" prefix; each shares nodes up to the common prefix.
    trie.insert("banana"); trie.insert("band");
    // Inserts two more words with a different prefix "ban"; these branch off from a separate subtree.
    std::cout << "search('app')=" << trie.search("app") << "\n";
    // search() checks for end-of-word marker at the node for 'p'; "app" was inserted, so returns 1.
    std::cout << "search('apt')=" << trie.search("apt") << "\n";
    // "apt" was never inserted; returns 0 (false).
    std::cout << "starts_with('app')=" << trie.starts_with("app") << "\n";
    // starts_with() only checks that the prefix path exists, not the end-of-word flag; returns 1.
    auto completions = trie.autocomplete("app");
    // DFS from the "app" prefix node collecting all words; returns {"app","apple","application"} or similar.
    std::cout << "autocomplete('app'): ";
    // Label for the autocomplete output.
    for (const auto& w : completions) std::cout << w << " ";
    // Print each completion word separated by a space.
    std::cout << "\n";
    // Newline after the completion list.
}
// End of demo_trie().

void demo_graph_ds() {
// Demonstrates the Graph adjacency-list structure and DisjointSet union-find.
    print_section("Graph + DisjointSet");
    // Print sub-header.
    Graph g(5);
    // Constructs a 5-node graph (default: undirected, unweighted for this call).
    g.add_edge(0,1); g.add_edge(1,2); g.add_edge(2,3); g.add_edge(3,4);
    // Adds a chain of edges: 0-1-2-3-4; forms a path graph.
    std::cout << "Graph: " << g.vertex_count() << " vertices\n";
    // vertex_count() should return 5.

    DisjointSet ds(5);
    // Constructs a union-find with 5 singleton sets: {0},{1},{2},{3},{4}.
    ds.unite(0,1); ds.unite(2,3);
    // Merges sets: {0,1},{2,3},{4}; components() should now return 3.
    std::cout << "DisjointSet: components=" << ds.components() << " (expect 3)\n";
    // components() counts distinct roots after union operations.
    std::cout << "connected(0,1)=" << ds.connected(0,1) << " connected(0,2)=" << ds.connected(0,2) << "\n";
    // connected() uses find() with path compression; 0 and 1 share a root (1), 0 and 2 do not.
}
// End of demo_graph_ds().

// ============================================================
//  DEMO: Algorithms
// ============================================================

void demo_sorting() {
// Demonstrates all 8 sorting algorithms on the same input array and prints their outputs.
    print_section("Sorting Algorithms");
    // Print sub-header.
    std::vector<int> original = {64, 34, 25, 12, 22, 11, 90};
    // The unsorted input; each algorithm sorts a fresh copy, so original is never mutated.

    auto run = [&](const std::string& name, std::function<void(std::vector<int>&)> fn) {
    // Lambda capturing original by reference; takes an algorithm name and a sorting function.
        auto arr = original;
        // Copy original for this run; each algorithm sees the same unsorted data.
        fn(arr);
        // Call the sorting function in-place on the copy.
        std::cout << std::setw(18) << std::left << name << ": [ ";
        // Print the name left-aligned in a 18-char field for column alignment.
        for (int x : arr) std::cout << x << " ";
        // Print each sorted element.
        std::cout << "]\n";
        // Close the bracket and newline.
    };
    // End of run lambda.

    run("bubble_sort",    bubble_sort);
    // O(n²) comparison sort; repeatedly swaps adjacent elements until sorted.
    run("insertion_sort", insertion_sort);
    // O(n²) avg; builds a sorted prefix one element at a time; fast on nearly-sorted data.
    run("selection_sort", selection_sort);
    // O(n²) always; finds the minimum of the unsorted suffix and swaps it into place.
    run("merge_sort",     merge_sort);
    // O(n log n) stable sort via divide-and-conquer; requires O(n) extra space for merging.
    run("quick_sort",     quick_sort);
    // O(n log n) avg, O(n²) worst; in-place partition-based sort; cache-friendly in practice.
    run("heap_sort",      heap_sort);
    // O(n log n) always, in-place; builds a max-heap then extracts elements one by one.
    run("counting_sort",  counting_sort);
    // O(n + k) where k = value range; not comparison-based; suitable only for small integer ranges.
    run("radix_sort",     radix_sort);
    // O(d*(n+k)) where d = digit count; sorts by digit from least significant to most significant.
}
// End of demo_sorting().

void demo_searching() {
// Demonstrates linear search, binary search, and exponential search on a sorted array.
    print_section("Searching Algorithms");
    // Print sub-header.
    std::vector<int> sorted = {5, 10, 15, 20, 25, 30, 35, 40};
    // Pre-sorted input array; binary and exponential search require sorted input.
    std::cout << "Array: [ "; for (int x : sorted) std::cout << x << " "; std::cout << "]\n";
    // Prints the array contents for reference.
    std::cout << "linear_search(25)     = index " << linear_search(sorted, 25)      << "\n";
    // O(n): scans left to right; 25 is at index 4.
    std::cout << "binary_search(25)     = index " << binary_search_idx(sorted, 25)  << "\n";
    // O(log n): halves the search range each step; 25 is at index 4.
    std::cout << "exponential_search(40)= index " << exponential_search(sorted, 40) << "\n";
    // O(log n): doubles the bound to find a range, then binary searches within it; 40 is at index 7.
    std::cout << "linear_search(99)     = index " << linear_search(sorted, 99)      << " (not found)\n";
    // Returns -1 (or similar sentinel) when the element is absent.
}
// End of demo_searching().

void demo_greedy() {
// Demonstrates activity selection (interval scheduling) and Huffman coding.
    print_section("Greedy Algorithms");
    // Print sub-header.
    std::vector<Activity> acts = {{0,6,1},{1,4,2},{3,5,3},{3,9,4},{5,9,5},{5,7,6},{8,11,7},{8,12,8},{2,14,9},{12,16,10}};
    // 10 activities each with {start, end, id}; the greedy algorithm picks a maximum non-overlapping subset.
    auto selected = activity_selection(acts);
    // Sorts by end time, then greedily picks each activity whose start >= the last selected activity's end.
    std::cout << "Activity Selection — selected " << selected.size() << " activities\n";
    // Prints how many non-overlapping activities were selected.
    for (const auto& a : selected)
    // Iterate the selected activities in order.
        std::cout << "  Activity " << a.id << " [" << a.start << "," << a.end << "]\n";
        // Print each selected activity's ID and time interval.

    std::vector<std::pair<char,int>> freq = {{'a',45},{'b',13},{'c',12},{'d',16},{'e',9},{'f',5}};
    // Character frequency table; Huffman coding assigns shorter codes to higher-frequency characters.
    auto codes = huffman_coding(freq);
    // Builds a min-heap of leaf nodes, merges them bottom-up into a Huffman tree, then reads off codes.
    std::cout << "Huffman codes:\n";
    // Header for the code table output.
    for (const auto& c : codes)
    // Iterate each character's code assignment.
        std::cout << "  '" << c.ch << "' freq=" << c.freq << " code=" << c.code << "\n";
        // Print character, its frequency, and its binary prefix-free code.
}
// End of demo_greedy().

void demo_dp() {
// Demonstrates four classical dynamic programming problems.
    print_section("Dynamic Programming");
    // Print sub-header.
    std::vector<int> weights = {2, 3, 4, 5};
    // Item weights for the 0/1 knapsack; each item can be taken at most once.
    std::vector<int> values  = {3, 4, 5, 6};
    // Corresponding item values; goal is to maximize value subject to weight constraint.
    std::cout << "0/1 Knapsack (W=8): " << knapsack(8, weights, values) << " (expect 10)\n";
    // knapsack() fills a 2D DP table (items × capacities); answer is dp[n][W] = 10.

    std::cout << "LCS('ABCBDAB','BDCAB')=" << lcs("ABCBDAB", "BDCAB") << " (expect 4)\n";
    // Longest Common Subsequence; DP table is len(s1)+1 × len(s2)+1; answer = 4 (e.g., "BCAB").

    std::vector<int> seq = {10, 9, 2, 5, 3, 7, 101, 18};
    // Input sequence for Longest Increasing Subsequence.
    std::cout << "LIS of {10,9,2,5,3,7,101,18}=" << lis(seq) << " (expect 4)\n";
    // LIS length = 4; one valid LIS is {2, 3, 7, 101} or {2, 5, 7, 101}.

    std::vector<int> dims = {40, 20, 30, 10, 30};
    // Dimension chain: matrices are 40×20, 20×30, 30×10, 10×30 (4 matrices from 5 dimensions).
    std::cout << "Matrix Chain {40x20,20x30,30x10,10x30}=" << matrix_chain(dims) << " (expect 26000)\n";
    // Minimum scalar multiplications to compute the product of all matrices = 26000.
}
// End of demo_dp().

void demo_backtracking() {
// Demonstrates N-Queens and Sudoku Solver using recursive backtracking.
    print_section("Backtracking");
    // Print sub-header.
    auto solutions = solve_n_queens(4);
    // Places 4 queens on a 4×4 board such that no two share a row, column, or diagonal.
    std::cout << "4-Queens solutions: " << solutions.size() << " (expect 2)\n";
    // There are exactly 2 solutions for N=4.
    for (const auto& sol : solutions) {
    // Iterate each solution; each sol is a vector where sol[row] = column of the queen in that row.
        std::cout << "  [ ";
        // Opening bracket for this solution.
        for (int c : sol) std::cout << "Q" << c << " ";
        // Print "QC" where C is the column index of the queen in each row.
        std::cout << "]\n";
        // Closing bracket and newline.
    }
    // End of solutions loop.

    std::vector<std::vector<int>> board = {
    // 9×9 Sudoku board with 0 representing empty cells.
        {5,3,0,0,7,0,0,0,0},
        // Row 0: 5 and 3 are given; zeros are blanks to fill.
        {6,0,0,1,9,5,0,0,0},
        // Row 1.
        {0,9,8,0,0,0,0,6,0},
        // Row 2.
        {8,0,0,0,6,0,0,0,3},
        // Row 3.
        {4,0,0,8,0,3,0,0,1},
        // Row 4.
        {7,0,0,0,2,0,0,0,6},
        // Row 5.
        {0,6,0,0,0,0,2,8,0},
        // Row 6.
        {0,0,0,4,1,9,0,0,5},
        // Row 7.
        {0,0,0,0,8,0,0,7,9}
        // Row 8.
    };
    // End of board initialization.
    bool solved = solve_sudoku(board);
    // Attempts to fill all zeros satisfying row/column/3×3-box uniqueness via backtracking.
    std::cout << "Sudoku solved: " << (solved ? "YES" : "NO") << "\n";
    // Prints whether a solution was found (should be YES for this classic puzzle).
    if (solved) std::cout << "  First row: ";
    // Only print the first row if the puzzle was actually solved.
    for (int x : board[0]) std::cout << x << " ";
    // Prints each cell of the (now-filled) first row.
    std::cout << "\n";
    // Newline after the first row output.
}
// End of demo_backtracking().

void demo_string_algos() {
// Demonstrates KMP, Rabin-Karp, and Z-algorithm for substring/pattern matching.
    print_section("String Algorithms");
    // Print sub-header.
    std::string text    = "aabaacaadaabaaba";
    // The text to search within; 16 characters with repeating "aab" patterns.
    std::string pattern = "aaba";
    // The 4-character pattern to locate within text.
    auto kmp_matches = kmp_search(text, pattern);
    // KMP preprocesses the pattern into a failure function in O(m), then scans text in O(n).
    std::cout << "KMP '" << pattern << "' in '" << text << "': matches at [ ";
    // Header for KMP match positions.
    for (int idx : kmp_matches) std::cout << idx << " ";
    // Print each 0-based starting index where the pattern was found.
    std::cout << "]\n";
    // Close the bracket.

    auto rk_matches = rabin_karp_search(text, pattern);
    // Rabin-Karp uses a rolling hash to check candidate windows in O(1) amortized; O(nm) worst case.
    std::cout << "Rabin-Karp: matches at [ ";
    // Header for Rabin-Karp match positions.
    for (int idx : rk_matches) std::cout << idx << " ";
    // Same expected positions as KMP.
    std::cout << "]\n";
    // Close the bracket.

    auto z = z_algorithm(text);
    // Z-algorithm computes z[i] = length of the longest substring starting at i that is also a prefix of text.
    std::cout << "Z-array[0..4]: ";
    // Print only the first 5 elements to keep output concise.
    for (int i = 0; i < 5 && i < (int)z.size(); ++i) std::cout << z[i] << " ";
    // Range guard (int)z.size() avoids signed/unsigned comparison warning.
    std::cout << "\n";
    // Newline after the Z-array snippet.
}
// End of demo_string_algos().

void demo_graph_algos() {
// Demonstrates BFS, DFS, Dijkstra, Kruskal MST, and topological sort.
    print_section("Graph Algorithms");
    // Print sub-header.
    Graph g(6, true);
    // Directed weighted graph with 6 nodes; second parameter true = directed.
    g.add_edge(0,1,4); g.add_edge(0,2,1); g.add_edge(2,1,2);
    // Edges: 0→1 (w=4), 0→2 (w=1), 2→1 (w=2); cheaper path 0→2→1 has total weight 3.
    g.add_edge(1,3,1); g.add_edge(2,3,5); g.add_edge(3,4,3); g.add_edge(4,5,2);
    // More edges completing the path 0→2→1→3→4→5 with total weight 1+2+1+3+2=9.

    auto bfs_order = bfs(g, 0);
    // BFS explores nodes level-by-level from source 0 using a FIFO queue.
    std::cout << "BFS from 0: [ ";
    // Header for BFS traversal order.
    for (int v : bfs_order) std::cout << v << " ";
    // Print each visited node in BFS order.
    std::cout << "]\n";
    // Close bracket and newline.

    auto dfs_order = dfs(g, 0);
    // DFS explores as deep as possible before backtracking, using recursion or an explicit stack.
    std::cout << "DFS from 0: [ ";
    // Header for DFS traversal order.
    for (int v : dfs_order) std::cout << v << " ";
    // Print each visited node in DFS order.
    std::cout << "]\n";
    // Close bracket and newline.

    auto dist = dijkstra(g, 0);
    // Dijkstra's algorithm on the directed graph; returns shortest distances from node 0 to all nodes.
    std::cout << "Dijkstra from 0: [ ";
    // Header for Dijkstra distances.
    for (int d : dist) std::cout << (d == Graph::INF ? -1 : d) << " ";
    // Print -1 for unreachable nodes (INF sentinel), otherwise the shortest distance.
    std::cout << "]\n";
    // Close bracket and newline.

    Graph ug(5, false);
    // Undirected weighted graph with 5 nodes; second parameter false = undirected.
    ug.add_edge(0,1,2); ug.add_edge(0,3,6); ug.add_edge(1,2,3);
    // Three edges for the MST demo graph.
    ug.add_edge(1,3,8); ug.add_edge(1,4,5); ug.add_edge(2,4,7); ug.add_edge(3,4,9);
    // Four more edges; Kruskal will select the minimum spanning tree of total weight 16.
    auto mst_k = kruskal(ug, 5);
    // Kruskal sorts all edges by weight and adds them greedily using a DisjointSet to avoid cycles.
    std::cout << "Kruskal MST edges: ";
    // Header for MST edge list.
    for (auto& e : mst_k) std::cout << "(" << e.first << "-" << e.second << ") ";
    // Print each MST edge as (u-v).
    std::cout << "\n";
    // Newline after edge list.

    Graph dag(6, true);
    // Directed Acyclic Graph with 6 nodes; used for topological sort.
    dag.add_edge(5,2,1); dag.add_edge(5,0,1); dag.add_edge(4,0,1);
    // Edges from high-numbered nodes to lower ones (typical for dependency graphs).
    dag.add_edge(4,1,1); dag.add_edge(2,3,1); dag.add_edge(3,1,1);
    // More dependency edges; topological sort produces a linear order respecting all edges.
    auto topo = topological_sort(dag);
    // DFS-based topological sort: nodes are pushed to a stack when all their successors are visited.
    std::cout << "Topological sort: [ ";
    // Header for topological order.
    for (int v : topo) std::cout << v << " ";
    // Print each node in topologically sorted order.
    std::cout << "]\n";
    // Close bracket and newline.
}
// End of demo_graph_algos().

// ============================================================
//  DEMO: Core Systems
// ============================================================

void demo_lru_cache() {
// Demonstrates DME::LRUCache: put, get, eviction of the least-recently-used entry.
    print_section("LRU Cache (Search Engine Query Cache)");
    // Print sub-header; context: imagine caching search query results.
    DME::LRUCache<std::string, std::string> cache(3);
    // Constructs an LRU cache with capacity 3; backed by a doubly-linked list + unordered_map.
    cache.put("c++",    "results for c++");
    // Inserts key "c++" with its value; "c++" is now the most-recently-used entry.
    cache.put("python", "results for python");
    // Inserts "python"; order (MRU→LRU): python, c++.
    cache.put("java",   "results for java");
    // Inserts "java"; order: java, python, c++.

    std::string val;
    // Output variable for cache.get(); passed by reference.
    std::cout << "get('c++'):    " << (cache.get("c++", val) ? val : "MISS") << "\n";
    // get() finds "c++" and moves it to MRU position; order: c++, java, python.
    cache.put("rust", "results for rust");   // evicts LRU (python)
    // Cache is full; inserting "rust" evicts "python" (LRU after c++ was accessed); order: rust, c++, java.
    std::cout << "After adding rust — get('python'): "
              << (cache.get("python", val) ? "HIT" : "MISS (evicted)") << "\n";
    // "python" was evicted; get() returns false and prints "MISS (evicted)".
    std::cout << "Cache size: " << cache.size() << "/" << cache.capacity() << "\n";
    // size() = 3 (rust, c++, java), capacity() = 3.
}
// End of demo_lru_cache().

void demo_lfu_cache() {
// Demonstrates DME::LFUCache: evicts the entry with the lowest access frequency.
    print_section("LFU Cache (Hot Content Cache)");
    // Print sub-header; context: imagine caching frequently accessed documents.
    DME::LFUCache<int, std::string> cache(3);
    // Constructs an LFU cache with capacity 3; internally tracks per-key frequency counts.
    cache.put(1, "Doc_A"); cache.put(2, "Doc_B"); cache.put(3, "Doc_C");
    // Inserts three entries; all start with frequency 1.
    std::string val;
    // Output variable for get().
    cache.get(1, val); cache.get(1, val);  // access Doc_A twice — highest freq
    // Each get() increments key 1's frequency; freq: {1:3, 2:1, 3:1} (put counts as freq 1).
    cache.get(2, val);                      // access Doc_B once
    // Increments key 2's frequency; freq: {1:3, 2:2, 3:1}.
    cache.put(4, "Doc_D");                  // evicts Doc_C (lowest freq)
    // Cache is full; key 3 has the lowest frequency (1); Doc_C is evicted to make room for Doc_D.
    std::cout << "get(3)='Doc_C': " << (cache.get(3, val) ? "HIT" : "MISS (evicted — lowest freq)") << "\n";
    // Key 3 was evicted; should print "MISS".
    std::cout << "get(1)='Doc_A': " << (cache.get(1, val) ? val : "MISS") << "\n";
    // Key 1 was never evicted (highest freq); should print "Doc_A".
}
// End of demo_lfu_cache().

void demo_rate_limiter() {
// Demonstrates DME::TokenBucket: controls request rate by consuming tokens that refill over time.
    print_section("Rate Limiter (Token Bucket)");
    // Print sub-header; context: API gateway limiting requests per second.
    DME::TokenBucket limiter(10.0, 5);  // 10 tokens/sec, capacity 5
    // Constructs a token bucket: refill rate 10 tokens/sec, max capacity 5 tokens, starts full.
    std::cout << "Available tokens: " << limiter.available_tokens() << "\n";
    // Should report 5 (full bucket at construction).
    std::cout << "consume(3): " << (limiter.consume(3) ? "ALLOWED" : "DENIED") << "\n";
    // Removes 3 tokens; 2 remain. Returns true (allowed) because bucket had enough tokens.
    std::cout << "consume(3): " << (limiter.consume(3) ? "ALLOWED" : "DENIED") << "\n";  // only 2 left
    // Tries to consume 3 but only 2 are available (plus any refill since last call); likely DENIED.
    std::cout << "consume(1): " << (limiter.consume(1) ? "ALLOWED" : "DENIED") << "\n";
    // Consumes 1 token from whatever is left after refill; depends on timing.
    std::cout << "consume(1): " << (limiter.consume(1) ? "ALLOWED" : "DENIED") << " (bucket empty)\n";
    // Comment notes that the bucket is expected to be empty at this point.
}
// End of demo_rate_limiter().

void demo_load_balancer() {
// Demonstrates RoundRobinBalancer and LeastConnectionBalancer distributing requests across servers.
    print_section("Load Balancer (Round Robin + Least Connection)");
    // Print sub-header.
    DME::RoundRobinBalancer rr;
    // Constructs a round-robin balancer with an empty server list.
    rr.add_server(DME::Server(1, "10.0.0.1", 8080));
    // Registers Server 1 at address 10.0.0.1 port 8080.
    rr.add_server(DME::Server(2, "10.0.0.2", 8080));
    // Registers Server 2.
    rr.add_server(DME::Server(3, "10.0.0.3", 8080));
    // Registers Server 3; the balancer now has 3 servers to cycle through.

    std::cout << "Round Robin — 5 requests:\n";
    // Header for round-robin output.
    for (int i = 0; i < 5; ++i) {
    // Simulate 5 incoming requests.
        auto* s = rr.next_server();
        // next_server() returns a pointer to the next server in the cycle (wraps around with %).
        if (s) std::cout << "  Request " << i+1 << " -> " << s->address << "\n";
        // Print which server handled this request; i+1 for 1-based display.
    }
    // End of round-robin loop; distribution should be 1→2→3→1→2.

    DME::LeastConnectionBalancer lc;
    // Constructs a least-connection balancer; tracks active connection count per server.
    lc.add_server(DME::Server(1, "192.168.1.1", 9090));
    // Registers Server 1 at 192.168.1.1.
    lc.add_server(DME::Server(2, "192.168.1.2", 9090));
    // Registers Server 2.
    auto* s1 = lc.next_server();
    // Picks the server with fewest active connections; both start at 0, so picks by index or ID.
    auto* s2 = lc.next_server();
    // After s1's connection count incremented, s2 may have fewer connections now.
    auto* s3 = lc.next_server();
    // Third request; distribution depends on which server has the smallest count after s1 and s2.
    std::cout << "Least Connection — 3 requests distributed to servers "
              << (s1 ? s1->id : -1) << ", "
              // Print server ID of first request's target; -1 if next_server() returned nullptr.
              << (s2 ? s2->id : -1) << ", "
              // Print server ID of second request's target.
              << (s3 ? s3->id : -1) << "\n";
              // Print server ID of third request's target and end the line.
}
// End of demo_load_balancer().

void demo_scheduler() {
// Demonstrates DME::TaskScheduler: submit tasks with priorities, cancel one, then run all in order.
    print_section("Task Scheduler (Priority Queue)");
    // Print sub-header.
    DME::TaskScheduler scheduler;
    // Constructs an empty priority-queue-backed task scheduler.

    scheduler.submit(DME::Task(1, "Compress logs",    2, []{ std::cout << "    [WORK] Compressing logs\n"; }));
    // Submits task id=1, priority=2 (low); lambda is the actual work to execute.
    scheduler.submit(DME::Task(2, "Index documents",  5, []{ std::cout << "    [WORK] Indexing documents\n"; }));
    // Submits task id=2, priority=5 (medium); will run after task 3 (priority 9).
    scheduler.submit(DME::Task(3, "Alert: disk full", 9, []{ std::cout << "    [WORK] Handling disk alert\n"; }));
    // Submits task id=3, priority=9 (highest); will execute first in run_all().
    scheduler.submit(DME::Task(4, "Send report",      3, []{ std::cout << "    [WORK] Sending report\n"; }));
    // Submits task id=4, priority=3; will execute third (after tasks 3 and 2).
    scheduler.cancel(1);
    // Marks task id=1 as cancelled; it will be skipped during run_all() execution.

    std::cout << "Pending tasks: " << scheduler.pending_count() << "\n";
    // pending_count() returns the number of non-cancelled tasks still in the queue (should be 3).
    std::cout << "Running all tasks (highest priority first):\n";
    // Header before execution output.
    scheduler.run_all();
    // Pops and executes each task from the priority queue; cancelled tasks are skipped.
}
// End of demo_scheduler().

// ============================================================
//  DEMO: Simulations
// ============================================================

void demo_search_engine() {
// Demonstrates DME::SearchEngine: indexing documents and querying with relevance scoring.
    print_section("Search Engine Simulation");
    // Print sub-header.
    DME::SearchEngine engine;
    // Constructs a SearchEngine; next_id_ = 1, inverted_index_ and documents_ are empty.
    engine.index_document({0, "C++ Performance Guide",    "algorithms data structures memory management performance"});
    // Indexes doc 1; id 0 is overwritten by the engine; tokens extracted from title+content.
    engine.index_document({0, "Python Machine Learning",  "python algorithms neural networks data science performance"});
    // Indexes doc 2; "algorithms" and "performance" appear in multiple docs — they'll match queries broadly.
    engine.index_document({0, "System Design Interview",  "algorithms distributed systems data structures scalability"});
    // Indexes doc 3; "algorithms" and "data" are high-frequency cross-document terms.
    engine.index_document({0, "Data Structures in Java",  "data structures algorithms java performance memory"});
    // Indexes doc 4; "algorithms", "data", "structures", "performance" all appear — likely a top scorer.

    std::cout << "Indexed " << engine.document_count() << " documents\n";
    // document_count() should return 4.
    auto results = engine.search("algorithms data structures performance");
    // Tokenizes the query into 4 terms and scores all documents that contain any of them.
    std::cout << "Query: 'algorithms data structures performance'\n";
    // Prints the query for context.
    for (const auto& doc : results)
    // Iterate results in descending score order.
        std::cout << "  [score=" << std::fixed << std::setprecision(1)
                  // Fixed-point notation with 1 decimal place for cleaner score display.
                  << doc.relevance_score << "] " << doc.title << "\n";
                  // Print each document's score and title.
}
// End of demo_search_engine().

void demo_job_recommender() {
// Demonstrates DME::RecommendationEngine: skill-based job matching for a specific candidate.
    print_section("Job Recommender Simulation");
    // Print sub-header.
    DME::RecommendationEngine engine;
    // Constructs the engine; constructor calls load_sample_jobs(), populating 8 job postings.
    DME::Candidate candidate{
    // Aggregate-initializes a Candidate struct (no constructor needed for plain struct).
        "Muskan",
        // Candidate name; used for display only.
        {"C++", "algorithms", "systems", "Python", "distributed systems"},
        // Five skills; matched against job.required_skills in compute_match().
        130000
        // Minimum salary threshold; jobs below $130k will be filtered out.
    };
    // End of Candidate initialization.
    std::cout << "Candidate: " << candidate.name << "\n";
    // Print the candidate's name.
    std::cout << "Skills: [ ";
    // Opening bracket for skill list.
    for (const auto& s : candidate.skills) std::cout << s << " ";
    // Print each skill separated by a space.
    std::cout << "]\n";
    // Close bracket and newline.
    std::cout << "Min salary: $" << candidate.min_salary << "\n\n";
    // Print the salary floor; double newline adds spacing before the results table.

    auto recommendations = engine.recommend(candidate, 5);
    // Returns up to 5 top-matched jobs after filtering by salary and sorting by match score.
    std::cout << "Top recommendations:\n";
    // Header for the recommendations table.
    for (const auto& job : recommendations)
    // Iterate each recommended job.
        std::cout << "  " << std::setw(30) << std::left << job.title
                  // Left-align the title in a 30-char field for columnar output.
                  << " @ " << std::setw(12) << job.company
                  // Left-align company name in a 12-char field.
                  << " | match=" << std::setprecision(0) << std::fixed
                  // Format match score as an integer percentage.
                  << (job.match_score * 100) << "%"
                  // Multiply fractional score by 100 for percentage display.
                  << " | $" << job.salary << "\n";
                  // Print salary and newline.
}
// End of demo_job_recommender().

void demo_traffic_router() {
// Demonstrates DME::TrafficRouter: adds network links and finds the minimum-latency path.
    print_section("Traffic Router Simulation");
    // Print sub-header.
    DME::TrafficRouter router(6);
    // Constructs a router for 6 nodes (Node0 through Node5); adj_ is initialized with 6 empty lists.
    router.add_link(0, 1, 10);
    // Bidirectional link between Node0 and Node1 with 10ms latency.
    router.add_link(0, 2, 3);
    // Bidirectional link between Node0 and Node2 with 3ms latency (much cheaper than 0→1).
    router.add_link(1, 3, 2);
    // 1↔3 with 2ms; path 0→1→3 total = 10+2 = 12ms.
    router.add_link(2, 1, 4);
    // 2↔1 with 4ms; path 0→2→1 total = 3+4 = 7ms — cheaper than direct 0→1.
    router.add_link(2, 3, 8);
    // 2↔3 with 8ms.
    router.add_link(2, 4, 2);
    // 2↔4 with 2ms; enables the fast path 0→2→4.
    router.add_link(3, 5, 5);
    // 3↔5 with 5ms.
    router.add_link(4, 5, 1);
    // 4↔5 with 1ms; path 0→2→4→5 total = 3+2+1 = 6ms — the optimal route.

    auto route = router.find_best_route(0, 5);
    // Runs Dijkstra from Node0; the shortest path to Node5 should be 0→2→4→5 (6ms).
    std::cout << "Best route Node0 → Node5:\n";
    // Header for the route output.
    std::cout << "  Path    : " << route.path_description << "\n";
    // Prints the human-readable path string built in find_best_route().
    std::cout << "  Latency : " << route.latency_ms << "ms\n";
    // Prints the total latency in milliseconds (should be 6ms).
}
// End of demo_traffic_router().

void demo_analytics() {
// Demonstrates DME::SlidingWindowMetrics: recording, querying statistics, and evicting old data.
    print_section("Analytics — Sliding Window Metrics");
    // Print sub-header.
    DME::SlidingWindowMetrics metrics(60000);  // 60-second window
    // Constructs a metrics window of 60,000ms (60 seconds); samples older than this are evictable.

    long long t = 1000;
    // Base timestamp in milliseconds; represents "1 second after some epoch".
    metrics.record(t,      120.5);
    // Records 120.5ms latency at t=1000ms.
    metrics.record(t+5000,  98.3);
    // Records 98.3ms at t=6000ms (5 seconds later).
    metrics.record(t+10000, 145.2);
    // Records 145.2ms at t=11000ms (10 seconds after base).
    metrics.record(t+20000, 110.0);
    // Records 110.0ms at t=21000ms (20 seconds after base).
    metrics.record(t+30000, 133.7);
    // Records 133.7ms at t=31000ms (30 seconds after base); all 5 samples are within the 60s window.

    std::cout << "Window has " << metrics.count() << " data points\n";
    // count() should return 5; no eviction has been called yet.
    std::cout << "Average latency: " << std::fixed << std::setprecision(2)
              // Fixed-point with 2 decimal places for clean latency display.
              << metrics.average() << "ms\n";
              // average() = (120.5+98.3+145.2+110.0+133.7) / 5 = 121.54ms.
    std::cout << "Max latency:     " << metrics.max() << "ms\n";
    // max() = 145.2ms (highest sample).
    std::cout << "Min latency:     " << metrics.min() << "ms\n";
    // min() = 98.3ms (lowest sample).

    metrics.evict(t + 65000);  // evict points older than 60s from t+65000
    // Cutoff = (t+65000) - 60000 = t+5000; samples with timestamp < t+5000 are removed.
    // The first sample (t=1000, which is t+0) is older than t+5000, so it gets evicted.
    std::cout << "After eviction: " << metrics.count() << " data points remain\n";
    // The sample at t=1000 (timestamp_ms=1000 < cutoff=6000) is evicted; 4 samples remain.
}
// End of demo_analytics().

// ============================================================
//  MAIN
// ============================================================

int main() {
// Program entry point: configures the logger, then runs all demo functions in order.
    DME::Logger::instance().set_level(DME::LogLevel::WARN);  // suppress INFO in demo output
    // Singleton logger is set to WARN so only warnings and errors appear; INFO is silenced for cleaner output.

    std::cout << "\n";
    // Blank line at the very top for visual breathing room before the banner.
    std::cout << "##############################################\n";
    // Top border of the opening banner.
    std::cout << "##     DSA Master Engine — Full Demo        ##\n";
    // Title line of the banner.
    std::cout << "##############################################\n";
    // Bottom border of the opening banner.

    // --- Data Structures ---
    print_header("DATA STRUCTURES");
    // Prints the "DATA STRUCTURES" section box header.
    demo_dynamic_array();
    // Runs the DynamicArray demo.
    demo_singly_ll();
    // Runs the SinglyLinkedList demo.
    demo_stack();
    // Runs the Stack demo.
    demo_queue();
    // Runs the SimpleQueue + CircularQueue + Deque demo.
    demo_hashmap();
    // Runs the HashMap (chaining + open addressing) demo.
    demo_bst();
    // Runs the BST + AVLTree demo.
    demo_heap();
    // Runs the MaxHeap + MinHeap demo.
    demo_segment_fenwick();
    // Runs the SegmentTree + FenwickTree demo.
    demo_trie();
    // Runs the Trie (insert, search, autocomplete) demo.
    demo_graph_ds();
    // Runs the Graph + DisjointSet demo.

    // --- Algorithms ---
    print_header("ALGORITHMS");
    // Prints the "ALGORITHMS" section box header.
    demo_sorting();
    // Runs all 8 sorting algorithm demos.
    demo_searching();
    // Runs linear, binary, and exponential search demos.
    demo_greedy();
    // Runs activity selection and Huffman coding demos.
    demo_dp();
    // Runs knapsack, LCS, LIS, and matrix chain multiplication demos.
    demo_backtracking();
    // Runs N-Queens and Sudoku solver demos.
    demo_string_algos();
    // Runs KMP, Rabin-Karp, and Z-algorithm demos.
    demo_graph_algos();
    // Runs BFS, DFS, Dijkstra, Kruskal, and topological sort demos.

    // --- Core Systems ---
    print_header("CORE SYSTEMS");
    // Prints the "CORE SYSTEMS" section box header.
    demo_lru_cache();
    // Runs the LRU Cache demo.
    demo_lfu_cache();
    // Runs the LFU Cache demo.
    demo_rate_limiter();
    // Runs the Token Bucket rate limiter demo.
    demo_load_balancer();
    // Runs the Round Robin + Least Connection load balancer demo.
    demo_scheduler();
    // Runs the priority-queue task scheduler demo.

    // --- Simulations ---
    print_header("SIMULATIONS");
    // Prints the "SIMULATIONS" section box header.
    demo_search_engine();
    // Runs the full-text search engine simulation.
    demo_job_recommender();
    // Runs the skill-based job recommendation simulation.
    demo_traffic_router();
    // Runs the Dijkstra-based network traffic router simulation.
    demo_analytics();
    // Runs the sliding-window metrics analytics simulation.

    std::cout << "\n##############################################\n";
    // Closing banner top border.
    std::cout << "##           All demos complete             ##\n";
    // Closing banner title.
    std::cout << "##############################################\n\n";
    // Closing banner bottom border with trailing newline.

    return 0;
    // Returns 0 to the OS, signaling successful program termination.
}
// End of main().
