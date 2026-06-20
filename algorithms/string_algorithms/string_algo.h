#pragma once
#include <vector>
#include <string>

std::vector<int> kmp_search      (const std::string& text, const std::string& pattern);
std::vector<int> rabin_karp_search(const std::string& text, const std::string& pattern);
std::vector<int> z_algorithm     (const std::string& s);
std::vector<int> build_suffix_array(const std::string& s);
