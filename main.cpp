#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// --- Config ---
#include "config/constants.h"
#include "config/system_config.h"

// --- Core ---
#include "core/logger/logger.h"
#include "core/memory_manager/lru_cache.h"
#include "core/memory_manager/lfu_cache.h"
#include "core/rate_limiter/token_bucket.h"
#include "core/load_balancer/server.h"
#include "core/load_balancer/round_robin.h"
#include "core/load_balancer/least_connection.h"
#include "core/schedular/task.h"
#include "core/schedular/task_schedular.h"

// --- Data Structures ---
#include "data_structures/array/dynamic_array.h"
#include "data_structures/linked_list/singly_ll.h"
#include "data_structures/linked_list/doubly_ll.h"
#include "data_structures/linked_list/circular_ll.h"
#include "data_structures/stack/stack_array.h"
#include "data_structures/queue/simple_queue.h"
#include "data_structures/queue/circular_queue.h"
#include "data_structures/queue/deque.h"
#include "data_structures/hashmap/hashmap_chaining.h"
#include "data_structures/hashmap/hashmap_open_addressing.h"
#include "data_structures/tree/bst.h"
#include "data_structures/tree/avl_tree.h"
#include "data_structures/tree/heap.h"
#include "data_structures/tree/segment_tree.h"
#include "data_structures/tree/fenwick_tree.h"
#include "data_structures/trie/trie.h"
#include "data_structures/graph/graph.h"
#include "data_structures/graph/disjoint_set.h"

// --- Algorithms ---
#include "algorithms/sorting/sorting.h"
#include "algorithms/searching/searching.h"
#include "algorithms/greedy/greedy.h"
#include "algorithms/divide_conquer/divide_conquer.h"
#include "algorithms/bactracking/backtracking.h"
#include "algorithms/dynamic_programming/dp.h"
#include "algorithms/string_algorithms/string_algo.h"
#include "algorithms/graph_algorithms/graph_algo.h"

// --- Simulations ---
#include "simulations/search_engine/search_engine.h"
#include "simulations/job_recommender/job_matcher.h"
#include "simulations/job_recommender/recommendation_engine.h"
#include "simulations/traffic_router/traffic_router.h"
#include "simulations/analytics/sliding_window_metrics.h"

// ============================================================
static void print_header(const std::string& title) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

static void print_section(const std::string& name) {
    std::cout << "\n--- " << name << " ---\n";
}

static void print_vec(const std::string& label, const std::vector<int>& v) {
    std::cout << label << ": [ ";
    for (int x : v) std::cout << x << " ";
    std::cout << "]\n";
}

// ============================================================
//  DEMO: Data Structures
// ============================================================

void demo_dynamic_array() {
    print_section("DynamicArray<int>");
    DynamicArray<int> arr;
    for (int i = 1; i <= 6; ++i) arr.push_back(i * 10);
    std::cout << "After 6 push_backs — size=" << arr.size()
              << " capacity=" << arr.capacity() << "\n";
    std::cout << "arr[2] = " << arr[2] << " (O(1) access)\n";
    arr.pop_back();
    std::cout << "After pop_back  — size=" << arr.size() << "\n";
}

void demo_singly_ll() {
    print_section("SinglyLinkedList<int>");
    SinglyLinkedList<int> list;
    list.insert_tail(10);
    list.insert_tail(20);
    list.insert_tail(30);
    list.insert_head(5);
    std::cout << "List size=" << list.size() << "\n";
    std::cout << "Search 20: " << (list.search(20) ? "found" : "not found") << "\n";
    list.delete_by_value(20);
    std::cout << "After delete_by_value(20) size=" << list.size() << "\n";
    std::cout << "Search 20: " << (list.search(20) ? "found" : "not found") << "\n";
}

void demo_stack() {
    print_section("Stack<int>");
    Stack<int> stk;
    stk.push(1); stk.push(2); stk.push(3);
    std::cout << "Top=" << stk.top() << " size=" << stk.size() << "\n";
    stk.pop();
    std::cout << "After pop: Top=" << stk.top() << "\n";
}

