#include "avl-tree.h";
#include <iostream>
using namespace std;

BinarySearchTree::Node* AVLTree::getDeletePointer(DataType val)
{
    //this is to get a pointer of the deleted node
    //it will return the deleted nodes parent, or if the applicable, the deleted nodes predecessor

    Node **pTtoRoot = getRootNodeAddress();
    Node *parent = nullptr;
    Node *current = *pTtoRoot;

    //find the node we want to delete
    while (current != nullptr) {
        if (current->val == val)
            break;
        else if (val < current->val) {
            parent = current;
            current = current->left;
        } else {
            parent = current;
            current = current->right;
        }
    }

    // find predecessor if applicable
    if (current->left != nullptr)
    {
        Node *predecessor = current->left;
        while (predecessor->right != nullptr)
        {
            predecessor = predecessor->right;
        }
        //return pointer to the predecessor
        parent = predecessor;

    }
    if (parent == nullptr)
    {
        //if the deleted node is the root, return the address of the root
        return *pTtoRoot;
    }
    return parent;

}

BinarySearchTree::Node* AVLTree::getParentNode(DataType val)
//function to return pointer to parent of deleted Node, or it's predecessor
{
    Node **pTtoRoot = getRootNodeAddress();
    Node *parent = nullptr;
    Node *current = *pTtoRoot;

    //find the node we want to delete
    while (current != nullptr) {
        if (current->val == val)
            break;
        else if (val < current->val) {
            parent = current;
            current = current->left;
        } else {
            parent = current;
            current = current->right;
        }
    }

    if (parent == nullptr)
    {
        return *pTtoRoot;
    }
    return parent;

}

int AVLTree::getNodeDepth2(Node *n)
{
    //depth function identical to the one used in Lab 3.1
    //had to make another function since we don't have access to the private getNodeDepth function in
    //the binary-search-tree.cpp file
    if (n == nullptr)
    {
        return -1;
    }
    else if (n->left == nullptr && n->right == nullptr)
        return 0;
    else
    {
        return 1+ std::max(getNodeDepth2(n->left), getNodeDepth2(n->right));
    }
}

bool AVLTree::remove(DataType val)
{
    if (exists(val))
    {
        Node** pTtoRoot = getRootNodeAddress();
        Node* parent = nullptr;
        parent = getDeletePointer(val);

        bool isEmptyRoot = false;

        if ((*pTtoRoot)->val == val && (*pTtoRoot)->right == nullptr
            && (*pTtoRoot)->left == nullptr)
        {
            isEmptyRoot = true;
        }
        //special case, when deleting the root and the tree only contains the root

        bool bst_remove = BinarySearchTree::remove(val);
        if (!(bst_remove)) return false;

        //if the root was just deleted and there is nothing left in the tree
        //the tree does not need to be balanced
        if (isEmptyRoot) return true;

        Node* alpha;
        Node* A;
        Node* N;

        //continue to move up and balance until reached a balanced root
        while (parent != *pTtoRoot || abs(getNodeDepth2(parent->right)- getNodeDepth2(parent->left)) >= 2)
        {
            //starting the original parent of the removed node, check balance
            if (abs(getNodeDepth2(parent->right) - getNodeDepth2(parent->left)) >= 2) {
                //if the parent is unbalanced
                alpha = parent;

                if (getNodeDepth2(alpha->right) > getNodeDepth2(alpha->left)) {
                    A = alpha->right;

                }
                else if (getNodeDepth2(alpha->right) < getNodeDepth2(alpha->left)) {
                    A = alpha->left;
                }

                //get N based on which subtree of A is higher

                //if right subtree is higher
                if (getNodeDepth2(A->right) > getNodeDepth2(A->left)) {
                    N = A->right;
                } else if (getNodeDepth2(A->right) < getNodeDepth2(A->left)) {
                    N = A->left;
                }

                //balance with the N, A, and alpha we have
                if (N->val < alpha->val && N->val < alpha->left->val) SingleRightRotation(pTtoRoot, alpha);
                else if (N->val > alpha->val && N->val > alpha->right->val) SingleLeftRotation(pTtoRoot, alpha);
                else if (N->val < alpha->val && N->val > alpha->left->val) LeftRightRotation(pTtoRoot, alpha);
                else if (N->val > alpha->val && N->val < alpha->right->val) RightLeftRotation(pTtoRoot, alpha);

            }
            //move up a level closer to the root and repeat
            parent = getParentNode(parent->val);
        }

        return true;
    }
    else return false; //the value we want to delete doesn't exist

}


