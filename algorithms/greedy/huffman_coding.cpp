#include "greedy.h"
#include <queue>
#include <memory>

struct HNode {
    char ch;
    int  freq;
    std::shared_ptr<HNode> left, right;
    HNode(char c, int f) : ch(c), freq(f) {}
};

struct HNodeCmp {
    bool operator()(const std::shared_ptr<HNode>& a, const std::shared_ptr<HNode>& b) {
        return a->freq > b->freq;
    }
};

static void encode(const std::shared_ptr<HNode>& node, const std::string& code,
                   std::vector<HuffmanCode>& result) {
    if (!node) return;
    if (!node->left && !node->right) {
        result.push_back({node->ch, code, node->freq});
        return;
    }
    encode(node->left,  code + "0", result);
    encode(node->right, code + "1", result);
}

std::vector<HuffmanCode> huffman_coding(const std::vector<std::pair<char,int>>& freq_map) {
    std::priority_queue<std::shared_ptr<HNode>, std::vector<std::shared_ptr<HNode>>, HNodeCmp> pq;
    for (auto& p : freq_map) pq.push(std::make_shared<HNode>(p.first, p.second));

    while (pq.size() > 1) {
        auto l = pq.top(); pq.pop();
        auto r = pq.top(); pq.pop();
        auto merged = std::make_shared<HNode>('\0', l->freq + r->freq);
        merged->left  = l;
        merged->right = r;
        pq.push(merged);
    }

    std::vector<HuffmanCode> result;
    if (!pq.empty()) encode(pq.top(), "", result);
    return result;
}