void demo_queue() {
    print_section("SimpleQueue + CircularQueue + Deque");
    SimpleQueue<int> q;
    q.enqueue(10); q.enqueue(20); q.enqueue(30);
    std::cout << "SimpleQueue front=" << q.front() << " size=" << q.size() << "\n";
    q.dequeue();
    std::cout << "After dequeue: front=" << q.front() << "\n";

    CircularQueue<int> cq(5);
    cq.enqueue(1); cq.enqueue(2); cq.enqueue(3);
    std::cout << "CircularQueue front=" << cq.front() << " size=" << cq.size() << "\n";

    Deque<int> dq;
    dq.push_back(10); dq.push_front(5); dq.push_back(20);
    std::cout << "Deque front=" << dq.front() << " back=" << dq.back() << " size=" << dq.size() << "\n";
}

void demo_hashmap() {
    print_section("HashMap (Chaining + Open Addressing)");
    HashMapChaining<std::string, int> hmc;
    hmc.insert("alice", 95);
    hmc.insert("bob",   82);
    hmc.insert("carol", 91);
    int score = 0;
    std::cout << "Chaining: alice=" << (hmc.get("alice", score) ? std::to_string(score) : "?") << "\n";
    hmc.remove("bob");
    std::cout << "After remove(bob): contains bob=" << hmc.contains("bob") << "\n";

    HashMapOpenAddressing<std::string, int> hmo;
    hmo.insert("x", 100); hmo.insert("y", 200);
    std::cout << "OpenAddr: x=" << (hmo.get("x", score) ? std::to_string(score) : "?") << "\n";
}

void demo_bst() {
    print_section("BST + AVLTree");
    BST<int> bst;
    for (int x : {5, 3, 7, 1, 4, 6, 8}) bst.insert(x);
    std::cout << "BST: search(4)=" << bst.search(4) << " search(9)=" << bst.search(9) << "\n";
    bst.remove(4);
    std::cout << "After remove(4): search(4)=" << bst.search(4) << "\n";

    AVLTree<int> avl;
    for (int x : {10, 20, 30, 40, 50}) avl.insert(x);  // would be right-heavy without rotations
    std::cout << "AVL size=" << avl.size() << " search(30)=" << avl.search(30) << "\n";
}

void demo_heap() {
    print_section("MaxHeap + MinHeap");
    MaxHeap<int> maxh;
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) maxh.push(x);
    std::cout << "MaxHeap top=" << maxh.top() << " (should be 9)\n";

    MinHeap<int> minh;
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) minh.push(x);
    std::cout << "MinHeap top=" << minh.top() << " (should be 1)\n";
}

void demo_segment_fenwick() {
    print_section("SegmentTree + FenwickTree");
    std::vector<int> data = {1, 3, 5, 7, 9, 11};
    SegmentTree<int> seg(data, [](int a, int b){ return a + b; }, 0);
    std::cout << "SegTree sum[1..4]=" << seg.query(1, 4) << " (expect 24)\n";
    seg.update(2, 10);
    std::cout << "After update(2,10) sum[1..4]=" << seg.query(1, 4) << " (expect 29)\n";

    FenwickTree fen(6);
    for (int i = 0; i < 6; ++i) fen.update(i, data[i]);
    std::cout << "Fenwick prefix_sum[0..4]=" << fen.prefix_sum(4) << " (expect 25)\n";
    std::cout << "Fenwick range_sum[2..4]=" << fen.range_sum(2, 4) << " (expect 21)\n";
}

void demo_trie() {
    print_section("Trie");
    Trie trie;
    trie.insert("apple"); trie.insert("app"); trie.insert("application");
    trie.insert("banana"); trie.insert("band");
    std::cout << "search('app')=" << trie.search("app") << "\n";
    std::cout << "search('apt')=" << trie.search("apt") << "\n";
    std::cout << "starts_with('app')=" << trie.starts_with("app") << "\n";
    auto completions = trie.autocomplete("app");
    std::cout << "autocomplete('app'): ";
    for (const auto& w : completions) std::cout << w << " ";
    std::cout << "\n";
}

