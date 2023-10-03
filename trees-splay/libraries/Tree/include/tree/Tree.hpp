#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

class Splay {
private:
    std::size_t tree_size = 0;
    struct Node {
        Node* left;
        Node* right;
        Node* parent;
        int value;
        Node(int value, Node* left, Node* right, Node* parent) {
            this->left   = left;
            this->right  = right;
            this->parent = parent;
            this->value  = value;
        }
        ~Node();
    };
    mutable Node* root = nullptr;
    void rotate_left(Node* v) const noexcept;
    void rotate_right(Node* v) const noexcept;
    bool zig_zig(Node* v) const noexcept;
    bool zig_zag(Node* v) const noexcept;
    bool zig(Node* v) const noexcept;
    void splay(Node* v) const noexcept;
    Node* find(int x) const noexcept;
    Node* insert(Node*& v, Node*& pv, int x);
    void dfs(Node* v, std::vector<int>& values) const noexcept;

public:
    bool remove(int x);
    bool insert(int x);
    [[nodiscard]] bool contains(int x) const noexcept;
    [[nodiscard]] std::vector<int> values() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    ~Splay();
};

#endif
