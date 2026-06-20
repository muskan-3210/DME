from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import cm
from reportlab.lib import colors
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle,
    PageBreak, HRFlowable, KeepTogether
)
from reportlab.lib.enums import TA_LEFT, TA_CENTER, TA_JUSTIFY
from reportlab.platypus import BaseDocTemplate, Frame, PageTemplate

# ── Colours ──────────────────────────────────────────────────────────────────
C_DARK    = colors.HexColor("#1a1a2e")
C_BLUE    = colors.HexColor("#0f3460")
C_ACCENT  = colors.HexColor("#16213e")
C_GOLD    = colors.HexColor("#e94560")
C_LIGHT   = colors.HexColor("#f5f5f5")
C_GREY    = colors.HexColor("#666666")
C_GREEN   = colors.HexColor("#2d6a4f")
C_CODE_BG = colors.HexColor("#f0f0f0")
C_WHITE   = colors.white

# ── Styles ───────────────────────────────────────────────────────────────────
styles = getSampleStyleSheet()

def make_style(name, parent="Normal", **kw):
    return ParagraphStyle(name, parent=styles[parent], **kw)

S_TITLE   = make_style("MyTitle",   "Title",
                        fontSize=28, textColor=C_WHITE,
                        spaceAfter=6, alignment=TA_CENTER, leading=34)
S_SUBTITLE= make_style("MySub",    "Normal",
                        fontSize=14, textColor=colors.HexColor("#aaaaaa"),
                        alignment=TA_CENTER, spaceAfter=4)
S_AUTHOR  = make_style("MyAuthor", "Normal",
                        fontSize=11, textColor=C_GOLD,
                        alignment=TA_CENTER, spaceAfter=2)

S_H1      = make_style("MyH1", "Heading1",
                        fontSize=18, textColor=C_WHITE,
                        spaceBefore=14, spaceAfter=6, leading=22,
                        backColor=C_BLUE,
                        borderPad=(6,8,6,8))
S_H2      = make_style("MyH2", "Heading2",
                        fontSize=13, textColor=C_BLUE,
                        spaceBefore=10, spaceAfter=4, leading=16)
S_H3      = make_style("MyH3", "Heading3",
                        fontSize=11, textColor=C_ACCENT,
                        spaceBefore=8, spaceAfter=3, leading=14)
S_BODY    = make_style("MyBody", "Normal",
                        fontSize=10, leading=15, spaceAfter=6,
                        alignment=TA_JUSTIFY)
S_BULLET  = make_style("MyBullet", "Normal",
                        fontSize=10, leading=14, spaceAfter=3,
                        leftIndent=16, bulletIndent=6)
S_CODE    = make_style("MyCode", "Code",
                        fontSize=8.5, leading=12, spaceAfter=6,
                        backColor=C_CODE_BG, leftIndent=12,
                        fontName="Courier", textColor=C_ACCENT)
S_TABLE_H = make_style("MyTH", "Normal",
                        fontSize=9, textColor=C_WHITE,
                        fontName="Helvetica-Bold", alignment=TA_CENTER)
S_TABLE_C = make_style("MyTC", "Normal",
                        fontSize=9, textColor=C_ACCENT,
                        alignment=TA_LEFT, leading=12)
S_TABLE_CC= make_style("MyTCC","Normal",
                        fontSize=9, textColor=C_ACCENT,
                        alignment=TA_CENTER, leading=12)
S_CAPTION = make_style("MyCap","Normal",
                        fontSize=8, textColor=C_GREY,
                        alignment=TA_CENTER, spaceAfter=8)
S_QUOTE   = make_style("MyQ","Normal",
                        fontSize=10, textColor=C_ACCENT,
                        leftIndent=24, rightIndent=24,
                        leading=15, spaceAfter=8,
                        backColor=colors.HexColor("#e8f4f8"),
                        borderPad=8)

# ── Helpers ──────────────────────────────────────────────────────────────────
def h1(text):
    return [Spacer(1, 6), Paragraph(text, S_H1), Spacer(1, 4)]

def h2(text):
    return [Paragraph(text, S_H2)]

def h3(text):
    return [Paragraph(text, S_H3)]

def body(text):
    return [Paragraph(text, S_BODY)]

def bullet(items, symbol="&#8226;"):
    return [Paragraph(f"{symbol}  {item}", S_BULLET) for item in items]

def code(text):
    return [Paragraph(text.replace("\n","<br/>").replace(" ","&nbsp;"), S_CODE)]

def rule():
    return [HRFlowable(width="100%", thickness=1, color=C_BLUE, spaceAfter=6)]

def spacer(h=8):
    return [Spacer(1, h)]

def table(headers, rows, col_widths=None, center_cols=None):
    center_cols = center_cols or []
    data = [[Paragraph(h, S_TABLE_H) for h in headers]]
    for row in rows:
        data.append([
            Paragraph(str(cell), S_TABLE_CC if i in center_cols else S_TABLE_C)
            for i, cell in enumerate(row)
        ])
    t = Table(data, colWidths=col_widths, repeatRows=1)
    t.setStyle(TableStyle([
        ("BACKGROUND",   (0,0), (-1,0),  C_BLUE),
        ("BACKGROUND",   (0,1), (-1,-1), C_WHITE),
        ("ROWBACKGROUNDS",(0,1),(-1,-1), [C_WHITE, C_LIGHT]),
        ("GRID",         (0,0), (-1,-1), 0.4, colors.HexColor("#cccccc")),
        ("TOPPADDING",   (0,0), (-1,-1), 5),
        ("BOTTOMPADDING",(0,0), (-1,-1), 5),
        ("LEFTPADDING",  (0,0), (-1,-1), 7),
        ("RIGHTPADDING", (0,0), (-1,-1), 7),
        ("VALIGN",       (0,0), (-1,-1), "MIDDLE"),
    ]))
    return [t, Spacer(1, 6)]

def quote(text):
    return [Paragraph(f'<i>"{text}"</i>', S_QUOTE), Spacer(1,4)]

def badge(text, bg=C_BLUE):
    st = ParagraphStyle("badge","Normal",
                        fontSize=8, textColor=C_WHITE, backColor=bg,
                        borderPad=3, leading=11, spaceAfter=4)
    return Paragraph(text, st)

