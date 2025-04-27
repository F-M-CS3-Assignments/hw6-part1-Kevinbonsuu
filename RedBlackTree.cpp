
#include "RedBlackTree.h"
#include <stdexcept>

using namespace std;

// Default constructor - creates an empty tree
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

// Constructor that creates a tree with a single black root node
RedBlackTree::RedBlackTree(int newData) {
    root = new RBTNode();
    root->data = newData;
    root->color = COLOR_BLACK; // Root must always be black
    numItems = 1;
}

// Copy constructor - deep copy of another tree
RedBlackTree::RedBlackTree(const RedBlackTree& rbt) {
    root = CopyOf(rbt.root);
    numItems = rbt.numItems;
}

// Public Insert function to add a new value into the tree
void RedBlackTree::Insert(int newData) {
    if (root == nullptr) {
        // If the tree is empty, create a black root node
        root = new RBTNode();
        root->data = newData;
        root->color = COLOR_BLACK;
        numItems++;
        return;
    }

    if (Contains(newData)) {
        throw invalid_argument("Duplicate value."); // No duplicate values allowed
    }

    // Create a new red node
    RBTNode* newNode = new RBTNode();
    newNode->data = newData;
    newNode->color = COLOR_RED;

    // Insert the node as in a normal BST
    BasicInsert(newNode);

    // Fix any red-black property violations
    InsertFixUp(newNode);

    // Always ensure the root is black
    root->color = COLOR_BLACK;

    numItems++;
}

// Helper function to insert node in BST fashion
void RedBlackTree::BasicInsert(RBTNode* node) {
    RBTNode* curr = root;
    RBTNode* prev = nullptr;

    while (curr != nullptr) {
        prev = curr;
        if (node->data < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    node->parent = prev;
    if (node->data < prev->data) {
        prev->left = node;
    } else {
        prev->right = node;
    }
}

// Fix up the tree after insertion to restore red-black properties
void RedBlackTree::InsertFixUp(RBTNode* node) {
    while (node != root && node->parent->color == COLOR_RED) {
        if (IsLeftChild(node->parent)) {
            RBTNode* uncle = GetUncle(node);
            if (uncle != nullptr && uncle->color == COLOR_RED) {
                // Case 1: Uncle is red - recolor
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                node = node->parent->parent;
            } else {
                if (IsRightChild(node)) {
                    // Case 2: Node is right child - rotate left
                    node = node->parent;
                    LeftRotate(node);
                }
                // Case 3: Node is left child - rotate right
                node->parent->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                RightRotate(node->parent->parent);
            }
        } else {
            // Mirror image of above
            RBTNode* uncle = GetUncle(node);
            if (uncle != nullptr && uncle->color == COLOR_RED) {
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                node = node->parent->parent;
            } else {
                if (IsLeftChild(node)) {
                    node = node->parent;
                    RightRotate(node);
                }
                node->parent->color = COLOR_BLACK;
                node->parent->parent->color = COLOR_RED;
                LeftRotate(node->parent->parent);
            }
        }
    }
    root->color = COLOR_BLACK; // Always end ensuring root is black
}

// Check if tree contains a specific value
bool RedBlackTree::Contains(int data) const {
    return Get(data) != nullptr;
}

// Find the minimum value in the tree
int RedBlackTree::GetMin() const {
    if (root == nullptr) throw invalid_argument("Tree is empty.");
    RBTNode* curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->data;
}

// Find the maximum value in the tree
int RedBlackTree::GetMax() const {
    if (root == nullptr) throw invalid_argument("Tree is empty.");
    RBTNode* curr = root;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr->data;
}

// Recursively build infix (sorted) string
string RedBlackTree::ToInfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return ToInfixString(n->left) + GetNodeString(n) + ToInfixString(n->right);
}

// Recursively build prefix string
string RedBlackTree::ToPrefixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return GetNodeString(n) + ToPrefixString(n->left) + ToPrefixString(n->right);
}

// Recursively build postfix string
string RedBlackTree::ToPostfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return ToPostfixString(n->left) + ToPostfixString(n->right) + GetNodeString(n);
}

// Get color as a string ("R" or "B")
string RedBlackTree::GetColorString(const RBTNode* n) {
    if (n == nullptr) return "";
    return (n->color == COLOR_RED) ? "R" : "B";
}

// Build a string for a single node (with color)
string RedBlackTree::GetNodeString(const RBTNode* n) {
    if (n == nullptr) return "";
    return " " + GetColorString(n) + to_string(n->data) + " ";
}

// Find the uncle of a given node (used in insert fix-up)
RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    if (node->parent == nullptr || node->parent->parent == nullptr) return nullptr;
    if (IsLeftChild(node->parent)) {
        return node->parent->parent->right;
    } else {
        return node->parent->parent->left;
    }
}

// Check if a node is a left child
bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node->parent != nullptr && node->parent->left == node;
}

// Check if a node is a right child
bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node->parent != nullptr && node->parent->right == node;
}

// Perform a left rotation around a node
void RedBlackTree::LeftRotate(RBTNode* node) {
    RBTNode* r = node->right;
    node->right = r->left;
    if (r->left != nullptr) {
        r->left->parent = node;
    }
    r->parent = node->parent;
    if (node->parent == nullptr) {
        root = r;
    } else if (IsLeftChild(node)) {
        node->parent->left = r;
    } else {
        node->parent->right = r;
    }
    r->left = node;
    node->parent = r;
}

// Perform a right rotation around a node
void RedBlackTree::RightRotate(RBTNode* node) {
    RBTNode* l = node->left;
    node->left = l->right;
    if (l->right != nullptr) {
        l->right->parent = node;
    }
    l->parent = node->parent;
    if (node->parent == nullptr) {
        root = l;
    } else if (IsRightChild(node)) {
        node->parent->right = l;
    } else {
        node->parent->left = l;
    }
    l->right = node;
    node->parent = l;
}

// Recursively copy a node and its subtree
RBTNode* RedBlackTree::CopyOf(const RBTNode* node) {
    if (node == nullptr) return nullptr;
    RBTNode* copy = new RBTNode();
    copy->data = node->data;
    copy->color = node->color;
    copy->IsNullNode = node->IsNullNode;
    copy->left = CopyOf(node->left);
    copy->right = CopyOf(node->right);
    if (copy->left != nullptr) copy->left->parent = copy;
    if (copy->right != nullptr) copy->right->parent = copy;
    return copy;
}

// Get a node containing the given data value
RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* curr = root;
    while (curr != nullptr) {
        if (data == curr->data) return curr;
        if (data < curr->data) curr = curr->left;
        else curr = curr->right;
    }
    return nullptr;
}