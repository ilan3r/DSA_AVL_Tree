#ifndef LAB3_AVL_TREE_H
#define LAB3_AVL_TREE_H

#include "binary-search-tree.h"

class AVLTree : public BinarySearchTree {
public:
    // Overriden insert and remove functions
    // Do not modify these definitions

    // The AVL tree should be kept balanced after calling just insert or remove.
    bool insert(DataType val);
    bool remove(DataType val);

    // Define additional functions and attributes below if you need
    void SingleRightRotation (Node **pTpTroot, Node *alpha);
    void SingleLeftRotation (Node **pTpTroot, Node *alpha);
    void LeftRightRotation (Node **pTpTroot, Node *alpha);
    void RightLeftRotation (Node **pTpTroot, Node *alpha);

    BinarySearchTree::Node* getParentNode(DataType val);
    BinarySearchTree::Node* getDeletePointer(DataType val);

    int getNodeDepth2(Node *n);
    bool balanceTree(Node** pTpTroot, DataType val);
};

#endif  // LAB3_AVL_TREE_H
