#include <conio.h>
#include <stdio.h>
#include <string.h>

#define INF 10000 //infinite global variable
//this is used for when we have weighted graphs -> when two vertices are not connected, in a weighted graph their weight will be infinite

//			-----------------graph and queue structs-----------------
//graph struct 
typedef struct {
	int vertices; // no. of vertices
	int edges; // no. of edges
	int** adj; // address of adjacency matrix - dynamically allocated
	int** table; // address of table matrix - dynamically allocated | 2 columns for unweighted; 3 columns for weighted
	int orient; // 0 - no; 1 - yes
	int weight; // 0 - no; 1 - yes
} graph;

//solution struct
typedef struct {
	int **matrix;
	int edges; 
} solution;

//			-----------------dynamically allocated matrix-----------------
//			~~~display a matrix~~~
//input: a - matrix address; l - no. of lines; c - no. of columns; t - title*
// * we set the parameter to 0 as default so that we can check if we want a title or not before displaying it
//output: -
void display(int** a, int l, int c, const char* t = 0)
{
	int i, j;
	printf("\n\n%s\n\n", t);
	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++)
			if (a[i][j] == INF)
				printf("INF ");
			else
				printf("%3d ", a[i][j]);
		printf("\n");
	}
}

//			~~~dynamically allocate a matrix~~~
//input: l - no. of lines; c - no. of columns;
//output: a - matrix address
int** allocate(int l, int c)
{
	int** a;
	int i;
	a = new int* [l];
	for (i = 0; i < l; i++)
		a[i] = new int[c];
	return a;
}

//			~~~deallocate a matrix~~~
//input: a - matrix address; l - no. of lines;
//output: a - matrix address that is set to NULL
void deallocate(int**& a, int l)
{
	int i;
	for (i = 0; i < l; i++)
		delete[] a[i];
	delete[] a;
	a = NULL; // set the matrix address to NULL
}


//			-----------------reading a graph from a text file-----------------
//			~~~convert table representation to adjacency matrix~~~
//input: g - graph
//output: g - graph
void convertMatrix(graph& g)
{
	int i, j, value; // value = the value we add in the table if there is no edge: INF - weighted; 0 - not weighted
	g.adj = allocate(g.vertices, g.vertices); // allocate space for the adjacency matrix
	if (g.weight == 0)
		value = 0;
	else
		value = INF;
	for (i = 0; i < g.vertices; i++)
		for (j = 0; j < g.vertices; j++)
			g.adj[i][j] = value;

	for (i = 0; i < g.edges; i++) // reading the table representation of the graph
	// structure: g.table[i][0] - start vertex; g.table[i][1] - end vertex; g.table[i][2] - weight (1 if no weight)
	{
		if (g.weight == 0)
			value = 1;
		else
			value = g.table[i][2];
		g.adj[g.table[i][0] - 1][g.table[i][1] - 1] = value;
		if (g.orient == 0)
			g.adj[g.table[i][1] - 1][g.table[i][0] - 1] = value; // if not oriented -> symmetric matrix
	}
}

//			~~~reading a graph from the text file~~~
//input: name - name of file; weight - weighted? (y/n); orient - oriented? (y/n)
//output: error: 0 = succes; 1 = error; ; g - graph (struct)
//by default we consider the graph NOT weighted and NOT oriented
int readGraph(char* name, graph& g, char weight = 'n', char orient = 'n')
{
	FILE* f;
	int i, j, error;
	f = fopen(name, "r");
	if (f == 0)
		error = 1; // file could not be opened;
	else
	{
		error = 0; // file is readable
		if (weight == 'n')
			g.weight = 0;
		else
			g.weight = 1;
		if (orient == 'n')
			g.orient = 0;
		else
			g.orient = 1;

		fscanf_s(f, "%d %d", &g.vertices, &g.edges); // read no. of vertices and edges
		g.table = allocate(g.edges, g.weight + 2); // allocating space for table representation
		for (i = 0; i < g.edges; i++)
			for (j = 0; j < g.weight + 2; j++)
				fscanf_s(f, "%d", &g.table[i][j]);

		convertMatrix(g);
		fclose(f);
	}
	return error;
}

//convert table matrix to adjacency matrix

int** convert(int** table, int edges, int vertices, int orient)
{
	int** adj, i, j;
	adj = allocate(vertices, vertices);
	for (i = 0; i < vertices; i++)
		for (j = 0; j < vertices; j++)
			adj[i][j] = 0;
	for (i = 0; i < edges; i++) 
	{
		adj[table[i][0] - 1][table[i][1] - 1] = 1;
		if (orient == 0)
			adj[table[i][1] - 1][table[i][0] - 1] = 1; // if not oriented -> symmetric matrix
	}

	return adj;
}

