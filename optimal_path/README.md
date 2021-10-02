## The problem presented
***Consider a sequence of vertices that form a path in a graph. The path is the graph itself. Compute an optimal path (shortest possible) while keeping the extremities by eliminating one single sequence from the path.***

Example:
```
v1, v2, v4, v5, v3, v1, v2, v5, v6, v7, v8, v9, v5, v9, v8, v10
becomes
v1, v2, v4, v5, v9, v8, v10
```
 ## Additional files
 There are 3 example files which can all be found [here](https://github.com/andreea-511/cplusplusprojects/tree/main/optimal_path/examples). Each .txt file contains a table representation of a matrix. On the first row we have the number of vertices and edges and the rest of the rows represent the edges by the two vertices that the edge connects. For the example above we have the following .txt file:
 ```
10 15
1 2
2 4
4 5
5 3
3 1
1 2
2 5
5 6
6 7
7 8
8 9
9 5
5 9
9 8
8 10
```
### Important to note:
In order to solve this problem I have used *Roy-Warshall path existance matrices*. For more information about the Roy-Warshall matrix check [this link](https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm). 

The graphs used in the examples are unweighted and unoriented, but the source code works on weighted and/or oriented graphs. Be sure to pick the right option when you read your graph from your .txt file.

## How I solved it
**The source code for this problem can be found [here](https://github.com/andreea-511/cplusplusprojects/blob/main/optimal_path/optimal_path.cpp).**

In order to find the longest single sequence that will create an optimal path while preserving the extremities I chose to try all possible sequences and find the longest that checks all of the conditions. All possible solutions have been stored in a struct vector that contains the solution matrix and number of edges. The solution with the smallest number of edges is the optimal solution.

To check a solution we follow a few steps:
1. we choose the start of the sequence that we want to remove
2. we choose the end of the sequence that we want to remove
3. we check throught the Roy-Warshall matrix if the remaining subgraph is connected
4. we check if the sequence length is longer or equal to the previous maximum lenght registered
5. if the solution is good we add it to the solution vector; if not, we advance the end of the sequence by one edge -> if that is not possible we advance the start of the sequence by one unit and set the end of the sequence one edge away
