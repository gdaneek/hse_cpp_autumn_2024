#ifndef AVL_HPP
#define AVL_HPP

#include <memory>
#include <stdexcept>
#include <utility>
#include "avl_iterator.hpp" // итератор вынес отдельно. в целом можно внести в avl, с точки зрения функционала разницы нет, using'ом привязал
#include "custom_alloc.hpp" // аллокатор

template <class Key, class T, class Compare = std::less<Key>,  template <typename> class AllocatorT = CustomAllocator> // так можно передать будет любой аллокатор, например <..., std::allocator>; мой по умолчанию
class avl {
    struct Node {
        std::pair<const Key, T> data;
        Node* left;
        Node* right;
        Node* parent;
        int height;

        explicit Node(const std::pair<const Key, T>& value)
            : data(value), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
    };

    Node* root;
    size_t tree_size;
    Compare comp;

    using Allocator = AllocatorT<Node>;
    using AllocTraits = std::allocator_traits<Allocator>;
    Allocator node_alloc; // аллокатор нод

    int height(Node* node) const { return node ? node->height : 0; }

    int balanceFactor(Node* node) const { return node ? height(node->left) - height(node->right) : 0; }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        if (newRoot->left)
            newRoot->left->parent = node;
        newRoot->left = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        if (newRoot->right)
            newRoot->right->parent = node;
        newRoot->right = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }

    Node* balance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    Node* insert(Node* node, const std::pair<const Key, T>& value, Node* parent = nullptr) {
        if (!node) {
            ++tree_size;
            Node* newNode = node_alloc.allocate(1);
            AllocTraits::construct(node_alloc, newNode, value);
            newNode->parent = parent;
            return newNode;
        }
        if (comp(value.first, node->data.first)) {
            node->left = insert(node->left, value, node);
        } else if (comp(node->data.first, value.first)) {
            node->right = insert(node->right, value, node);
        } else {
            node->data.second = value.second; // Обновление значения
        }
        return balance(node);
    }

    Node* findMin(Node* node) const {
        return node->left ? findMin(node->left) : node;
    }

    Node* removeMin(Node* node) {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return balance(node);
    }

    Node* erase(Node* node, const Key& key) {
        if (!node) return nullptr;
        if (comp(key, node->data.first)) {
            node->left = erase(node->left, key);
        } else if (comp(node->data.first, key)) {
            node->right = erase(node->right, key);
        } else {
            Node* left = node->left;
            Node* right = node->right;
            AllocTraits::destroy(node_alloc, node);
            node_alloc.deallocate(node, 1);
            --tree_size;
            if (!right) return left;
            Node* min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            if (min->left)
                min->left->parent = min;
            return balance(min);
        }
        return balance(node);
    }

    Node* find(Node* node, const Key& key) const {
        if (!node) return nullptr;
        if (comp(key, node->data.first))
            return find(node->left, key);
        else if (comp(node->data.first, key))
            return find(node->right, key);
        return node;
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            AllocTraits::destroy(node_alloc, node);
            node_alloc.deallocate(node, 1);
        }
    }

public:
    using iterator = AVLIterator<Node, Key, T>;
    using const_iterator = AVLConstIterator<Node, Key, T>;

    avl() : root(nullptr), tree_size(0) {}

    ~avl() {
        clear(root);
        root = nullptr;
    }

    T& operator[](const Key& key) {
        Node* node = find(root, key);
        if (!node) {
            root = insert(root, std::make_pair(key, T{}));
            node = find(root, key);
        }
        return node->data.second;
    }

    T& at(const Key& key) {
        Node* node = find(root, key);
        if (!node)
            throw std::out_of_range("Key not found");
        return node->data.second;
    }

    void insert(const std::pair<const Key, T>& value) {
        root = insert(root, value);
    }

    void erase(const Key& key) {
        root = erase(root, key);
    }

    bool contains(const Key& key) const {
        return find(root, key) != nullptr;
    }

    bool empty() const { return tree_size == 0; }

    size_t size() const { return tree_size; }

    void clear() {
        clear(root);
        root = nullptr;
        tree_size = 0;
    }

    iterator begin() {
        return iterator(findMin(root));
    }

    iterator end() {
        return iterator(nullptr);
    }

    const_iterator begin() const {
        return const_iterator(findMin(root));
    }

    const_iterator end() const {
        return const_iterator(nullptr);
    }

    iterator rbegin() {
        return iterator(findMax(root)); 
    }

    iterator rend() {
        return iterator(nullptr);  
    }

    const_iterator rbegin() const {
        return const_iterator(findMax(root));  
    }

    const_iterator rend() const {
        return const_iterator(nullptr);  
    }
};

#endif // AVL_HPP