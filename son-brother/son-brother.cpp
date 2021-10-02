#include <conio.h>
#include <stdio.h>
//struct for son-brother tree graph represenation
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