void demo_graph_ds() {
    print_section("Graph + DisjointSet");
    Graph g(5);
    g.add_edge(0,1); g.add_edge(1,2); g.add_edge(2,3); g.add_edge(3,4);
    std::cout << "Graph: " << g.vertex_count() << " vertices\n";

    DisjointSet ds(5);
    ds.unite(0,1); ds.unite(2,3);
    std::cout << "DisjointSet: components=" << ds.components() << " (expect 3)\n";
    std::cout << "connected(0,1)=" << ds.connected(0,1) << " connected(0,2)=" << ds.connected(0,2) << "\n";
}

// ============================================================
//  DEMO: Algorithms
// ============================================================

void demo_sorting() {
    print_section("Sorting Algorithms");
    std::vector<int> original = {64, 34, 25, 12, 22, 11, 90};

    auto run = [&](const std::string& name, std::function<void(std::vector<int>&)> fn) {
        auto arr = original;
        fn(arr);
        std::cout << std::setw(18) << std::left << name << ": [ ";
        for (int x : arr) std::cout << x << " ";
        std::cout << "]\n";
    };

    run("bubble_sort",    bubble_sort);
    run("insertion_sort", insertion_sort);
    run("selection_sort", selection_sort);
    run("merge_sort",     merge_sort);
    run("quick_sort",     quick_sort);
    run("heap_sort",      heap_sort);
    run("counting_sort",  counting_sort);
    run("radix_sort",     radix_sort);
}

void demo_searching() {
    print_section("Searching Algorithms");
    std::vector<int> sorted = {5, 10, 15, 20, 25, 30, 35, 40};
    std::cout << "Array: [ "; for (int x : sorted) std::cout << x << " "; std::cout << "]\n";
    std::cout << "linear_search(25)     = index " << linear_search(sorted, 25)      << "\n";
    std::cout << "binary_search(25)     = index " << binary_search_idx(sorted, 25)  << "\n";
    std::cout << "exponential_search(40)= index " << exponential_search(sorted, 40) << "\n";
    std::cout << "linear_search(99)     = index " << linear_search(sorted, 99)      << " (not found)\n";
}

void demo_greedy() {
    print_section("Greedy Algorithms");
    std::vector<Activity> acts = {{0,6,1},{1,4,2},{3,5,3},{3,9,4},{5,9,5},{5,7,6},{8,11,7},{8,12,8},{2,14,9},{12,16,10}};
    auto selected = activity_selection(acts);
    std::cout << "Activity Selection — selected " << selected.size() << " activities\n";
    for (const auto& a : selected)
        std::cout << "  Activity " << a.id << " [" << a.start << "," << a.end << "]\n";

    std::vector<std::pair<char,int>> freq = {{'a',45},{'b',13},{'c',12},{'d',16},{'e',9},{'f',5}};
    auto codes = huffman_coding(freq);
    std::cout << "Huffman codes:\n";
    for (const auto& c : codes)
        std::cout << "  '" << c.ch << "' freq=" << c.freq << " code=" << c.code << "\n";
}

void demo_dp() {
    print_section("Dynamic Programming");
    std::vector<int> weights = {2, 3, 4, 5};
    std::vector<int> values  = {3, 4, 5, 6};
    std::cout << "0/1 Knapsack (W=8): " << knapsack(8, weights, values) << " (expect 10)\n";

    std::cout << "LCS('ABCBDAB','BDCAB')=" << lcs("ABCBDAB", "BDCAB") << " (expect 4)\n";

    std::vector<int> seq = {10, 9, 2, 5, 3, 7, 101, 18};
    std::cout << "LIS of {10,9,2,5,3,7,101,18}=" << lis(seq) << " (expect 4)\n";

    std::vector<int> dims = {40, 20, 30, 10, 30};
    std::cout << "Matrix Chain {40x20,20x30,30x10,10x30}=" << matrix_chain(dims) << " (expect 26000)\n";
}

