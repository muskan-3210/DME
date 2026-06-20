#pragma once
#include <vector>
#include <string>

struct Activity { int start, end, id; };
struct HuffmanCode { char ch; std::string code; int freq; };

std::vector<Activity>    activity_selection(std::vector<Activity> activities);
std::vector<HuffmanCode> huffman_coding(const std::vector<std::pair<char,int>>& freq_map);
