#include <conio.h>
#include <stdio.h>

//Consider a tree graph represented using the Son-Brother method. \
Find the parent of a given vertex x.

//Write a function that solves the problem. Explain (via comments) \
how you want to solve the problem. Write a main program that reads the \
tree representation from a text file, applies the above function on it \
and displays the results.

//struct for tree
typedef struct {
	int vertices; //number of vertices
	int root; // root of the tree
	int* son; // address of sons vector
	int* brother; // address of brothers vector
} tree;

//input: t - tree struct
//output: parents vector which has the parent for each vertex except the root (the root is the only vertex with parent -1)
int* father(tree t)
{
	int i, *parents;
	parents = new int[t.vertices];
	for (i = 0; i < t.vertices; i++)
		parents[i] = 0; // we initialize the parents vector with 0
	parents[t.root - 1] = -1; // the root has no parent -> value -1 in the vector

	for (i = 0; i < t.vertices; i++)
		if (t.son[i] != 0)
			parents[t.son[i] - 1] = i + 1;
	
	for (i = 0; i < t.vertices; i++)
		if (parents[i] != 0 && t.brother[i] != 0)
			parents[t.brother[i] - 1] = parents[i];

	return parents;
}

/*EXPLANATIONS: 
The solution to the problem is a vector of parents. The way this vector was computed is as follows:
if the son of a vertex is NON-zero then that vertex is the parent of their son (lines 29-30)
if a vertex has a parent and has brothers as well, all of its brothers will have the same parent (lines 33-34)
at the end, to find the parent of a vertex we simply have to check the parents vector;

the text file that I worked on:

16 1
2 5 0 8 0 9 0 14 0 0 0 0 0 0 0 0
0 3 4 0 6 7 0 0 10 11 12 13 0 15 16 0

first line contains the number of vertices and the root
second line is the sons vector
third line is the brothers vector

OBSERVATION: the root of the tree can be deducted from the vectors as well, it is the only value that doesn't appear
in either vectors -> in our case we can clearly see that 1 isn't included in the sons vector nor the brothers vector
*/



void main()
{
	char name[80];
	int i, *parents, vertex;
	tree t;
	FILE* f;

	printf("Name of the file where the tree can be found: ");
	gets_s(name);

	f = fopen(name, "r");

	if (!f)
		printf("\nFile %s could not be opened.\n", name);
	else
	{
		fscanf_s(f,"%d %d", &t.vertices, &t.root);
		t.son = new int[t.vertices];
		t.brother = new int[t.vertices];

		printf("\nThe sons vector is:\n");
		for (i = 0; i < t.vertices; i++)
		{
			fscanf_s(f, "%d", &t.son[i]);
			printf("%d ", t.son[i]);
		}

		printf("\n\nThe brothers vector is:\n");
		for (i = 0; i < t.vertices; i++)
		{
			fscanf_s(f, "%d", &t.brother[i]);
			printf("%d ", t.brother[i]);
		}

		parents = father(t);

		printf("\n\n\tEnter the vertex whose parent you want to find: ");
		scanf_s("%d", &vertex);

		while (!feof(stdin))
		{
			while ((vertex < 1 || vertex > t.vertices) && !feof(stdin)) //while loop to check if the vertex value is valid
			{
				printf("\nInvalid vertex value. Try again (1-%d) (or CTRL+Z to cancel): ", t.vertices);
				scanf_s("%d", &vertex);
			}

			if (!feof(stdin))
			{
				if (parents[vertex - 1] == -1)
					printf("\nVertex %d is the root of the tree\n", vertex);
				else
					printf("\nThe parent of vertex %d is vertex %d\n", vertex, parents[vertex - 1]);

				printf("\n\n\tEnter another vertex (or CTRL+Z to cancel): ");
				scanf_s("%d", &vertex);
			}
		}

		delete[] t.son;
		delete[] t.brother;
		delete[] parents;
		t.son = NULL;
		t.brother = NULL;
		parents = NULL;

		fclose(f);
	}
	printf("\nDone. Press any key to continue...\n");
	_getch();
}
