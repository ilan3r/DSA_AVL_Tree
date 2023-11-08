#include "binary-search-tree.h"

#include <iostream>
#include <cstdlib>

#include <queue>

using namespace std;

BinarySearchTree::Node::Node(DataType newval) {
    left = nullptr;
    right = nullptr;
    val = newval;
}

int BinarySearchTree::getNodeDepth(Node* n) const {
    //use with depth fuction since this can be called recursively
    if (n == nullptr)
    {
        return -1;
    }
    else if (n->left == nullptr && n->right == nullptr)
        return 0;
    else
    {
        return 1+ std::max(getNodeDepth(n->left), getNodeDepth(n->right));
    }
}

BinarySearchTree::BinarySearchTree() {
    size_ = 0;
    root_ = nullptr;
}

BinarySearchTree::~BinarySearchTree() {
    if (root_ == nullptr)
        return;
    else
    {
        queue <Node*> q;
        q.push(root_);
        while (!(q.empty()))
        {
            Node *temp = q.front();
            q.pop();
            if (temp->left != nullptr) q.push(temp->left);
            if (temp->right != nullptr) q.push(temp->right);
            delete temp;
        }
        return;
    }

}

unsigned int BinarySearchTree::size() const {
    return size_;
}

BinarySearchTree::DataType BinarySearchTree::max() const {
    if (size_ == 0)
    {
        cout << "should not call max function on empty tree" << endl;
        return -1;
    }
    else
    {
        Node *cur = root_;
        //go as far right in the tree as we can
        while (cur->right != nullptr)
        {
            cur = cur->right;
        }
        return cur->val;

    }
}

BinarySearchTree::DataType BinarySearchTree::min() const {
    if (size_ == 0)
    {
        cout << "should not call max function on empty tree" << endl;
        return -1;
    }
    else
    {
        Node *cur = root_;
        //go as far left in the tree as we can
        while (cur->left != nullptr)
        {
            cur = cur->left;
        }
        return cur->val;
    }

}

unsigned int BinarySearchTree::depth() const {
    return getNodeDepth(root_);
    //use the getNodeDepth function
}

void BinarySearchTree::print() const {
    cout << "===printing tree===";
    if (root_ == nullptr)
    {
        cout << "cannot print an empty bst" << endl;
        return;
    }
    else
    {

        queue <Node*> q;
        q.push(root_);
        while (!(q.empty()))
        {
            Node *temp = q.front();
            if (temp->left != nullptr) q.push(temp->left);
            if (temp->right != nullptr) q.push(temp->right);
            q.pop();
            cout << temp->val << " ";
        }
        cout << endl;
    }
}

bool BinarySearchTree::exists(DataType val) const {

    Node* cur = root_;
    while (cur != nullptr) {
        if (cur->val == val) return true;
        else if (val < cur->val) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return false;
}

BinarySearchTree::Node* BinarySearchTree::getRootNode() {
    return root_;
}

BinarySearchTree::Node** BinarySearchTree::getRootNodeAddress() {
    return &root_;
}

bool BinarySearchTree::insert(DataType val) {
    Node *cur = root_;
    Node* temp = new Node(val);
    if (cur == nullptr)
    {
        root_ = temp;
        size_++;
        return true;
    }
    else
    {
        while (cur != nullptr)
        {
            if (cur-> val == val)
                return false;

            else if (temp->val < cur->val)
            {
                if(cur->left == nullptr)
                {
                    cur->left = temp;
                    size_++;
                    return true;
                }
                else cur = cur->left;
            }

            else if (temp->val > cur->val)
            {
                if(cur->right == nullptr)
                {
                    cur->right = temp;
                    size_++;
                    return true;
                }
                else cur = cur->right;
            }
        }
        cout << "error - unable to insert value" << endl;
        return false;


    }
}

bool BinarySearchTree::remove(DataType val) {
    if (size_ == 0)
    {
        return false;
    }
    else if (!(exists(val)))
        return false;

    else if(exists(val)) {
        //find the node we want to delete, and its parent

        //pointer to pointer of root
        Node **ptpt = getRootNodeAddress();

        Node *cur = *ptpt;
        Node *par = nullptr;
        bool isLeftChild = false;
        bool found = false;

        while (cur != nullptr) {
            if (cur->val == val) {
                found = true;
                break;
            } else if (val < cur->val) {
                par = cur;
                cur = cur->left;
                isLeftChild = true;
            } else {
                par = cur;
                cur = cur->right;
                isLeftChild = false;
            }
        }
        if (found == false) return false;
        //we now have a cur and par pointer to current and parent node
        //and a bool for left child

        //if the node we want to delete is a leaf node
        if (cur->right == nullptr && cur->left == nullptr) {
            if (cur == *ptpt)
                //special case, if current equals pointer to pointer of root node
            {
                delete *ptpt;
                *ptpt = nullptr;
                size_--;
                return true;
            } else
                //if cur is not the root node
            {
                delete cur;
                cur = nullptr;
                if (isLeftChild) par->left = nullptr;
                else {
                    par->right = nullptr;
                }
                size_--;
                return true;

            }

        }

        //if the node we want to delete has one child node

        //if left is empty and right isn't
        if (cur->left == nullptr && cur->right != nullptr) {

            //if cur is pointing to root
            if (cur == *ptpt)
            {
                root_= cur->right;
                delete cur;
                cur = nullptr;
                size_--;
                return true;
            }
            if (isLeftChild) par->left = cur->right;
            else par->right = cur->right;

            delete cur;
            cur = nullptr;

            size_--;
            return true;

        }

        //if right is empty and left isn't
        if (cur->left != nullptr && cur->right == nullptr) {
            //if cur is pointing to root
            if (cur == *ptpt)
            {
                root_= cur->left;
                delete cur;
                cur = nullptr;
                size_--;
                return true;
            }
            if (isLeftChild) par->left = cur->left;
            else par->right = cur->left;

            delete cur;
            cur = nullptr;

            size_--;
            return true;

        }

        //if the node we are removing has two children,
        //find the predecessor - the largest value in left subtree
        if (cur->right != nullptr && cur->left != nullptr) {
            Node *predecessor = cur->left;
            isLeftChild = true;
            Node *predecessor_par = cur;

            while (predecessor->right != nullptr) {
                predecessor_par = predecessor;
                predecessor = predecessor->right;
                isLeftChild = false;
            }

            //replace node we want to delete with the predecessor
            cur->val = predecessor->val;

            //delete the predecessor

            if (predecessor->left == nullptr)
                //if predecessor is a leaf node
            {
                if (isLeftChild) predecessor_par->left = nullptr;
                else predecessor_par->right = nullptr;
            } else
                //if predecessor has one child (can only have a left child when it is the true predecessor
            {
                if (isLeftChild) predecessor_par->left = predecessor->left;
                else predecessor_par->right = predecessor->left;
            }
            delete predecessor;
            predecessor = nullptr;
            size_--;
            return true;
        }
    }

}
