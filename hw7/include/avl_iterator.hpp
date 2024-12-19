#ifndef AVL_ITERATOR_HPP
#define AVL_ITERATOR_HPP

#include <iterator>

template <typename Node, typename Key, typename T>
class AVLIterator {
    Node* current;

    Node* findNext(Node* node) {
        if (node->right) {
            node = node->right;
            while (node->left)
                node = node->left;
        } else {
            while (node->parent && node == node->parent->right)
                node = node->parent;
            node = node->parent;
        }
        return node;
    }

    Node* findPrev(Node* node) {
        if (node->left) {
            node = node->left;
            while (node->right)
                node = node->right;
        } else {
            while (node->parent && node == node->parent->left)
                node = node->parent;
            node = node->parent;
        }
        return node;
    }

public:

    using value_type = std::pair<const Key, T>;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag; // двунаправленный 

    explicit AVLIterator(Node* node) : current(node) {}

    reference operator*() const { return current->data; }

    pointer operator->() const { return &(current->data); }

    AVLIterator& operator++() {
        current = findNext(current);
        return *this;
    }

    AVLIterator operator++(int) {
        AVLIterator temp = *this;
        ++(*this);
        return temp;
    }

    AVLIterator& operator--() {
        current = findPrev(current);
        return *this;
    }

    AVLIterator operator--(int) {
        AVLIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const AVLIterator& other) const { return current == other.current; }

    bool operator!=(const AVLIterator& other) const { return current != other.current; }
};

template <typename Node, typename Key, typename T>
class AVLConstIterator {
public:
    using value_type = const std::pair<const Key, T>;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    const Node* current;

public:
    explicit AVLConstIterator(const Node* node) : current(node) {}

    reference operator*() const { return current->data; }

    pointer operator->() const { return &(current->data); }

    AVLConstIterator& operator++() {
        current = AVLIterator<Node, Key, T>::findNext(const_cast<Node*>(current));
        return *this;
    }

    AVLConstIterator operator++(int) {
        AVLConstIterator temp = *this;
        ++(*this);
        return temp;
    }

    AVLConstIterator& operator--() {
        current = AVLIterator<Node, Key, T>::findPrev(const_cast<Node*>(current));
        return *this;
    }

    AVLConstIterator operator--(int) {
        AVLConstIterator temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const AVLConstIterator& other) const { return current == other.current; }

    bool operator!=(const AVLConstIterator& other) const { return current != other.current; }
};

#endif // AVL_ITERATOR_HPP
 