void demo_backtracking() {
    print_section("Backtracking");
    auto solutions = solve_n_queens(4);
    std::cout << "4-Queens solutions: " << solutions.size() << " (expect 2)\n";
    for (const auto& sol : solutions) {
        std::cout << "  [ ";
        for (int c : sol) std::cout << "Q" << c << " ";
        std::cout << "]\n";
    }

    std::vector<std::vector<int>> board = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };
    bool solved = solve_sudoku(board);
    std::cout << "Sudoku solved: " << (solved ? "YES" : "NO") << "\n";
    if (solved) std::cout << "  First row: ";
    for (int x : board[0]) std::cout << x << " ";
    std::cout << "\n";
}

void demo_string_algos() {
    print_section("String Algorithms");
    std::string text    = "aabaacaadaabaaba";
    std::string pattern = "aaba";
    auto kmp_matches = kmp_search(text, pattern);
    std::cout << "KMP '" << pattern << "' in '" << text << "': matches at [ ";
    for (int idx : kmp_matches) std::cout << idx << " ";
    std::cout << "]\n";

    auto rk_matches = rabin_karp_search(text, pattern);
    std::cout << "Rabin-Karp: matches at [ ";
    for (int idx : rk_matches) std::cout << idx << " ";
    std::cout << "]\n";

    auto z = z_algorithm(text);
    std::cout << "Z-array[0..4]: ";
    for (int i = 0; i < 5 && i < (int)z.size(); ++i) std::cout << z[i] << " ";
    std::cout << "\n";
}

void demo_graph_algos() {
    print_section("Graph Algorithms");
    Graph g(6, true);
    g.add_edge(0,1,4); g.add_edge(0,2,1); g.add_edge(2,1,2);
    g.add_edge(1,3,1); g.add_edge(2,3,5); g.add_edge(3,4,3); g.add_edge(4,5,2);

    auto bfs_order = bfs(g, 0);
    std::cout << "BFS from 0: [ ";
    for (int v : bfs_order) std::cout << v << " ";
    std::cout << "]\n";

    auto dfs_order = dfs(g, 0);
    std::cout << "DFS from 0: [ ";
    for (int v : dfs_order) std::cout << v << " ";
    std::cout << "]\n";

    auto dist = dijkstra(g, 0);
    std::cout << "Dijkstra from 0: [ ";
    for (int d : dist) std::cout << (d == Graph::INF ? -1 : d) << " ";
    std::cout << "]\n";

    Graph ug(5, false);
    ug.add_edge(0,1,2); ug.add_edge(0,3,6); ug.add_edge(1,2,3);
    ug.add_edge(1,3,8); ug.add_edge(1,4,5); ug.add_edge(2,4,7); ug.add_edge(3,4,9);
    auto mst_k = kruskal(ug, 5);
    std::cout << "Kruskal MST edges: ";
    for (auto& e : mst_k) std::cout << "(" << e.first << "-" << e.second << ") ";
    std::cout << "\n";

    Graph dag(6, true);
    dag.add_edge(5,2,1); dag.add_edge(5,0,1); dag.add_edge(4,0,1);
    dag.add_edge(4,1,1); dag.add_edge(2,3,1); dag.add_edge(3,1,1);
    auto topo = topological_sort(dag);
    std::cout << "Topological sort: [ ";
    for (int v : topo) std::cout << v << " ";
    std::cout << "]\n";
}

// ============================================================
//  DEMO: Core Systems
// ============================================================

void demo_lru_cache() {
    print_section("LRU Cache (Search Engine Query Cache)");
    DME::LRUCache<std::string, std::string> cache(3);
    cache.put("c++",    "results for c++");
    cache.put("python", "results for python");
    cache.put("java",   "results for java");

    std::string val;
    std::cout << "get('c++'):    " << (cache.get("c++", val) ? val : "MISS") << "\n";
    cache.put("rust", "results for rust");   // evicts LRU (python)
    std::cout << "After adding rust — get('python'): "
              << (cache.get("python", val) ? "HIT" : "MISS (evicted)") << "\n";
    std::cout << "Cache size: " << cache.size() << "/" << cache.capacity() << "\n";
}