//compute the path existance matrix (Roy-Warshall)
int** roywarshall(int** adj, int n)
{
	int i, j, k;
	int** copy;
	copy = allocate(n, n);

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			copy[i][j] = adj[i][j];

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (copy[i][j] != 0)
				for (k = 0; k < n; k++)
					if ((copy[i][k] < copy[k][j]) || (copy[i][k] > copy[k][j]))
						copy[i][k] = copy[k][j] = 1;
	return copy;
}

//function to copy matrix b to address of matrix a
int** copy( int **b, int lin, int col)
{
	int i, j, **a;
	a=allocate(lin, col);
	for (i = 0; i < lin; i++)
		for (j = 0; j < col; j++)
			a[i][j] = b[i][j];
	return a;
}

solution subgraph(graph g)
{
	int** aux, max, i, j, k, ** roy, edges, ** adj, ok;
	solution sol;
	max = 0;
	aux = allocate(g.edges, 2);
	i = 1; 

	while (i < g.edges - 1)
	{
		aux = copy(g.table, g.edges, 2);
		edges = g.edges;
		j = i;
		ok = 1;
		while (j < g.edges - 1 && ok == 1)
		{
			for (k = i + 1; k < edges; k++)
			{
				aux[k - 1][0] = aux[k][0];
				aux[k - 1][1] = aux[k][1];
			}
			edges--;
			adj = allocate(g.vertices, g.vertices);
			adj = convert(aux, edges, g.vertices, g.orient);
			roy = allocate(g.vertices, g.vertices);
			roy = roywarshall(adj, g.vertices);

			if (roy[g.table[0][0] - 1][g.table[g.edges - 1][1] - 1] == 0)
				ok = 0;
			else
				if (j - i + 1 > max)
				{
					if (max != 0)
						deallocate(sol.matrix, sol.edges);
					sol.matrix = allocate(edges, 2);
					sol.matrix = copy(aux, edges, 2);
					sol.edges = edges;
					max = j - i + 1;
				}
			j++;
			deallocate(adj, g.vertices);
			deallocate(roy, g.vertices);
		}
		i++;
	}
	deallocate(aux, g.edges);
	return sol;
}

//			-----------------main programs-----------------
// main for finding the subgraph
void main()
{
	graph g;
	char name[30], weight, orient, useless[80];
	int error, i;
	solution sol;
	printf("\nName of .txt file where the graph can be found: ");
	gets_s(name);
	printf("\n\nWeighted? (y/n) ");
	scanf("%c", &weight);
	gets_s(useless);
	while (weight != 'y' && weight != 'n')
	{
		printf("\nInvalid value. Type << y >> - weighted graph or << n >> - unweighted graph: ");
		scanf("%c", &weight);
		gets_s(useless);
	}
	printf("\n\nOriented? (y/n) ");
	scanf("%c", &orient);
	while (orient != 'y' && orient != 'n')
	{
		printf("\nInvalid value. Type << y >> - oriented graph or << n >> - not oriented graph: ");
		scanf("%c", &orient);
		gets_s(useless);
	}
	error = readGraph(name, g, weight, orient);
	if (error == 1)
		printf("\nThe file could not be opened.\n");
	else
	{
		printf("\nNo. of vertices: %d", g.vertices);
		printf("\nNo. of edges: %d", g.edges);

		display(g.table, g.edges, g.weight + 2, "\t~~~Table representation~~~");
		display(g.adj, g.vertices, g.vertices, "\t~~~Adjacency Matrix~~~");

		sol = subgraph(g);

		display(sol.matrix, sol.edges, 2, "\t~~~Subrgraph~~~");
		printf("\n\nOriginal path is: ");
		for (i = 0; i < g.edges; i++)
			printf("%2d  - ", g.table[i][0]);
		printf("%2d\n",g.table[g.edges - 1][1]);
		printf("\n\nThe optimal path is: ");
		for (i = 0; i < sol.edges; i++)
			printf("%2d  - ", sol.matrix[i][0]);
		printf("%2d", sol.matrix[sol.edges-1][1]);

		deallocate(sol.matrix, sol.edges);
		deallocate(g.table, g.edges);
		deallocate(g.adj, g.vertices);

	}
	printf("\n\nDone, press any key to continue...\n");
	_getch();
}
