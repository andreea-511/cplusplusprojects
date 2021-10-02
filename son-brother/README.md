## The problem presented
***Write a C++ program that finds the parent of a given vertex x using the Son-Brother tree graph representation.***
_____________________________
A son-brother tree graph representation is usually a matrix with two rows and a variable number of collumns that describes a tree graph. An example of such representation is as follows:
```
 2  5  0  8  0  9  0 14  0  0  0  0  0  0  0  0
 0  3  4  0  6  7  0  0 10 11 12 13  0 15 16  0
```

The two rows represent the son row and the brother row in this example but it is not mandatory to be in this order and it should always be specified. In my code and problem I will be using this format. 
Knowing this, if we look at collumn 3 for example we can say that the vertex 3 has no sons (last vertex on its branch) but has 4 as a brother (4 has the same father as 3).

### Important to note: 
father/parent - ascendant;
son - descendant;

Although not necessary in my example file I also included the vertex that represents the root of the tree as well as the total number of vertices.

### Observation 1:
The root of the tree graph can be easily identified from the Son-Brother matrix because it is the only vertex value that does not appear at all.

## Additional files
The [.txt file](https://github.com/andreea-511/cplusplusprojects/blob/main/son-brother/example.txt) offered as a tree graph representation example in order to test the source code can be found [here](https://github.com/andreea-511/cplusplusprojects/blob/main/son-brother/example.txt).

### Observation 2:
On the first line of the .txt file there are two additional numbers that represent the number of vertices and the root of the tree graph respectively. As mentioned above, this information is not required but it is often provided in problems.

## How I solved it
**The source code for this problem can be found [here](https://github.com/andreea-511/cplusplusprojects/blob/main/son-brother/son-brother.cpp).**

In order to solve the problem I created a new vector that has the parent of all vertices except the root which has -1. By reading the vector you can find the parent of any given vertex. But how do we make this vector?

The logic is as follows: 
- *if the son of a vertex is non-zero* then that vertex is the parents of its son.
> Example: if 2 has the son 5 then 5 has the parent 2

- *if a vertex has brothers* then its brothers will have the same parent as the original vertex
> Example: if 3 has the brother 4 and the parent 1 then 4 will have 1 as a parent as well

Using these two assessments we can compute the parent vector easily.