void demo_lfu_cache() {
    print_section("LFU Cache (Hot Content Cache)");
    DME::LFUCache<int, std::string> cache(3);
    cache.put(1, "Doc_A"); cache.put(2, "Doc_B"); cache.put(3, "Doc_C");
    std::string val;
    cache.get(1, val); cache.get(1, val);  // access Doc_A twice — highest freq
    cache.get(2, val);                      // access Doc_B once
    cache.put(4, "Doc_D");                  // evicts Doc_C (lowest freq)
    std::cout << "get(3)='Doc_C': " << (cache.get(3, val) ? "HIT" : "MISS (evicted — lowest freq)") << "\n";
    std::cout << "get(1)='Doc_A': " << (cache.get(1, val) ? val : "MISS") << "\n";
}

void demo_rate_limiter() {
    print_section("Rate Limiter (Token Bucket)");
    DME::TokenBucket limiter(10.0, 5);  // 10 tokens/sec, capacity 5
    std::cout << "Available tokens: " << limiter.available_tokens() << "\n";
    std::cout << "consume(3): " << (limiter.consume(3) ? "ALLOWED" : "DENIED") << "\n";
    std::cout << "consume(3): " << (limiter.consume(3) ? "ALLOWED" : "DENIED") << "\n";  // only 2 left
    std::cout << "consume(1): " << (limiter.consume(1) ? "ALLOWED" : "DENIED") << "\n";
    std::cout << "consume(1): " << (limiter.consume(1) ? "ALLOWED" : "DENIED") << " (bucket empty)\n";
}

void demo_load_balancer() {
    print_section("Load Balancer (Round Robin + Least Connection)");
    DME::RoundRobinBalancer rr;
    rr.add_server(DME::Server(1, "10.0.0.1", 8080));
    rr.add_server(DME::Server(2, "10.0.0.2", 8080));
    rr.add_server(DME::Server(3, "10.0.0.3", 8080));

    std::cout << "Round Robin — 5 requests:\n";
    for (int i = 0; i < 5; ++i) {
        auto* s = rr.next_server();
        if (s) std::cout << "  Request " << i+1 << " -> " << s->address << "\n";
    }

    DME::LeastConnectionBalancer lc;
    lc.add_server(DME::Server(1, "192.168.1.1", 9090));
    lc.add_server(DME::Server(2, "192.168.1.2", 9090));
    auto* s1 = lc.next_server();
    auto* s2 = lc.next_server();
    auto* s3 = lc.next_server();
    std::cout << "Least Connection — 3 requests distributed to servers "
              << (s1 ? s1->id : -1) << ", "
              << (s2 ? s2->id : -1) << ", "
              << (s3 ? s3->id : -1) << "\n";
}

void demo_scheduler() {
    print_section("Task Scheduler (Priority Queue)");
    DME::TaskScheduler scheduler;

    scheduler.submit(DME::Task(1, "Compress logs",    2, []{ std::cout << "    [WORK] Compressing logs\n"; }));
    scheduler.submit(DME::Task(2, "Index documents",  5, []{ std::cout << "    [WORK] Indexing documents\n"; }));
    scheduler.submit(DME::Task(3, "Alert: disk full", 9, []{ std::cout << "    [WORK] Handling disk alert\n"; }));
    scheduler.submit(DME::Task(4, "Send report",      3, []{ std::cout << "    [WORK] Sending report\n"; }));
    scheduler.cancel(1);

    std::cout << "Pending tasks: " << scheduler.pending_count() << "\n";
    std::cout << "Running all tasks (highest priority first):\n";
    scheduler.run_all();
}

// ============================================================
//  DEMO: Simulations
// ============================================================

void demo_search_engine() {
    print_section("Search Engine Simulation");
    DME::SearchEngine engine;
    engine.index_document({0, "C++ Performance Guide",    "algorithms data structures memory management performance"});
    engine.index_document({0, "Python Machine Learning",  "python algorithms neural networks data science performance"});
    engine.index_document({0, "System Design Interview",  "algorithms distributed systems data structures scalability"});
    engine.index_document({0, "Data Structures in Java",  "data structures algorithms java performance memory"});

    std::cout << "Indexed " << engine.document_count() << " documents\n";
    auto results = engine.search("algorithms data structures performance");
    std::cout << "Query: 'algorithms data structures performance'\n";
    for (const auto& doc : results)
        std::cout << "  [score=" << std::fixed << std::setprecision(1)
                  << doc.relevance_score << "] " << doc.title << "\n";
}

