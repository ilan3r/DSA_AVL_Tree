# DSA_AVL_Tree
Data Structures and Algorithms (MTE 140) Assignment 
Professor Charbel Azzi 

Implementing an AVL Tree - A self-balancing binary search tree. In an AVL Tree, the difference in height between the left and right subtrees is less than 1 for every node
The purpose of this self-balancing binary search tree is to prevent the binary search tree from becoming too long such that it approaches time O(n)

To make the AVL Tree, we use the binary search tree code, but add the balancing functions, and create new insert and remove which incorporate the balancing after a new node/leaf has been added/removed. In the AVL Tree class, we inherit the binary search tree class. 