# ── Cover page builder ────────────────────────────────────────────────────────
def cover_page(canvas, doc):
    canvas.saveState()
    w, h = A4
    # Background gradient blocks
    canvas.setFillColor(C_DARK)
    canvas.rect(0, 0, w, h, fill=1, stroke=0)
    canvas.setFillColor(C_BLUE)
    canvas.rect(0, h*0.55, w, h*0.45, fill=1, stroke=0)
    # Gold accent bar
    canvas.setFillColor(C_GOLD)
    canvas.rect(0, h*0.545, w, 5, fill=1, stroke=0)
    # Title
    canvas.setFont("Helvetica-Bold", 30)
    canvas.setFillColor(C_WHITE)
    canvas.drawCentredString(w/2, h*0.72, "DSA Master Engine")
    canvas.setFont("Helvetica", 15)
    canvas.setFillColor(colors.HexColor("#aaaaaa"))
    canvas.drawCentredString(w/2, h*0.66, "Complete Project Documentation")
    # Gold line
    canvas.setStrokeColor(C_GOLD)
    canvas.setLineWidth(2)
    canvas.line(w*0.2, h*0.63, w*0.8, h*0.63)
    # Subtitle
    canvas.setFont("Helvetica", 11)
    canvas.setFillColor(C_GOLD)
    canvas.drawCentredString(w/2, h*0.59, "Advanced C++ System Design  |  FAANG-Level DSA Project")
    # Info box
    canvas.setFillColor(colors.HexColor("#0d1f3c"))
    canvas.roundRect(w*0.1, h*0.30, w*0.8, h*0.22, 10, fill=1, stroke=0)
    info = [
        ("Language",     "C++17"),
        ("Architecture", "Layered  (Config -> Core -> DS -> Algo -> Simulations)"),
        ("Data Structs", "20+ implementations"),
        ("Algorithms",   "30+ algorithms across 8 categories"),
        ("Simulations",  "Search Engine, Job Recommender, Traffic Router, Analytics"),
        ("Build System", "CMake 3.16+  |  MSVC / GCC / Clang"),
    ]
    canvas.setFont("Helvetica-Bold", 9)
    canvas.setFillColor(C_GOLD)
    canvas.drawString(w*0.15, h*0.50, "Project At a Glance")
    canvas.setFont("Helvetica", 9)
    y = h*0.46
    for label, val in info:
        canvas.setFillColor(colors.HexColor("#888888"))
        canvas.drawString(w*0.15, y, label + ":")
        canvas.setFillColor(C_WHITE)
        canvas.drawString(w*0.42, y, val)
        y -= 0.032*h
    # Footer
    canvas.setFont("Helvetica", 8)
    canvas.setFillColor(colors.HexColor("#555555"))
    canvas.drawCentredString(w/2, h*0.06, "DSA Master Engine  —  Engineering-First Learning  —  2026")
    canvas.drawCentredString(w/2, h*0.04, "Built with mentorship from FAANG-level engineering principles")
    canvas.restoreState()

def later_pages(canvas, doc):
    canvas.saveState()
    w, h = A4
    canvas.setFillColor(C_BLUE)
    canvas.rect(0, h-1.1*cm, w, 1.1*cm, fill=1, stroke=0)
    canvas.setFillColor(C_GOLD)
    canvas.rect(0, h-1.15*cm, w, 0.05*cm, fill=1, stroke=0)
    canvas.setFont("Helvetica-Bold", 9)
    canvas.setFillColor(C_WHITE)
    canvas.drawString(1.5*cm, h-0.75*cm, "DSA Master Engine")
    canvas.setFont("Helvetica", 8)
    canvas.drawRightString(w-1.5*cm, h-0.75*cm, "Complete Project Documentation")
    canvas.setFillColor(colors.HexColor("#cccccc"))
    canvas.setFont("Helvetica", 8)
    canvas.drawCentredString(w/2, 0.6*cm, f"Page {doc.page}")
    canvas.setStrokeColor(colors.HexColor("#dddddd"))
    canvas.setLineWidth(0.5)
    canvas.line(1.5*cm, 0.9*cm, w-1.5*cm, 0.9*cm)
    canvas.restoreState()

# ── Document ─────────────────────────────────────────────────────────────────
OUT = "D:/DME/DSA_Master_Engine_Documentation.pdf"
doc = SimpleDocTemplate(
    OUT, pagesize=A4,
    leftMargin=2*cm, rightMargin=2*cm,
    topMargin=2.5*cm, bottomMargin=2*cm,
    title="DSA Master Engine — Complete Documentation",
    author="DSA Master Engine Project",
)

story = []

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 1 — PROJECT OVERVIEW
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("1. Project Overview")

story += quote(
    "I don't solve DSA problems. I design systems that naturally require DSA."
)

story += body(
    "DSA Master Engine is a long-term, production-quality C++ project designed to teach "
    "Data Structures and Algorithms through the lens of real system design — not through "
    "isolated LeetCode-style exercises. Every data structure, every algorithm, and every "
    "core system in this project exists because a real simulation demands it."
)

story += h2("1.1  What This Project Is NOT")
story += bullet([
    "A collection of isolated algorithm implementations",
    "An interview-prep cheat sheet",
    "A college assignment with print statements",
    "A clone of std::vector or std::map",
])

story += h2("1.2  What This Project IS")
story += bullet([
    "A layered software system built like a production FAANG codebase",
    "A learning vehicle that forces deep understanding of memory, ownership, and tradeoffs",
    "A portfolio piece discussable at Google / Meta / Amazon / Apple / Netflix / Microsoft interviews",
    "A project where every DSA concept is motivated by a real engineering need",
])

story += spacer(10)
story += quote(
    "Before writing a single line of code, a senior engineer asks: "
    "What is the purpose? What invariants must hold? Who owns this memory? "
    "What is the complexity? What is the tradeoff?"
)

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 2 — ARCHITECTURE
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("2. System Architecture")

story += body(
    "The project follows a strict layered architecture. Each layer depends only on the "
    "layer below it. Simulations never reach into core internals directly — they use "
    "algorithms and data structures. Algorithms never know about simulations. "
    "This separation makes every component independently testable and replaceable."
)

arch_data = [
    ["Layer",         "Directory",      "Purpose",                          "Depends On"],
    ["Simulations",   "simulations/",   "Real-world problem domains",       "Algorithms, Data Structures, Core"],
    ["Algorithms",    "algorithms/",    "Pure algorithmic logic",           "Data Structures"],
    ["Data Structs",  "data_structures/","Storage and organization",        "Standard Library only"],
    ["Core Systems",  "core/",          "Infrastructure: cache, logger...", "Config, Standard Library"],
    ["Config",        "config/",        "Constants and system settings",    "Standard Library only"],
]
story += table(
    arch_data[0], arch_data[1:],
    col_widths=[2.5*cm, 3.5*cm, 6*cm, 4.5*cm],
    center_cols=[0,1]
)

story += h2("2.1  Dependency Flow")
story += code(
    "  main.cpp\n"
    "     |\n"
    "     v\n"
    "  Simulations   (search_engine, job_recommender, traffic_router, analytics)\n"
    "     |\n"
    "     v\n"
    "  Algorithms    (sorting, searching, graph, DP, backtracking, string...)\n"
    "     |\n"
    "     v\n"
    "  Data Structs  (array, list, stack, queue, tree, graph, trie, hashmap...)\n"
    "     |\n"
    "     v\n"
    "  Core          (logger, cache, scheduler, rate limiter, load balancer)\n"
    "     |\n"
    "     v\n"
    "  Config        (constants.h, system_config.h)"
)

story += h2("2.2  Engineering Principles")
story += bullet([
    "Invariant-first design: every class defines what must always be true before writing a single method",
    "Ownership clarity: every heap allocation has exactly one owner responsible for freeing it",
    "Complexity awareness: every operation's time and space complexity is known and justified",
    "No premature abstraction: functionality is added only when a real simulation demands it",
    "Const-correctness: read-only methods are marked const throughout",
    "Template discipline: generic containers use templates; infrastructure uses concrete types",
])

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 3 — PROJECT STRUCTURE
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("3. Complete File Structure")

