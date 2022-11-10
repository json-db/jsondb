# B+ Tree 的算法

* http://www.cburch.com/cs/340/reading/btree/
* https://www.geeksforgeeks.org/difference-between-b-tree-and-b-tree/
* https://www.geeksforgeeks.org/insertion-in-a-b-tree/

## 

A B+-tree maintains the following invariants:

1. Every node has one more references than it has keys.
2. All leaves are at the same distance from the root.
3. For every non-leaf node N with k being the number of keys in N: all keys in the first child's subtree are less than N's first key; and all keys in the ith child's subtree (2 ≤ i ≤ k) are between the (i − 1)th key of n and the ith key of n.
4. The root has at least two children.
5. Every non-leaf, non-root node has at least floor(d / 2) children.
6. Each leaf contains at least floor(d / 2) keys.
7. Every key from the table appears in a leaf, in left-to-right sorted order.

## Insertion algorithm

1. If the node has an empty space, insert the key/reference pair into the node.
2. If the node is already full, split it into two nodes, distributing the keys evenly between the two nodes. If the node is a leaf, take a copy of the minimum value in the second of these two nodes and repeat this insertion algorithm to insert it into the parent node. If the node is a non-leaf, exclude the middle value during the split and repeat this insertion algorithm to insert this excluded value into the parent node.

## Deletion algorithm