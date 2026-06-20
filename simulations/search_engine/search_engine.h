#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace DME {

struct Document {
    int         id;
    std::string title;
    std::string content;
    double      relevance_score;
};

class SearchEngine {
public:
    SearchEngine();
    void   index_document(const Document& doc);
    std::vector<Document> search(const std::string& query);
    size_t document_count() const;

private:
    std::unordered_map<std::string, std::vector<int>> inverted_index_;
    std::unordered_map<int, Document>                 documents_;
    int                                               next_id_;

    std::vector<std::string> tokenize(const std::string& text) const;
    double compute_relevance(int doc_id, const std::vector<std::string>& terms) const;
};

} // namespace DME
