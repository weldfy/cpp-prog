#include "ga/Genome.hpp"

#include <iostream>
#include <map>
#include <unordered_map>

namespace genome {

void Dfs(int x, std::vector<std::vector<std::pair<int, int>>>& transitions, std::vector<bool>& used,
         std::vector<int>& euler_path) {
    for (int i = 0; i < int(transitions[x].size()); i++) {
        if (used[transitions[x][i].second] == 0) {
            used[transitions[x][i].second] = 1;
            Dfs(transitions[x][i].first, transitions, used, euler_path);
        }
    }
    euler_path.push_back(x);
}

void reverse(std::vector<int>& array) {
    for (int left = 0, right = array.size() - left - 1; left < right; left++, right--) {
        std::swap(array[left], array[right]);
    }
    return;
}

int getStart(std::vector<std::vector<std::pair<int, int>>>& transitions, std::vector<int>& cntInEdge) {
    int start = 0;
    for (int i = 1; i < int(transitions.size()); i++) {
        if (transitions[i].size() - cntInEdge[i] == 1) {
            start = i;
            break;
        }
    }
    return start;
}

std::string assembly(size_t k, const std::vector<std::string>& fragments) {
    if (fragments.size() == 0 || k == 0) {
        return "";
    }

    int timer = 1;

    std::map<std::pair<int, int>, std::vector<int>> edgeString;
    std::unordered_map<std::string, int> table;

    std::vector<std::vector<std::pair<int, int>>> transitions;
    std::vector<int> cntInEdge;
    cntInEdge.resize(fragments.size() * 2 + 10);
    transitions.resize(fragments.size() * 2 + 10);

    for (int i = 0; i < int(fragments.size()); i++) {
        std::string left  = fragments[i].substr(0, k);
        std::string right = fragments[i].substr(int(fragments[i].size()) - k);
        int leftNumber    = table[left];
        int rightNumber   = table[right];
        if (leftNumber == 0) {
            leftNumber = timer++;
        }
        if (left == right) {
            rightNumber = leftNumber;
        } else {
            if (rightNumber == 0) {
                rightNumber = timer++;
            }
        }

        transitions[leftNumber].push_back({rightNumber, i});
        cntInEdge[rightNumber]++;
        edgeString[{leftNumber, rightNumber}].push_back(i);
        table[left]  = leftNumber;
        table[right] = rightNumber;
    }
    std::vector<bool> used;
    std::vector<int> euler_path;
    used.resize(fragments.size() * 2 + 10);
    Dfs(getStart(transitions, cntInEdge), transitions, used, euler_path);
    reverse(euler_path);
    std::string K_concatenation = "";
    for (int i = 1; i < int(euler_path.size()); i++) {
        if (K_concatenation == "")
            K_concatenation += fragments[edgeString[{euler_path[i - 1], euler_path[i]}].back()];
        else {
            K_concatenation += fragments[edgeString[{euler_path[i - 1], euler_path[i]}].back()].substr(k);
        }
        edgeString[{euler_path[i - 1], euler_path[i]}].pop_back();
    }
    return K_concatenation;
}

}  // namespace genome