void demo_job_recommender() {
    print_section("Job Recommender Simulation");
    DME::RecommendationEngine engine;
    DME::Candidate candidate{
        "Muskan",
        {"C++", "algorithms", "systems", "Python", "distributed systems"},
        130000
    };
    std::cout << "Candidate: " << candidate.name << "\n";
    std::cout << "Skills: [ ";
    for (const auto& s : candidate.skills) std::cout << s << " ";
    std::cout << "]\n";
    std::cout << "Min salary: $" << candidate.min_salary << "\n\n";

    auto recommendations = engine.recommend(candidate, 5);
    std::cout << "Top recommendations:\n";
    for (const auto& job : recommendations)
        std::cout << "  " << std::setw(30) << std::left << job.title
                  << " @ " << std::setw(12) << job.company
                  << " | match=" << std::setprecision(0) << std::fixed
                  << (job.match_score * 100) << "%"
                  << " | $" << job.salary << "\n";
}

void demo_traffic_router() {
    print_section("Traffic Router Simulation");
    DME::TrafficRouter router(6);
    router.add_link(0, 1, 10);
    router.add_link(0, 2, 3);
    router.add_link(1, 3, 2);
    router.add_link(2, 1, 4);
    router.add_link(2, 3, 8);
    router.add_link(2, 4, 2);
    router.add_link(3, 5, 5);
    router.add_link(4, 5, 1);

    auto route = router.find_best_route(0, 5);
    std::cout << "Best route Node0 → Node5:\n";
    std::cout << "  Path    : " << route.path_description << "\n";
    std::cout << "  Latency : " << route.latency_ms << "ms\n";
}

void demo_analytics() {
    print_section("Analytics — Sliding Window Metrics");
    DME::SlidingWindowMetrics metrics(60000);  // 60-second window

    long long t = 1000;
    metrics.record(t,      120.5);
    metrics.record(t+5000,  98.3);
    metrics.record(t+10000, 145.2);
    metrics.record(t+20000, 110.0);
    metrics.record(t+30000, 133.7);

    std::cout << "Window has " << metrics.count() << " data points\n";
    std::cout << "Average latency: " << std::fixed << std::setprecision(2)
              << metrics.average() << "ms\n";
    std::cout << "Max latency:     " << metrics.max() << "ms\n";
    std::cout << "Min latency:     " << metrics.min() << "ms\n";

    metrics.evict(t + 65000);  // evict points older than 60s from t+65000
    std::cout << "After eviction: " << metrics.count() << " data points remain\n";
}

// ============================================================
//  MAIN
// ============================================================

int main() {
    DME::Logger::instance().set_level(DME::LogLevel::WARN);  // suppress INFO in demo output

    std::cout << "\n";
    std::cout << "##############################################\n";
    std::cout << "##     DSA Master Engine — Full Demo        ##\n";
    std::cout << "##############################################\n";

    // --- Data Structures ---
    print_header("DATA STRUCTURES");
    demo_dynamic_array();
    demo_singly_ll();
    demo_stack();
    demo_queue();
    demo_hashmap();
    demo_bst();
    demo_heap();
    demo_segment_fenwick();
    demo_trie();
    demo_graph_ds();

    // --- Algorithms ---
    print_header("ALGORITHMS");
    demo_sorting();
    demo_searching();
    demo_greedy();
    demo_dp();
    demo_backtracking();
    demo_string_algos();
    demo_graph_algos();

    // --- Core Systems ---
    print_header("CORE SYSTEMS");
    demo_lru_cache();
    demo_lfu_cache();
    demo_rate_limiter();
    demo_load_balancer();
    demo_scheduler();

    // --- Simulations ---
    print_header("SIMULATIONS");
    demo_search_engine();
    demo_job_recommender();
    demo_traffic_router();
    demo_analytics();

    std::cout << "\n##############################################\n";
    std::cout << "##           All demos complete             ##\n";
    std::cout << "##############################################\n\n";

    return 0;
}
