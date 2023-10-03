#include "tree/Tree.hpp"

Splay::Node::~Node() {
    delete left;
    delete right;
}

Splay::~Splay() {
    delete root;
}

std::size_t Splay::size() const noexcept {
    return tree_size;
}

bool Splay::empty() const noexcept {
    return tree_size == 0;
}

void Splay::dfs(Node* v, std::vector<int>& values) const noexcept {
    if (!v) {
        return;
    }
    dfs(v->left, values);
    values.push_back(v->value);
    dfs(v->right, values);
}

std::vector<int> Splay::values() const noexcept {
    std::vector<int> values;
    values.reserve(size());
    dfs(root, values);
    return values;
}

void Splay::rotate_right(Splay::Node* v) const noexcept {
    Node* x = v->left;
    Node* y = x->right;
    Node* p = v->parent;
    if (p) {
        if (p->left != v) {
            p->right = x;
        } else {
            p->left = x;
        }
    }
    if (y) {
        y->parent = v;
    }
    x->parent = p;
    x->right  = v;
    v->parent = x;
    v->left   = y;
}

void Splay::rotate_left(Splay::Node* v) const noexcept {
    Node* x = v->right;
    Node* y = x->left;
    Node* p = v->parent;
    if (p) {
        if (p->left != v) {
            p->right = x;
        } else {
            p->left = x;
        }
    }
    if (y) {
        y->parent = v;
    }
    x->parent = p;
    x->left   = v;
    v->parent = x;
    v->right  = y;
}

bool Splay::zig_zag(Splay::Node* v) const noexcept {
    Node* p  = v->parent;
    Node* pp = p->parent;
    if (pp->left == p && p->right == v) {
        rotate_left(p);
        rotate_right(pp);
        return true;
    }
    if (pp->right == p && p->left == v) {
        rotate_right(p);
        rotate_left(pp);
        return true;
    }
    return false;
}

bool Splay::zig_zig(Splay::Node* v) const noexcept {
    Node* p  = v->parent;
    Node* pp = p->parent;
    if (p->left == v && pp->left == p) {
        rotate_right(pp);
        rotate_right(p);
        return true;
    }
    if (p->right == v && pp->right == p) {
        rotate_left(pp);
        rotate_left(p);
        return true;
    }
    return false;
}

bool Splay::zig(Splay::Node* v) const noexcept {
    if (v->parent->left != v) {
        rotate_left(v->parent);
    } else {
        rotate_right(v->parent);
    }
    return true;
}

void Splay::splay(Splay::Node* v) const noexcept {
    while (v->parent) {
        Node* pp = v->parent->parent;
        if (pp) {
            if (!zig_zag(v)) {
                zig_zig(v);
            }
        } else {
            zig(v);
        }
    }
    root = v;
}

Splay::Node* Splay::find(int x) const noexcept {
    Node* v = root;
    while (v && v->value != x) {
        v = (v->value < x) ? v->right : v->left;
    }
    if (v) {
        splay(v);
    }
    return v;
}

bool Splay::contains(int x) const noexcept {
    return find(x);
}

bool Splay::remove(int x) {
    if (!find(x)) {
        return false;
    }
    tree_size--;
    Node* v = root;
    while (v->value != x) {
        v = (v->value < x) ? v->right : v->left;
    }
    splay(v);
    Node* left  = root->left;
    Node* right = root->right;
    root->left  = nullptr;
    root->right = nullptr;
    delete root;
    root = nullptr;
    if (!right && !left) {
        return true;
    }
    if (!right) {
        root         = left;
        root->parent = nullptr;
        return true;
    }
    if (!left) {
        root         = right;
        root->parent = nullptr;
        return true;
    }
    root         = left;
    root->parent = nullptr;
    Node* mx     = left;
    while (mx->right && mx->right->value > mx->value) {
        mx = mx->right;
    }
    splay(mx);
    root->right   = right;
    right->parent = root;
    return true;
}

Splay::Node* Splay::insert(Splay::Node*& v, Splay::Node*& last, int x) {
    if (!v) {
        v = new Node(x, nullptr, nullptr, last);
        return v;
    }
    if (v->value > x) {
        return insert(v->left, v, x);
    }
    if (v->value < x) {
        return insert(v->right, v, x);
    }
    return nullptr;
}

bool Splay::insert(int x) {
    Node* last   = nullptr;
    Node* answer = insert(root, last, x);
    if (answer) {
        tree_size++;
        splay(answer);
        return true;
    }
    return false;
}
