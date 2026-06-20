#include "search_engine.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace DME {

SearchEngine::SearchEngine() : next_id_(1) {}

void SearchEngine::index_document(const Document& doc) {
    Document d  = doc;
    d.id        = next_id_++;
    documents_[d.id] = d;
    auto tokens = tokenize(d.title + " " + d.content);
    for (const auto& token : tokens)
        inverted_index_[token].push_back(d.id);
}

std::vector<Document> SearchEngine::search(const std::string& query) {
    auto terms = tokenize(query);
    std::unordered_map<int, double> scores;

    for (const auto& term : terms) {
        auto it = inverted_index_.find(term);
        if (it == inverted_index_.end()) continue;
        for (int doc_id : it->second)
            scores[doc_id] += compute_relevance(doc_id, terms);
    }

    std::vector<Document> results;
    for (auto& p : scores) {
        Document d = documents_[p.first];
        d.relevance_score = p.second;
        results.push_back(d);
    }

    std::sort(results.begin(), results.end(),
              [](const Document& a, const Document& b){
                  return a.relevance_score > b.relevance_score;
              });

    return results;
}

size_t SearchEngine::document_count() const {
    return documents_.size();
}

std::vector<std::string> SearchEngine::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream ss(text);
    std::string word;
    while (ss >> word) {
        std::string clean;
        for (char c : word)
            if (std::isalpha(static_cast<unsigned char>(c)))
                clean += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (!clean.empty()) tokens.push_back(clean);
    }
    return tokens;
}

double SearchEngine::compute_relevance(int doc_id, const std::vector<std::string>& terms) const {
    const auto& doc = documents_.at(doc_id);
    double score    = 0.0;
    for (const auto& term : terms) {
        auto it = inverted_index_.find(term);
        if (it == inverted_index_.end()) continue;
        // TF-inspired: more occurrences = higher score
        long count = std::count(it->second.begin(), it->second.end(), doc_id);
        // Title match is worth more
        std::string title_lower = doc.title;
        std::transform(title_lower.begin(), title_lower.end(),
                       title_lower.begin(), [](unsigned char c){ return std::tolower(c); });
        score += static_cast<double>(count);
        if (title_lower.find(term) != std::string::npos) score += 2.0;
    }
    return score;
}

} // namespace DME
