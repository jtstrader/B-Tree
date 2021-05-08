# B-Tree
An implementation of B-Tree that takes in varying key types (in this example, Albums) to create and save a B-Tree to disk.

btree.cpp is particularly built for max order 5. Splitting occurs by swapping the max value in the node (assuming it is greater than the key attempting to be inserted) with the inserting key and sorting all children and keys, before performing the split and writing the new contents to disk. Each individual split node should on average increment the write variable by 2.

Can only perform insertions for the purpose of creating a B-Tree and applying searches. Any deletions intended will have to be deleted through the input files and then the tree will have to be recreated.

updateBT.cpp takes in "transaction files" to either search or an object or insert one into the tree.
