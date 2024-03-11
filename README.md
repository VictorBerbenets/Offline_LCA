# Search tree implementation (AVL tree)
### About 
This program solves RMQ problem in <O(n), O(1)> complexity. Where preprocessing
takes O(n) and answering each querry - O(1).
The key role here is played by reducing the RMQ problem to LCA and back LCA to
RMQ.
1) First we build a Cartesian tree using an implicit key.
2) Afterwards we solve the problem RMQ +-1 (Eulerian tree traversal)
3) At the end we use the Farah-Colton and Bender algorithm