bool AVLTree::insert(DataType val) {
    bool bst_insert = BinarySearchTree::insert(val);
    if (!(bst_insert)) return false;
    Node **p = getRootNodeAddress();
    balanceTree(p, val);
    return true;

}

bool AVLTree::balanceTree(Node** pTpTroot, DataType val)
{
    Node *root = *pTpTroot;

    //find if any of the nodes are unbalanced

    //if we just have the root left
    if (root->val == val) return true;

    Node *temp =root;
    Node *temp_parent;
    Node *alpha = nullptr;

    do
    {
        temp_parent = temp;
        if (val > temp->val) temp = temp->right;
        else if (val < temp->val) temp = temp->left;
        int leftDepth = getNodeDepth2(temp_parent->left);
        int rightDepth = getNodeDepth2(temp_parent->right);

        //check if node is unbalanced
        if (abs(rightDepth-leftDepth) >=2) alpha = temp_parent;

    }
    while(temp->val != val);

        //if no alpha found, there are no unbalanced nodes
    if(alpha == nullptr) return true;

    if (val<alpha->val && val<alpha->left->val) SingleRightRotation(pTpTroot, alpha);
    else if (val > alpha->val && val > alpha->right->val ) SingleLeftRotation(pTpTroot, alpha);
    else if ( val < alpha->val && val > alpha->left->val ) LeftRightRotation(pTpTroot, alpha);
    else if (val > alpha->val && val < alpha->right->val ) RightLeftRotation(pTpTroot, alpha);

    return true;
}



void AVLTree::SingleRightRotation (Node **pTpTroot, Node *alpha)
{
    //node A is the left child of alpha
    BinarySearchTree::Node *A = alpha->left;

    //alpha's new left child is A's right
    alpha->left = A->right;

    //A's new right is alpha
    A->right = alpha;

    //if alpha was the root of the tree, change the root to A
    if (*pTpTroot == alpha) *pTpTroot = A;

    else
    {
        //make alpha's parent take A as the new child

        Node *temp = *pTpTroot;
        Node **parentpTalpha = nullptr;
        //find alpha's parent
        do
        {
            if (alpha->val > temp->val)
            {
                parentpTalpha = &temp->right;
                temp = temp->right;
            }
            else if (alpha->val < temp->val)
            {
                parentpTalpha = &temp->left;
                temp=temp->left;
            }
        }
        while (temp->val != alpha->val);

        *parentpTalpha = A;


    }
}


void AVLTree::SingleLeftRotation (Node **pTpTroot, Node *alpha)
{
    //node A is the right child of alpha
    Node *A = alpha->right;

    //alpha's new right child is A's left
    alpha->right = A->left;

    //A's new left is alpha
    A->left = alpha;

    //if alpha was the root of the tree, change the root to A
    if (*pTpTroot == alpha) *pTpTroot = A;

    else
    {
        //make alpha's parent take A as the new child

        BinarySearchTree::Node *temp = *pTpTroot;
        BinarySearchTree::Node **parentpTalpha = nullptr;
        //find alpha's parent
        do
        {
            if (alpha->val > temp->val)
            {
                parentpTalpha = &temp->right;
                temp = temp->right;
            }
            else if (alpha->val < temp->val)
            {
                parentpTalpha = &temp->left;
                temp=temp->left;
            }
        }
        while (temp->val != alpha->val);

        *parentpTalpha = A;
    }
}

void AVLTree::LeftRightRotation (Node **pTpTroot, Node *alpha)
{
    //A is alpha's left
    Node* A = alpha->left;

    //single left rotation about A
    SingleLeftRotation(pTpTroot, A);

    //single right rotation about alpha
    SingleRightRotation(pTpTroot, alpha);
}


void AVLTree::RightLeftRotation (Node **pTpTroot, Node *alpha)
{
    //A is alpha's right
    Node* A = alpha->right;

    //single right rotation about A
    SingleRightRotation(pTpTroot, A);

    //single left rotation about alpha
    SingleLeftRotation(pTpTroot, alpha);
}