story += code(
    "DSA-Master-Engine/\n"
    "|\n"
    "+-- CMakeLists.txt            # Build system (C++17, MSVC + GCC compatible)\n"
    "+-- main.cpp                  # Entry point, demos all layers\n"
    "+-- generate_doc.py           # This documentation generator\n"
    "|\n"
    "+-- config/\n"
    "|   +-- constants.h           # System-wide compile-time constants\n"
    "|   +-- system_config.h       # SystemConfig struct + singleton accessor\n"
    "|\n"
    "+-- core/\n"
    "|   +-- logger/\n"
    "|   |   +-- logger.h/.cpp     # Thread-safe singleton logger, LOG_* macros\n"
    "|   +-- memory_manager/\n"
    "|   |   +-- lru_cache.h/.cpp  # LRU cache (doubly-linked list + hashmap)\n"
    "|   |   +-- lfu_cache.h/.cpp  # LFU cache (frequency map + per-freq lists)\n"
    "|   +-- rate_limiter/\n"
    "|   |   +-- token_bucket.h/.cpp  # Token bucket rate limiter\n"
    "|   +-- load_balancer/\n"
    "|   |   +-- server.h          # Server struct\n"
    "|   |   +-- round_robin.h/.cpp\n"
    "|   |   +-- least_connection.h/.cpp\n"
    "|   +-- schedular/\n"
    "|       +-- task.h            # Task struct with priority and work function\n"
    "|       +-- task_schedular.h/.cpp  # Max-heap priority scheduler\n"
    "|\n"
    "+-- data_structures/\n"
    "|   +-- array/dynamic_array.h\n"
    "|   +-- linked_list/singly_ll.h, doubly_ll.h, circular_ll.h\n"
    "|   +-- stack/stack_array.h\n"
    "|   +-- queue/simple_queue.h, circular_queue.h, deque.h\n"
    "|   +-- hashmap/hashmap_chaining.h, hashmap_open_addressing.h\n"
    "|   +-- tree/binary_tree.h, bst.h, avl_tree.h, red_black_tree.h\n"
    "|   +-- tree/heap.h, segment_tree.h, fenwick_tree.h\n"
    "|   +-- trie/trie.h\n"
    "|   +-- graph/graph.h, disjoint_set.h\n"
    "|\n"
    "+-- algorithms/\n"
    "|   +-- sorting/      bubble, insertion, selection, merge, quick,\n"
    "|   |                 heap, counting, radix\n"
    "|   +-- searching/    linear, binary, exponential\n"
    "|   +-- greedy/       activity_selection, huffman_coding\n"
    "|   +-- divide_conquer/ merge_sort_dc, max_subarray_sum\n"
    "|   +-- bactracking/  n_queens, sudoku_solver\n"
    "|   +-- dynamic_programming/ knapsack, lcs, lis, matrix_chain\n"
    "|   +-- string_algorithms/   kmp, rabin_karp, z_algorithm, suffix_array\n"
    "|   +-- graph_algorithms/    bfs, dfs, dijkstra, bellman_ford,\n"
    "|                            floyd_warshall, kruskal, prim, topo_sort\n"
    "|\n"
    "+-- simulations/\n"
    "    +-- search_engine/   search_engine.h/.cpp\n"
    "    +-- job_recommender/ job_matcher.h/.cpp, recommendation_engine.h/.cpp\n"
    "    +-- traffic_router/  traffic_router.h/.cpp\n"
    "    +-- analytics/       sliding_window_metrics.h/.cpp"
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 4 — DATA STRUCTURES
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("4. Data Structures")

story += body(
    "All data structures are implemented as header-only C++ templates "
    "(except FenwickTree and Trie which are non-template). "
    "Each is designed with explicit invariants that every method must maintain."
)

# 4.1 DynamicArray
story += h2("4.1  DynamicArray<T>")
story += body(
    "A heap-allocated, contiguous-memory, growable sequence container — "
    "the foundation of the project and the first data structure taught. "
    "It is NOT a std::vector clone; it exists to teach memory management, "
    "ownership, amortized analysis, and object lifetime."
)
story += h3("Internal State")
story += code(
    "T*     data_;      // owning raw pointer to heap allocation\n"
    "size_t size_;      // number of live elements\n"
    "size_t capacity_;  // number of allocated slots"
)
story += h3("Invariants")
story += bullet([
    "size_ <= capacity_  (always)",
    "capacity_ == 0  =>  data_ == nullptr",
    "capacity_ > 0   =>  data_ != nullptr",
])
story += table(
    ["Operation", "Complexity", "Reason"],
    [
        ["push_back",  "Amortized O(1)", "Geometric growth (x2): total copies over N pushes = 2N"],
        ["pop_back",   "O(1)",           "Just decrements size_. Allocation untouched."],
        ["operator[]", "O(1)",           "Direct pointer arithmetic: base + index * sizeof(T)"],
        ["at()",       "O(1)",           "Bounds check + pointer arithmetic. Throws out_of_range."],
        ["reserve()",  "O(n)",           "Allocate new block, copy n elements, free old block."],
        ["clear()",    "O(1)",           "Sets size_ = 0. Keeps allocation alive for reuse."],
    ],
    col_widths=[3.5*cm, 4*cm, 9*cm], center_cols=[0,1]
)
story += h3("Key Design Tradeoff")
story += bullet([
    "operator[] — no bounds check. Used in tight loops (e.g. ranking engine). Performance over safety.",
    "at()        — bounds check, throws. Used at system boundaries (e.g. user-provided indices).",
    "Growth by x2 not x1: at 1M elements, x1 costs O(N^2) copies total; x2 costs O(N) total.",
])

# 4.2 SinglyLinkedList
story += h2("4.2  SinglyLinkedList<T>")
story += body(
    "A chain of heap-allocated nodes connected through pointers. Teaches "
    "the opposite mental model from DynamicArray: non-contiguous memory, "
    "O(1) insert anywhere (with pointer), O(n) index access. "
    "Used as the task queue in the Task Scheduler."
)
story += h3("Node Structure")
story += code(
    "struct Node {\n"
    "    T     data;\n"
    "    Node* next;    // address of next node, nullptr if last\n"
    "};"
)
story += h3("Invariants")
story += bullet([
    "head_ == nullptr  =>  tail_ == nullptr  =>  size_ == 0",
    "size_ == 1        =>  head_ == tail_",
    "tail_->next       == nullptr  (always)",
])
story += table(
    ["Operation",       "Complexity", "Notes"],
    [
        ["insert_head",      "O(1)", "Rewire one pointer + update head_"],
        ["insert_tail",      "O(1)", "tail_ gives direct access"],
        ["delete_head",      "O(1)", "Rewire head_, handle empty result"],
        ["delete_tail",      "O(n)", "Must walk to find second-to-last — singly list limitation"],
        ["delete_by_value",  "O(n)", "Walk with prev/current two-pointer pattern"],
        ["search",           "O(n)", "No index; must follow pointer chain"],
    ],
    col_widths=[4*cm, 3*cm, 9.5*cm], center_cols=[0,1]
)
story += body(
    "The O(n) delete_tail is the fundamental limitation of a singly linked list — "
    "you cannot go backwards without a prev pointer. This directly motivates DoublyLinkedList."
)

# 4.3 DoublyLinkedList
story += h2("4.3  DoublyLinkedList<T>")
story += body(
    "Extends singly linked list with a prev pointer per node. "
    "This single addition makes delete_tail O(1) and enables the move_to_front "
    "operation critical for LRU cache. Tradeoff: 8 extra bytes per node on 64-bit systems."
)
story += bullet([
    "Used internally by LRU Cache for O(1) eviction of least-recently-used entry",
    "move_to_front(node*) — rewires prev/next in O(1), used on every cache hit",
    "delete_tail() — O(1) via tail_->prev, the key upgrade over singly linked list",
])

# 4.4 Stack
story += h2("4.4  Stack<T>")
story += body(
    "Built on top of DynamicArray<T>. Demonstrates the abstraction layer: "
    "the user sees push/pop/top with no awareness of the underlying heap management. "
    "This is the stack used for DFS traversal in graph algorithms."
)
story += table(
    ["Method", "Delegates To", "Complexity"],
    [
        ["push(val)", "data_.push_back(val)", "Amortized O(1)"],
        ["pop()",     "data_.pop_back()",     "O(1)"],
        ["top()",     "data_[size-1]",        "O(1)"],
    ],
    col_widths=[4*cm, 5*cm, 4*cm], center_cols=[0,2]
)

# 4.5 Queue variants
story += h2("4.5  Queue Variants")
story += bullet([
    "SimpleQueue<T>  — std::deque backed, unbounded, used in BFS traversal",
    "CircularQueue<T> — fixed-capacity ring buffer, O(1) enqueue/dequeue, wrap-around index arithmetic",
    "Deque<T>        — growable ring buffer with push/pop at both ends, used in sliding window analytics",
])

# 4.6 HashMaps
story += h2("4.6  HashMap (Two Implementations)")
story += body(
    "Both implementations map keys to values using hash functions. "
    "They differ in collision resolution strategy — a fundamental tradeoff in systems design."
)
story += table(
    ["",                    "HashMapChaining",              "HashMapOpenAddressing"],
    [
        ["Collision handling", "Linked list per bucket",    "Linear probing + tombstones"],
        ["Memory layout",      "Non-contiguous (pointers)", "Contiguous array"],
        ["Cache performance",  "Poor (pointer chasing)",    "Good (sequential access)"],
        ["Load factor",        "Can exceed 1.0",            "Must stay below 0.5"],
        ["Deletion",           "Remove from list O(1)",     "Mark as DELETED tombstone"],
        ["Used when",          "Simple, flexible",          "Performance-critical paths"],
    ],
    col_widths=[4*cm, 6.5*cm, 6*cm], center_cols=[0]
)

story.append(PageBreak())

# 4.7 Trees
story += h2("4.7  Tree Data Structures")

story += h3("BST (Binary Search Tree)")
story += body(
    "Maintains the BST invariant: left subtree < node < right subtree. "
    "Average case O(log n) for insert/search/delete. "
    "Worst case O(n) on sorted input — motivates AVL tree."
)

story += h3("AVL Tree")
story += body(
    "Self-balancing BST. Maintains balance factor = height(left) - height(right) in {-1, 0, 1}. "
    "Performs single or double rotations on insert to restore balance. "
    "Guarantees O(log n) worst case for all operations."
)
story += table(
    ["Rotation Type", "When Triggered",        "Result"],
    [
        ["Right rotate",       "Left-heavy (balance > 1, left child balanced/left-heavy)", "Reduces left subtree height"],
        ["Left rotate",        "Right-heavy (balance < -1, right child balanced/right-heavy)","Reduces right subtree height"],
        ["Left-Right rotate",  "Left-heavy, left child is right-heavy",                    "Double rotation to fix"],
        ["Right-Left rotate",  "Right-heavy, right child is left-heavy",                   "Double rotation to fix"],
    ],
    col_widths=[4*cm, 8*cm, 4.5*cm]
)

story += h3("Red-Black Tree")
story += body(
    "A self-balancing BST with 5 invariants: nodes are red or black, "
    "root is black, red nodes have black children, all paths from a node "
    "to leaves have equal black-height, leaves (NIL sentinels) are black. "
    "Less strictly balanced than AVL — O(log n) guaranteed but with fewer rotations on insert."
)

story += h3("Heap (MaxHeap / MinHeap)")
story += body(
    "A complete binary tree stored in an array. Parent is always >= children (max-heap). "
    "Enables O(1) access to maximum element and O(log n) insert/delete. "
    "Used as the priority queue backbone in TaskScheduler and Dijkstra's algorithm."
)
story += code(
    "// Parent-child index relationships in array representation:\n"
    "parent(i) = (i - 1) / 2\n"
    "left(i)   = 2 * i + 1\n"
    "right(i)  = 2 * i + 2"
)

story += h3("Segment Tree")
story += body(
    "Enables O(log n) range queries (sum, min, max, GCD) and O(log n) point updates "
    "on an array. Parameterized by a combine function and identity element — "
    "the same tree structure handles any associative operation. "
    "Used in analytics for range aggregation over time windows."
)

story += h3("Fenwick Tree (Binary Indexed Tree)")
story += body(
    "A space-efficient alternative to segment tree for prefix sum queries. "
    "O(log n) update, O(log n) prefix sum using bit manipulation. "
    "Used in the analytics layer for cumulative metric tracking."
)

story += h3("Trie")
story += body(
    "A tree where each node represents a character. Enables O(m) insert/search "
    "(m = word length), prefix queries, and autocomplete in O(m + results). "
    "Used in the Search Engine for fast prefix-based document lookup."
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 5 — ALGORITHMS
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("5. Algorithms")

story += body(
    "All algorithms are implemented as standalone functions in .cpp files with "
    "corresponding .h declaration files. No algorithm file contains a main() function. "
    "Each algorithm is motivated by a real need within the simulations."
)

# 5.1 Sorting
story += h2("5.1  Sorting Algorithms")
story += table(
    ["Algorithm",      "Best",      "Average",   "Worst",    "Space",  "Stable", "When to Use"],
    [
        ["Bubble Sort",    "O(n)",    "O(n^2)",  "O(n^2)",  "O(1)",   "Yes", "Teaching only — never production"],
        ["Insertion Sort", "O(n)",    "O(n^2)",  "O(n^2)",  "O(1)",   "Yes", "Nearly-sorted data, small n"],
        ["Selection Sort", "O(n^2)",  "O(n^2)",  "O(n^2)",  "O(1)",   "No",  "When writes are expensive"],
        ["Merge Sort",     "O(n logn)","O(n logn)","O(n logn)","O(n)", "Yes", "Stable sort, linked lists"],
        ["Quick Sort",     "O(n logn)","O(n logn)","O(n^2)",  "O(logn)","No",  "General purpose, best cache behaviour"],
        ["Heap Sort",      "O(n logn)","O(n logn)","O(n logn)","O(1)", "No",  "When guaranteed O(n logn) + O(1) space"],
        ["Counting Sort",  "O(n+k)",  "O(n+k)",  "O(n+k)",  "O(k)",   "Yes", "Integer keys with small range k"],
        ["Radix Sort",     "O(nk)",   "O(nk)",   "O(nk)",   "O(n+k)", "Yes", "Large integers, fixed-length strings"],
    ],
    col_widths=[3.2*cm, 2.2*cm, 2.6*cm, 2.2*cm, 1.8*cm, 1.6*cm, 3*cm],
    center_cols=[1,2,3,4,5]
)

story += h3("Project Connection: Search Engine Ranking")
story += body(
    "When the Search Engine returns results, they are sorted by relevance score "
    "using std::sort (introsort — hybrid of quicksort + heapsort + insertion sort). "
    "Understanding when to use each sorting algorithm is what allows you to "
    "choose the right tool when implementing a custom ranking pipeline."
)

# 5.2 Searching
story += h2("5.2  Searching Algorithms")
story += table(
    ["Algorithm",         "Complexity",  "Requirement",        "Project Use"],
    [
        ["Linear Search",     "O(n)",    "Unsorted array",    "Fallback search in small datasets"],
        ["Binary Search",     "O(log n)", "Sorted array",     "Searching sorted job listings by salary"],
        ["Exponential Search","O(log n)", "Sorted array",     "Unknown-size sorted stream searching"],
    ],
    col_widths=[4.5*cm, 3*cm, 4.5*cm, 4.5*cm], center_cols=[1]
)

# 5.3 Graph Algorithms
story += h2("5.3  Graph Algorithms")
story += table(
    ["Algorithm",          "Complexity",         "Purpose in Project"],
    [
        ["BFS",               "O(V + E)",          "Level-order traversal, shortest hops in unweighted graph"],
        ["DFS",               "O(V + E)",          "Cycle detection, connectivity, topological ordering"],
        ["Dijkstra",          "O((V+E) log V)",    "Min-latency routing in TrafficRouter simulation"],
        ["Bellman-Ford",      "O(V * E)",          "Negative-weight edge detection in routing"],
        ["Floyd-Warshall",    "O(V^3)",            "All-pairs shortest paths for small routing tables"],
        ["Kruskal (MST)",     "O(E log E)",        "Minimum spanning tree using DisjointSet union-find"],
        ["Prim (MST)",        "O(E log V)",        "Alternative MST — better for dense graphs"],
        ["Topological Sort",  "O(V + E)",          "Task dependency ordering in Scheduler"],
    ],
    col_widths=[4*cm, 4*cm, 8.5*cm], center_cols=[0,1]
)

story += h3("Project Connection: Traffic Router")
story += body(
    "The TrafficRouter simulation uses Dijkstra's algorithm to find the minimum-latency "
    "path between network nodes. The Graph data structure (adjacency list) backs the "
    "router's network topology. Kruskal's algorithm with DisjointSet union-find is used "
    "to compute the minimum spanning tree — the backbone of network design."
)

# 5.4 Dynamic Programming
story += h2("5.4  Dynamic Programming")
story += table(
    ["Problem",          "Technique",         "Complexity",     "Project Application"],
    [
        ["0/1 Knapsack",   "Bottom-up 2D DP",   "O(n*W)",        "Resource allocation in Scheduler"],
        ["LCS",            "Bottom-up 2D DP",   "O(m*n)",        "Document similarity in Search Engine"],
        ["LIS",            "Patience sorting",  "O(n log n)",    "Version sequence detection"],
        ["Matrix Chain",   "Interval DP",       "O(n^3)",        "Query plan optimization"],
    ],
    col_widths=[3.5*cm, 3.5*cm, 3*cm, 6.5*cm], center_cols=[2]
)

# 5.5 Backtracking
story += h2("5.5  Backtracking")
story += table(
    ["Problem",     "Approach",                               "Complexity"],
    [
        ["N-Queens",  "Place queen row by row; backtrack on conflict", "O(n!)"],
        ["Sudoku",    "Fill empty cells 1-9; backtrack on invalid",    "O(9^(n*n))"],
    ],
    col_widths=[3.5*cm, 9.5*cm, 3.5*cm], center_cols=[2]
)

# 5.6 String Algorithms
story += h2("5.6  String Algorithms")
story += table(
    ["Algorithm",      "Preprocessing", "Search",   "Key Idea"],
    [
        ["KMP",          "O(m)",         "O(n)",    "Failure function avoids re-examining matched characters"],
        ["Rabin-Karp",   "O(m)",         "O(n)",    "Rolling hash; compare hash before string comparison"],
        ["Z-Algorithm",  "O(n)",         "O(n)",    "Z-array: longest prefix match starting at each position"],
        ["Suffix Array", "O(n log n)",   "O(m logn)","Sorted array of all suffixes enables binary search"],
    ],
    col_widths=[3.5*cm, 3*cm, 2.5*cm, 7.5*cm], center_cols=[1,2]
)

story += h3("Project Connection: Search Engine")
story += body(
    "KMP is used in the Search Engine for exact keyword matching within documents. "
    "Rabin-Karp provides rolling-hash multi-pattern search. "
    "The Trie structure handles prefix queries and autocomplete. "
    "Together these form a complete text search pipeline."
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 6 — CORE SYSTEMS
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("6. Core Systems")

story += body(
    "Core systems are infrastructure components that every serious production "
    "system needs before any business logic. The mentor principle: "
    "every real system begins with configuration, visibility (logging), and diagnostics."
)

# 6.1 Logger
story += h2("6.1  Logger")
story += body(
    "A thread-safe singleton logger with four log levels: DEBUG, INFO, WARN, ERROR. "
    "Uses std::mutex to prevent interleaved output from concurrent systems. "
    "Provides convenience macros LOG_INFO(), LOG_WARN(), LOG_ERROR(), LOG_DEBUG()."
)
story += code(
    "// Usage:\n"
    "LOG_INFO(\"Scheduler: submitted task [\" + task.name + \"]\");\n"
    "LOG_ERROR(\"Rate limiter: request denied — bucket empty\");"
)
story += bullet([
    "Singleton pattern — one logger instance for the entire system",
    "Level filtering — DEBUG messages suppressed in production",
    "Optional file output — appends to log file if enabled",
    "Thread-safe — std::lock_guard on every write operation",
])

# 6.2 LRU Cache
story += h2("6.2  LRU Cache")
story += body(
    "Caches the most recently used items and evicts the least recently used "
    "when capacity is exceeded. Used in the Search Engine to cache query results "
    "so repeated queries don't re-scan the inverted index."
)
story += h3("Data Structure Combination")
story += code(
    "std::list<pair<K,V>>                          // doubly linked list\n"
    "    front = most recent  |  back = LRU (evict target)\n"
    "\n"
    "std::unordered_map<K, list::iterator>         // O(1) lookup\n"
    "    key -> pointer into the list"
)
story += table(
    ["Operation", "Complexity", "How"],
    [
        ["get(key)",      "O(1)", "hashmap lookup + splice node to front"],
        ["put(key,val)",  "O(1)", "hashmap insert + list push_front; evict back if full"],
        ["evict()",       "O(1)", "erase list.back(), remove from hashmap"],
    ],
    col_widths=[3.5*cm, 2.5*cm, 10.5*cm], center_cols=[0,1]
)

# 6.3 LFU Cache
story += h2("6.3  LFU Cache")
story += body(
    "Evicts the least frequently used item. When multiple items share minimum "
    "frequency, evicts the least recently used among them. Used for hot content caching "
    "where access frequency determines retention priority."
)
story += bullet([
    "key_map: key -> list iterator (O(1) access to entry)",
    "freq_map: frequency -> doubly linked list of entries at that frequency",
    "min_freq: tracks the current minimum frequency for O(1) eviction",
    "On access: move entry to freq+1 list; update min_freq if freq list becomes empty",
])

# 6.4 Rate Limiter
story += h2("6.4  Token Bucket Rate Limiter")
story += body(
    "Limits API request rate. Tokens accumulate at a fixed rate (e.g. 10/sec) "
    "up to a maximum capacity. Each request consumes one token. "
    "Burst requests are allowed up to bucket capacity; sustained rate is capped."
)
story += code(
    "TokenBucket limiter(10.0, 50);   // 10 tokens/sec, capacity 50\n"
    "\n"
    "if (limiter.consume(1)) {\n"
    "    handle_request();            // allowed\n"
    "} else {\n"
    "    return 429_TOO_MANY_REQUESTS; // denied\n"
    "}"
)

# 6.5 Load Balancer
story += h2("6.5  Load Balancer")
story += table(
    ["Strategy",          "Algorithm",                      "Best For",                     "Limitation"],
    [
        ["Round Robin",     "Cycle through servers in order", "Equal-cost homogeneous servers","Ignores actual load"],
        ["Least Connection","Route to server with fewest active connections","Variable request cost","Slightly more overhead"],
    ],
    col_widths=[3.5*cm, 5*cm, 4.5*cm, 4*cm]
)

# 6.6 Task Scheduler
story += h2("6.6  Task Scheduler")
story += body(
    "A priority-based task executor. Tasks are submitted with an integer priority. "
    "The scheduler always runs the highest-priority pending task next (max-heap). "
    "Supports cancellation via a cancellation map — cancelled tasks are skipped at dequeue time."
)
story += code(
    "scheduler.submit(Task(1, \"Compress logs\",    priority=2, work));\n"
    "scheduler.submit(Task(2, \"Index documents\",  priority=5, work));\n"
    "scheduler.submit(Task(3, \"ALERT: disk full\", priority=9, work));\n"
    "scheduler.cancel(1);    // cancel task 1\n"
    "scheduler.run_all();    // runs in order: 9, 5  (task 1 skipped)"
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 7 — SIMULATIONS
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("7. Simulations")

story += body(
    "Simulations are the top layer of the project — the real-world problem domains "
    "that justify every data structure and algorithm below them. "
    "A simulation does not implement algorithms directly; it orchestrates the layers beneath it."
)

# 7.1 Search Engine
story += h2("7.1  Search Engine")
story += body(
    "A document indexing and retrieval system with TF-inspired relevance scoring."
)
story += table(
    ["Component",        "Data Structure / Algorithm",  "Purpose"],
    [
        ["Inverted Index", "HashMapChaining<string, vector<int>>", "Map term -> list of document IDs"],
        ["Document Store", "unordered_map<int, Document>",         "O(1) document retrieval by ID"],
        ["Tokenizer",      "string parsing",                       "Normalize text to lowercase terms"],
        ["Ranking",        "std::sort on relevance score",         "Sort results by TF + title-match bonus"],
        ["Query Cache",    "LRUCache<string, vector<Document>>",   "Cache repeated query results"],
        ["Autocomplete",   "Trie",                                 "Prefix-based term suggestions"],
    ],
    col_widths=[3.5*cm, 6.5*cm, 6.5*cm]
)

# 7.2 Job Recommender
story += h2("7.2  Job Recommender")
story += body(
    "Matches job listings to candidates based on skill overlap and salary requirements."
)
story += bullet([
    "Candidate provides: skill set, minimum salary requirement",
    "Each job has: required skills, offered salary, company",
    "Match score = |candidate_skills ∩ job_skills| / |job_skills|  (Jaccard-like coefficient)",
    "Results filtered by salary >= min_salary, sorted by match score descending",
    "8 hardcoded FAANG/top-tech jobs pre-loaded (Google, Meta, Amazon, OpenAI, Apple, Netflix, Microsoft, Cloudflare)",
])

# 7.3 Traffic Router
story += h2("7.3  Traffic Router")
story += body(
    "Models a network topology as a weighted undirected graph. "
    "Finds the minimum-latency path between any two nodes using Dijkstra's algorithm "
    "with a priority queue (min-heap). Reconstructs the full path via parent array."
)
story += code(
    "TrafficRouter router(6);          // 6 network nodes\n"
    "router.add_link(0, 2,  3);        // Node0 <-> Node2: 3ms\n"
    "router.add_link(2, 4,  2);        // Node2 <-> Node4: 2ms\n"
    "router.add_link(4, 5,  1);        // Node4 <-> Node5: 1ms\n"
    "\n"
    "auto route = router.find_best_route(0, 5);\n"
    "// Path: Node0 -> Node2 -> Node4 -> Node5\n"
    "// Latency: 6ms"
)

# 7.4 Analytics
story += h2("7.4  Analytics — Sliding Window Metrics")
story += body(
    "Tracks real-time system metrics (latency, throughput, error rate) "
    "over a configurable time window using a deque-backed sliding window."
)
story += bullet([
    "Deque maintains events in chronological order — front = oldest, back = newest",
    "evict(current_time) — pops from front while oldest event is outside the window",
    "average(), max(), min(), count() — computed over the current window contents",
    "Used to track search engine query latency and recommendation engine response times",
])

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 8 — CONFIG SYSTEM
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("8. Configuration System")

story += h2("8.1  constants.h")
story += body(
    "Compile-time constants defined in the DME namespace. "
    "All magic numbers in the system reference these constants rather than raw literals."
)
story += code(
    "namespace DME {\n"
    "    constexpr size_t DEFAULT_CACHE_CAPACITY  = 100;\n"
    "    constexpr double DEFAULT_TOKEN_RATE       = 10.0;  // tokens/sec\n"
    "    constexpr size_t MAX_SERVERS              = 32;\n"
    "    constexpr size_t SLIDING_WINDOW_SIZE      = 60;    // seconds\n"
    "    constexpr size_t MAX_SEARCH_RESULTS       = 20;\n"
    "}"
)

story += h2("8.2  system_config.h")
story += body(
    "A runtime-configurable SystemConfig struct with a singleton accessor. "
    "Subsystems query get_config() to read their operating parameters. "
    "This allows the same binary to run in different configurations "
    "(e.g. larger cache for production, smaller for test)."
)

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 9 — BUILD SYSTEM
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("9. Build System")

story += h2("9.1  CMakeLists.txt")
story += bullet([
    "CMake 3.16+ required",
    "C++17 standard enforced (std::optional, structured bindings, if-constexpr)",
    "MSVC warnings: /W3 /wd4996",
    "GCC/Clang warnings: -Wall -Wextra",
    "Release: /O2 (MSVC) or -O3 (GCC/Clang)",
    "Debug: /Od /Zi (MSVC) or -O0 -g (GCC/Clang)",
    "All .cpp files discovered via file(GLOB_RECURSE) — no manual listing needed",
    "Test executable built only if tests/*.cpp files exist (conditional)",
])

story += h2("9.2  Build Instructions")
story += h3("Visual Studio (Windows — Recommended)")
story += code(
    "# Open 'Developer Command Prompt for VS 2022'\n"
    "cd D:\\DME\n"
    "cmake -B build\n"
    "cmake --build build --config Release\n"
    "build\\Release\\dsa_engine.exe"
)
story += h3("MinGW / GCC (Windows)")
story += code(
    "cd D:\\DME\n"
    "cmake -B build -G \"MinGW Makefiles\" -DCMAKE_BUILD_TYPE=Release\n"
    "cmake --build build\n"
    ".\\build\\dsa_engine.exe"
)
story += h3("GCC (Linux / macOS)")
story += code(
    "cd /path/to/DME\n"
    "cmake -B build -DCMAKE_BUILD_TYPE=Release\n"
    "cmake --build build\n"
    "./build/dsa_engine"
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 10 — ENGINEERING PHILOSOPHY
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("10. Engineering Philosophy")

story += quote(
    "Do not memorize algorithms. Build systems that require algorithms."
)

story += h2("10.1  Invariant-First Design")
story += body(
    "Before implementing any data structure, define the invariants — "
    "the rules that must be true at all times. Every method must maintain them. "
    "Breaking an invariant, even temporarily inside a method, is a bug."
)
story += bullet([
    "DynamicArray: size_ <= capacity_  always",
    "SinglyLinkedList: tail_->next == nullptr  always",
    "AVLTree: |balance_factor| <= 1  at every node  always",
    "Heap: parent >= children (max-heap)  at every node  always",
])

story += h2("10.2  Ownership Model")
story += body(
    "In C++ without a garbage collector, every heap allocation must have "
    "exactly one owner responsible for freeing it. The project teaches three levels:"
)
story += table(
    ["Level",     "Pattern",                        "Example in Project"],
    [
        ["Raw pointer", "Manual new/delete in class",  "DynamicArray::data_, all tree nodes"],
        ["RAII",        "Constructor allocates, destructor frees","Every class with heap state"],
        ["Shared ownership","std::shared_ptr",         "Huffman tree nodes in huffman_coding.cpp"],
    ],
    col_widths=[3*cm, 5.5*cm, 8*cm]
)

story += h2("10.3  Amortized Analysis")
story += body(
    "An operation that is occasionally expensive but cheap on average. "
    "DynamicArray::push_back doubles capacity when full. "
    "Each reallocation copies N elements, but this happens so rarely "
    "that averaged over N pushes the cost is O(1) per operation."
)
story += code(
    "N pushes with capacity doubling:\n"
    "   Copy counts:  1 + 2 + 4 + 8 + ... + N  =  ~2N total copies\n"
    "   Cost per push: 2N / N  =  O(1) amortized\n"
    "\n"
    "N pushes with capacity + 1 each time:\n"
    "   Copy counts:  0 + 1 + 2 + ... + N-1  =  N*(N-1)/2  =  O(N^2) total\n"
    "   Cost per push: O(N)  --  UNACCEPTABLE at scale"
)

story += h2("10.4  The Senior Engineer Mindset")
story += body("For every data structure and algorithm, a senior engineer asks:")
story += table(
    ["Question",              "What It Reveals"],
    [
        ["What is the purpose?",          "Is this component justified? Could something simpler work?"],
        ["What invariants must hold?",    "What assumptions does every caller rely on?"],
        ["Who owns this memory?",         "Which destructor frees this allocation?"],
        ["What is the complexity?",       "Will this scale to 10x, 100x, 1000x the current load?"],
        ["What is the tradeoff?",         "What does this design sacrifice to gain its advantages?"],
        ["What breaks at the boundary?",  "What happens with empty input, max input, duplicate input?"],
    ],
    col_widths=[5.5*cm, 11*cm]
)

story += h2("10.5  Learning Progression")
story += body(
    "The data structures are built in a deliberate order. "
    "Each one teaches a concept that the next one builds upon."
)
story += table(
    ["Order", "Structure",       "Core Concept Taught"],
    [
        ["1",  "DynamicArray",     "Contiguous memory, ownership, amortized O(1), Rule of 5"],
        ["2",  "SinglyLinkedList", "Pointers, non-contiguous memory, structural invariants"],
        ["3",  "Stack",            "Abstraction: hide implementation behind clean interface"],
        ["4",  "SimpleQueue",      "State transitions, FIFO discipline"],
        ["5",  "CircularQueue",    "Index wrap-around, fixed-capacity ring buffer"],
        ["6",  "Deque",            "Bidirectional operations, growable ring buffer"],
        ["7",  "HashMap",          "Hashing, collision resolution, load factor"],
        ["8",  "BST",              "Recursive structure, O(log n) average case"],
        ["9",  "AVL Tree",         "Self-balancing, rotations, O(log n) worst case"],
        ["10", "Heap",             "Complete tree in array, sift operations, priority"],
        ["11", "Trie",             "Character-level indexing, prefix operations"],
        ["12", "Graph",            "Vertex-edge relationships, adjacency representation"],
    ],
    col_widths=[1.5*cm, 4*cm, 11*cm], center_cols=[0]
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 11 — QUICK REFERENCE
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("11. Quick Reference — Complexity Cheat Sheet")

story += h2("11.1  Data Structures")
story += table(
    ["Structure",        "Access", "Search", "Insert", "Delete", "Space"],
    [
        ["DynamicArray",   "O(1)",   "O(n)",  "O(1)*",  "O(n)",  "O(n)"],
        ["SinglyLinkedList","O(n)",  "O(n)",  "O(1)",   "O(n)",  "O(n)"],
        ["DoublyLinkedList","O(n)",  "O(n)",  "O(1)",   "O(1)",  "O(n)"],
        ["Stack",          "O(1)top","O(n)",  "O(1)*",  "O(1)",  "O(n)"],
        ["Queue",          "O(1)frnt","O(n)", "O(1)",   "O(1)",  "O(n)"],
        ["HashMap",        "O(1)*",  "O(1)*", "O(1)*",  "O(1)*", "O(n)"],
        ["BST",            "O(logn)","O(logn)","O(logn)","O(logn)","O(n)"],
        ["AVL Tree",       "O(logn)","O(logn)","O(logn)","O(logn)","O(n)"],
        ["Heap",           "O(1)top","O(n)",  "O(logn)","O(logn)","O(n)"],
        ["Trie",           "O(m)",   "O(m)",  "O(m)",   "O(m)",  "O(n*m)"],
        ["Segment Tree",   "-",      "O(logn)","O(logn)","-",    "O(n)"],
        ["Fenwick Tree",   "-",      "O(logn)","O(logn)","-",    "O(n)"],
    ],
    col_widths=[4*cm, 2.5*cm, 2.5*cm, 2.5*cm, 2.5*cm, 2.5*cm],
    center_cols=[1,2,3,4,5]
)
story += [Paragraph("* = amortized or average case", S_CAPTION)]

story += h2("11.2  Sorting Algorithms")
story += table(
    ["Algorithm",    "Best",     "Average",  "Worst",    "Space"],
    [
        ["Bubble",     "O(n)",    "O(n^2)",  "O(n^2)",  "O(1)"],
        ["Insertion",  "O(n)",    "O(n^2)",  "O(n^2)",  "O(1)"],
        ["Selection",  "O(n^2)",  "O(n^2)",  "O(n^2)",  "O(1)"],
        ["Merge",      "O(nlogn)","O(nlogn)","O(nlogn)","O(n)"],
        ["Quick",      "O(nlogn)","O(nlogn)","O(n^2)",  "O(logn)"],
        ["Heap",       "O(nlogn)","O(nlogn)","O(nlogn)","O(1)"],
        ["Counting",   "O(n+k)",  "O(n+k)",  "O(n+k)",  "O(k)"],
        ["Radix",      "O(nk)",   "O(nk)",   "O(nk)",   "O(n+k)"],
    ],
    col_widths=[3.5*cm, 3*cm, 3*cm, 3*cm, 3*cm],
    center_cols=[1,2,3,4]
)

story += h2("11.3  Graph Algorithms")
story += table(
    ["Algorithm",       "Time",           "Space",  "Notes"],
    [
        ["BFS",           "O(V+E)",        "O(V)",  "Shortest path (unweighted)"],
        ["DFS",           "O(V+E)",        "O(V)",  "Cycle detection, topological sort"],
        ["Dijkstra",      "O((V+E)logV)",  "O(V)",  "Non-negative weights only"],
        ["Bellman-Ford",  "O(VE)",         "O(V)",  "Handles negative weights"],
        ["Floyd-Warshall","O(V^3)",        "O(V^2)","All-pairs shortest paths"],
        ["Kruskal MST",   "O(E logE)",     "O(V)",  "Requires DisjointSet"],
        ["Prim MST",      "O(E logV)",     "O(V)",  "Better for dense graphs"],
        ["Topo Sort",     "O(V+E)",        "O(V)",  "DAG only"],
    ],
    col_widths=[4*cm, 3.5*cm, 2.5*cm, 6.5*cm], center_cols=[1,2]
)

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 12 — GLOSSARY
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("12. Glossary")

glossary = [
    ("Amortized O(1)",    "Average cost per operation over a sequence of N operations is O(1), "
                          "even if some individual operations cost O(N)."),
    ("Invariant",         "A condition that must be true at all times. Violation means a bug."),
    ("RAII",              "Resource Acquisition Is Initialization — constructor acquires resource, "
                          "destructor releases it. Guarantees no leaks."),
    ("Contiguous Memory", "Elements stored at consecutive memory addresses. "
                          "Enables O(1) index access via pointer arithmetic."),
    ("Cache Locality",    "CPU caches work on cache lines (~64 bytes). Contiguous data is loaded "
                          "together, making sequential access fast. Linked lists break locality."),
    ("Load Factor",       "ratio = elements / buckets in a hash table. "
                          "High load factor increases collisions; triggers rehashing."),
    ("Sentinel Node",     "A dummy node (e.g. NIL in Red-Black Tree) that simplifies "
                          "boundary conditions. Eliminates nullptr checks."),
    ("Sift Up/Down",      "Heap operations to restore heap property after insert (sift up) "
                          "or delete (sift down)."),
    ("Balance Factor",    "AVL Tree: height(left) - height(right). Must stay in {-1, 0, 1}."),
    ("Tombstone",         "A marker in open-addressing hash tables indicating a deleted slot. "
                          "Allows search to continue past it without treating it as empty."),
    ("Path Compression",  "DisjointSet optimization: during find(), make every node on the "
                          "path point directly to the root. Amortizes future finds to ~O(1)."),
    ("Union by Rank",     "DisjointSet optimization: always attach smaller tree under larger. "
                          "Keeps tree height at O(log n)."),
    ("Inverted Index",    "Maps each term to the list of documents containing it. "
                          "Core data structure of every search engine."),
    ("Token Bucket",      "Rate limiting algorithm: tokens accumulate at rate R up to capacity C. "
                          "Each request consumes one token. Allows bursts up to C."),
]
for term, defn in glossary:
    story += [Paragraph(f"<b>{term}</b>: {defn}", S_BODY)]

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION 13 — INTERVIEW GUIDE
# ═══════════════════════════════════════════════════════════════════════════════
story += h1("13. FAANG Interview Discussion Guide")

story += body(
    "This section maps the project's components to common FAANG system design "
    "and DSA interview topics. Use this project as evidence of deep understanding "
    "rather than memorized solutions."
)

story += h2("13.1  System Design Topics Covered")
story += table(
    ["Topic",                    "Where in This Project"],
    [
        ["Caching strategies",       "LRU Cache (recency), LFU Cache (frequency) — both implemented from scratch"],
        ["Rate limiting",            "Token Bucket in core/rate_limiter/ — production pattern"],
        ["Load balancing",           "Round Robin + Least Connection in core/load_balancer/"],
        ["Search systems",           "Inverted index, TF scoring, Trie autocomplete in simulations/search_engine/"],
        ["Priority scheduling",      "Max-heap TaskScheduler with cancellation in core/schedular/"],
        ["Graph routing",            "Dijkstra-based TrafficRouter in simulations/traffic_router/"],
        ["Sliding window metrics",   "Deque-backed SlidingWindowMetrics in simulations/analytics/"],
        ["Recommendation systems",   "Skill-match scoring JobRecommender in simulations/job_recommender/"],
    ],
    col_widths=[5*cm, 11.5*cm]
)

story += h2("13.2  DSA Depth Questions You Can Answer")
story += bullet([
    "Why does DynamicArray use capacity *= 2 instead of capacity += 1? (Amortized O(1) vs O(N^2))",
    "Why does delete_tail() cost O(n) in a singly linked list? (No backwards traversal)",
    "Why does LRU Cache need a doubly linked list + hashmap? (O(1) delete + O(1) lookup)",
    "What invariant does AVL Tree maintain and how does it enforce it? (Balance factor via rotations)",
    "Why is Dijkstra's algorithm wrong with negative weights? (Greedy assumption breaks)",
    "What is the difference between Kruskal and Prim for MST? (Edge-sorted vs vertex-grown)",
    "Why does the Trie use unordered_map<char, Node*> instead of a fixed array? (Sparse alphabet)",
    "What is path compression in DisjointSet and why does it matter? (Near-O(1) amortized find)",
    "Why use a Fenwick Tree instead of a Segment Tree for prefix sums? (Half the space, simpler code)",
    "What is the time complexity of KMP and why? (O(n+m) — failure function prevents backtracking)",
])

story += h2("13.3  The Core Message")
story += quote(
    "I built a system where data structures and algorithms are not isolated exercises — "
    "they are requirements. My search engine needs a Trie. My traffic router needs Dijkstra. "
    "My scheduler needs a max-heap. I understand not just how these work, "
    "but WHY they exist and what breaks if you choose the wrong one."
)

# ═══════════════════════════════════════════════════════════════════════════════
# BUILD
# ═══════════════════════════════════════════════════════════════════════════════
doc.build(
    story,
    onFirstPage=cover_page,
    onLaterPages=later_pages,
)

print(f"\nPDF generated: {OUT}")
print(f"Open it at: D:\\DME\\DSA_Master_Engine_Documentation.pdf")
