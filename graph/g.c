#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define false 0
#define true 1

typedef int bool;

// A vertex is a 2D point
typedef struct Vertex {
	int x; // x-coordinate
	int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
} Edge;
typedef struct VertexNode {
	Vertex *v;
	struct VertexNode *Next;
} VertexNode;
typedef struct VertexList {
	Vertex* origin;
	VertexNode *header;
	struct VertexList *Next;
} VertexList;
typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexList* listHeader; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;
// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

Vertex* newVertex(int x, int y) {
	Vertex* v = (Vertex*)malloc(sizeof(Vertex));
	v->x = x;
	v->y = y;
	return v;
}

VertexNode* newVertexNode(Vertex* v) {
	VertexNode* vn = (VertexNode*)malloc(sizeof(VertexNode));
	vn->v = v;
	vn->Next = NULL;
	return vn;
}

VertexList* newVertexList(Vertex* v, VertexNode* vn) {
	VertexList* vl = (VertexList*)malloc(sizeof(VertexList));
	vl->origin = v;
	vl->header = vn;
	vl->Next = NULL;
	return vl;
}

// Add the time complexity analysis of CreateEmptyGraph() here
// Time complexity: O(1)
Graph CreateEmptyGraph()
{
	GraphRep* grp = (GraphRep*)malloc(sizeof(GraphRep));
	grp->listHeader = NULL;
	grp->nE = grp->nV = 0;
	return grp;
}

// Add the time complexity analysis of InsertEdge() here
// Time complexity: O(m+n) m represents the number of edges and n represents the number of vertices
int InsertEdge(Graph g, Edge *e)
{
	bool v1_existed = false, v2_existed = false, edgeExisted = false;
	if (g->listHeader == NULL) {
		g->nV += 2;
		g->nE++;
		VertexNode* vn1 = newVertexNode(e->p1);
		VertexList* vl1 = newVertexList(e->p2, vn1);
		VertexNode* vn2 = newVertexNode(e->p2);
		vl1->Next = newVertexList(e->p1, vn2);
		g->listHeader = vl1;
		return 1;
	}
	VertexList* tempList = g->listHeader;
	while (tempList) {
		if (tempList->origin->x == e->p1->x && tempList->origin->y == e->p1->y) {
			v1_existed = true;
			VertexNode* tempNode = tempList->header;
			while (tempNode) {
				if (tempNode->v->x == e->p2->x && tempNode->v->y == e->p2->y) {
					v2_existed = true;
					edgeExisted = true;
					break;
				}
				tempNode = tempNode->Next;
			}
			if (!edgeExisted) {
				VertexNode* vn_2 = newVertexNode(e->p2);
				vn_2->Next = tempList->header;
				tempList->header = vn_2;
			}
		}
		if (edgeExisted)break;
		if (tempList->origin->x == e->p2->x && tempList->origin->y == e->p2->y) {
			v2_existed = true;
			VertexNode* tempNode = tempList->header;
			while (tempNode) {
				if (tempNode->v->x == e->p1->x && tempNode->v->y == e->p1->y) {
					v1_existed = true;
					edgeExisted = true;
					break;
				}
				tempNode = tempNode->Next;
			}
			if (!edgeExisted) {
				VertexNode* vn_1 = newVertexNode(e->p1);
				vn_1->Next = tempList->header;
				tempList->header = vn_1;
			}
		}
		if (edgeExisted)break;
		tempList = tempList->Next;
	}
	if (edgeExisted)return 0;
	else if (v1_existed && v2_existed)g->nE++;
	else if (v1_existed && !v2_existed) {
		VertexNode* add_vn1 = newVertexNode(e->p1);
		VertexList* newV2 = newVertexList(e->p2, add_vn1);
		newV2->Next = g->listHeader;
		g->listHeader = newV2;
		g->nE++; 
		g->nV++;
	}
	else if (!v1_existed && v2_existed) {
		VertexNode* add_vn2 = newVertexNode(e->p2);
		VertexList* newV1 = newVertexList(e->p1, add_vn2);
		newV1->Next = g->listHeader;
		g->listHeader = newV1;
		g->nE++;
		g->nV++;
	}
	else {
		g->nE++;
		g->nV += 2;
		VertexNode* vn1 = newVertexNode(e->p1);
		VertexList* vl1 = newVertexList(e->p2, vn1);
		vl1->Next = g->listHeader;
		g->listHeader = vl1;
		VertexNode* vn2 = newVertexNode(e->p2);
		VertexList* vl2 = newVertexList(e->p1, vn2);
		vl2->Next = g->listHeader;
		g->listHeader = vl2;
	}
	return 1;
}

bool DeleteVertexNode(VertexList* vl, Vertex* v) {
	VertexNode *tempNode = vl->header, *pre = NULL;
	if (tempNode->v->x == v->x && tempNode->v->y == v->y) {
		vl->header = tempNode->Next;
		free(tempNode);
		return true;
	}
	while (tempNode) {
		if (tempNode->v->x == v->x && tempNode->v->y == v->y) {
			pre->Next = tempNode->Next;
			free(tempNode);
			return true;
		}
		pre = tempNode;
		tempNode = tempNode->Next;
	}
	return false;
}

// Add the time complexity analysis of DeleteEdge() here
// Time complexity: O(m+n) m represents the number of edges and n represents the number of vertices
void DeleteEdge(Graph g, Edge *e)
{
	VertexList* tempList = g->listHeader;
	bool existed = false;
	int deleteCnt = 0;
	while (tempList) {
		if (tempList->origin->x == e->p1->x && tempList->origin->y == e->p1->y) {
			existed = DeleteVertexNode(tempList, e->p2);
			deleteCnt++;
		}
		if (tempList->origin->x == e->p2->x && tempList->origin->y == e->p2->y) {
			existed = DeleteVertexNode(tempList, e->p1);
			deleteCnt++;
		}
		if (deleteCnt == 2)break;
		tempList = tempList->Next;
	}
	if (existed)(g->nE)--;
}

typedef struct Queue {
	VertexNode* qhead;
	int size;
} Queue;

Queue* newQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->qhead = NULL;
	q->size = 0;
}

VertexNode* removeFront(Queue* q) {
	if (q->size == 0)return NULL;
	VertexNode* temp = q->qhead;
	q->qhead = q->qhead->Next;
	q->size--;
	return temp;
}

void push_back(Queue* q, VertexNode* vn) {
	if (q->qhead == NULL) {
		q->qhead = vn;
		q->size++;
		return;
	}
	VertexNode* temp = q->qhead;
	while (temp->Next) {
		temp = temp->Next;
	}
	temp->Next = vn;
	q->size++;
}

int getRank(VertexList* vl, Vertex* v) {
	int cnt = 0;
	VertexList* temp = vl;
	while (temp) {
		if (temp->origin->x == v->x && temp->origin->y == v->y) return cnt;
		temp = temp->Next;
		cnt++;
	}
	return -1;
}

VertexList* getVertexList(VertexList* vl, int idx) {
	if (vl == NULL)return NULL;
	int cnt = 0;
	while (vl && cnt < idx) {
		vl = vl->Next;
		cnt++;
	}
	return vl;
}

VertexList* getListByNode(VertexList* vl, VertexNode* vn) {
	while (vl) {
		if (vl->origin->x == vn->v->x && vl->origin->y == vn->v->y)return vl;
		vl = vl->Next;
	}
	return NULL;
}

// Add the time complexity analysis of ReachableVertices() here
// Time complexity: O(m+n) here m represents the number of edges and n represents the number of vertices
// Method: breadth first traversal and adjacency list used to represent the graph
void ReachableVertices(Graph g, Vertex *v)
{
	bool* printed = (bool*)malloc((g->nV) * sizeof(bool));
	for (int i = 0; i < g->nV; i++)
		printed[i] = false;

	bool* reached = (bool*)malloc((g->nV) * sizeof(bool));
	for (int i = 0; i < g->nV; i++)
		reached[i] = false;
	bool existed = false,first=true;
	VertexList* tempList = g->listHeader;
	while (tempList) {
		if (tempList->origin->x == v->x && tempList->origin->y == v->y) {
			existed = true;
			break;
		}
		tempList = tempList->Next;
	}
	if (!existed)return;
	Queue* que = newQueue();
	push_back(que, newVertexNode(v));
	while (que->size) {
		VertexNode* front = removeFront(que);
		int rank = getRank(g->listHeader, front->v);

		if (reached[rank])continue;
		reached[rank] = true;
		tempList = getListByNode(g->listHeader, front);
		if (tempList == NULL)continue;
		VertexNode* t1 = tempList->header;
		while (t1) {
			if (!reached[getRank(g->listHeader, t1->v)]) {
				push_back(que, newVertexNode(t1->v));
				int idx = getRank(g->listHeader, t1->v);
				if (first && !printed[idx]) {
					printf("(%d,%d)", t1->v->x, t1->v->y);
					first = false;
					printed[idx] = true;
				}
				else if(!printed[idx]){
					printf(",(%d,%d)", t1->v->x, t1->v->y);
					printed[idx] = true;
				}
			}
			t1 = t1->Next;
		}
	}
	printf("\n");
}

VertexList* getMinDist(Graph g, double* d, bool* v) {
	VertexList* vl = NULL;
	double minDist = 999999999;
	int idx = -1;
	for (int i = 0; i < g->nV; i++) {
		if (!v[i] && d[i] < minDist) {
			minDist = d[i];
			vl = getVertexList(g->listHeader, i);
			idx = i;
		}
	}
	if (idx != -1)v[idx] = true;
	return vl;
}

double calculateDistance(Vertex* a, Vertex* b) {
	return sqrt((b->x - a->x)*(b->x - a->x) + (b->y - a->y)*(b->y - a->y));
}

void printShortestPath(Graph g,Vertex* pre,Vertex* cur,Vertex* start) {
	if (cur->x == start->x && cur->y == start->y)return;
	int idx = getRank(g->listHeader, cur);
	Vertex* temp = &pre[idx];
	printShortestPath(g, pre, temp, start);
	printf(",(%d,%d)", cur->x, cur->y);
}

// Add the time complexity analysis of ShortestPath() here
// Time complexity: O(m*log(n))
void ShortestPath(Graph g, Vertex *u, Vertex *v)    //djikstra's algorithm
{
	bool uExist = false, vExist = false;
	VertexList* tempList = g->listHeader;
	VertexList* startList = NULL;
	while (tempList) {
		if (tempList->origin->x == u->x && tempList->origin->y == u->y) {
			uExist = true;
			startList = tempList;
		}
		if (tempList->origin->x == v->x && tempList->origin->y == v->y) {
			vExist = true;
		}
		if (uExist && vExist)break;
		tempList = tempList->Next;
	}
	if (!uExist || !vExist)return;

	Vertex *pre = (Vertex*)malloc((g->nV) * sizeof(Vertex));
	tempList = g->listHeader;
	for (int i = 0; i < g->nV; i++) {
		pre[i] = *(tempList->origin);
		if (tempList)break;
		tempList = tempList->Next;
	}
	double *dist = (double*)malloc((g->nV) * sizeof(double));
	for (int i = 0; i < g->nV; i++)
		dist[i] = 999999999;
	dist[getRank(g->listHeader, startList->origin)] = 0.0;
	bool *visited = (bool*)malloc((g->nV) * sizeof(bool));
	for (int i = 0; i < g->nV; i++)
		visited[i] = false;
	double tempDist = 0.0;

	for (int i = 0; i <(g->nV)-1; i++) {                
		tempList = getMinDist(g, dist, visited);
		if (tempList == NULL)continue;
		VertexNode* vn = tempList->header;
		while (vn) {
			int idx = getRank(g->listHeader, vn->v);
			if (visited[idx]) { vn = vn->Next; continue; }
			tempDist = calculateDistance(tempList->origin, vn->v);
			if (dist[getRank(g->listHeader,tempList->origin)]+tempDist < dist[idx]) {
				dist[idx] = dist[getRank(g->listHeader, tempList->origin)] + tempDist;
				pre[idx] = *(tempList->origin);
			}
			vn = vn->Next;
		}
	}

	if (dist[getRank(g->listHeader, v)] == 999999999)return;
	else {
		printf("(%d,%d)", u->x, u->y);
		printShortestPath(g, pre, v, u);
		printf("\n");
	}
}

// Add the time complexity analysis of FreeGraph() here
// Time complexity: O(n+m) m represents the number of edges and n represents the number of vertices
void FreeGraph(Graph g)
{
	VertexList* vl = g->listHeader;
	while (vl) {
		VertexNode* vn = vl->header;
		while (vn) {
			Vertex* v = vn->v;
			free(v);
			v = NULL;
			VertexNode* tempNode = vn;
			vn = vn->Next;
			free(tempNode);
			tempNode = NULL;
		}
		VertexList* tempList = vl;
		vl = vl->Next;
		free(tempList);
		tempList = NULL;
	}
	free(g);
	g = NULL;
}



// Add the time complexity analysis of ShowGraph() here
// Time complexity: O(n+m) n represents the number of vertices and m represents the number of edges
// Method: breadth first traversal and adjacency list represented graph
void ShowGraph(Graph g)
{
	if (g->nE == 0 || g->nV == 0 || g->listHeader == NULL)return;
	bool first = true;
	bool *visited = (bool*)malloc((g->nV) * sizeof(bool));
	for (int i = 0; i < g->nV; i++)
		visited[i] = false;

	for (int i = 0; i < g->nV; i++) {
		if (visited[i])continue;
		VertexList* temp = getVertexList(g->listHeader, i);
		if (temp == NULL)continue;
		Queue* que = newQueue();
		push_back(que, newVertexNode(temp->origin));
		while (que->size) {
			VertexNode* front = removeFront(que);

			int rank = getRank(g->listHeader, front->v);
			if (visited[rank])continue;
			visited[rank] = true;
			temp = getListByNode(g->listHeader, front);
			if (temp == NULL)continue;
			VertexNode* t1 = temp->header;
			while (t1) {
				if (!visited[getRank(g->listHeader, t1->v)]) {
			
					push_back(que, newVertexNode(t1->v));
					if (first) {
						printf("(%d,%d),(%d,%d)", temp->origin->x, temp->origin->y, t1->v->x, t1->v->y);
						first = false;
					}
					else {
						printf(" (%d,%d),(%d,%d)", temp->origin->x, temp->origin->y, t1->v->x, t1->v->y);
					}
				}
				t1 = t1->Next;
			}
		}
	}
	printf("\n");
}

void printStructure(Graph g) {
	VertexList* temp = g->listHeader;
	while (temp) {
		Vertex* x = temp->origin;
		printf("origin is %d,%d ", x->x, x->y);
		VertexNode* vn = temp->header;
		while (vn) {
			printf(" (%d,%d) ", vn->v->x, vn->v->y);
			vn = vn->Next;
		}
		printf(" rank is %d", getRank(g->listHeader, x));
		printf("\n");
		temp = temp->Next;
	}
}

int main() //sample main for testing 
{
	Graph g1;
	Edge *e_ptr;
	Vertex *v1, *v2;

	// Create an empty graph g1;
	g1 = CreateEmptyGraph();

	// Create first connected component 
	// Insert edge (0,0)-(0,10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 0;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0,0)-(5,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 5;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0, 10)-(10, 10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 10;
	v2->x = 10;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0,10)-(5,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 10;
	v2->x = 5;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0,0)-(5,4)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 5;
	v2->y = 4;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (5, 4)-(10, 4)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 5;
	v1->y = 4;
	v2->x = 10;
	v2->y = 4;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (5,6)-(10,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 5;
	v1->y = 6;
	v2->x = 10;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (10,10)-(10,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 10;
	v1->y = 10;
	v2->x = 10;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (10, 6)-(10, 4)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 10;
	v1->y = 6;
	v2->x = 10;
	v2->y = 4;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Create second connected component
	// Insert edge (20,4)-(20,10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 20;
	v1->y = 4;
	v2->x = 20;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (20,10)-(30,10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 20;
	v1->y = 10;
	v2->x = 30;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (25,5)-(30,10) 	
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 25;
	v1->y = 5;
	v2->x = 30;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	//Display graph g1
	ShowGraph(g1);

	// Find the shortest path between (0,0) and (10,6) 
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 10;
	v2->y = 6;
	ShortestPath(g1, v1, v2);
	free(v1);
	free(v2);

	// Delete edge (0,0)-(5, 6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 5;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	DeleteEdge(g1, e_ptr);
	free(e_ptr);
	free(v1);
	free(v2);

	// Display graph g1
	ShowGraph(g1);

	// Find the shortest path between (0,0) and (10,6) 
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 10;
	v2->y = 6;
	ShortestPath(g1, v1, v2);
	free(v1);
	free(v2);

	// Find the shortest path between (0,0) and (25,5)
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex *)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 25;
	v2->y = 5;
	ShortestPath(g1, v1, v2);
	free(v1);
	free(v2);

	// Find reachable vertices of (0,0)
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v1->x = 0;
	v1->y = 0;
	ReachableVertices(g1, v1);
	free(v1);

	// Find reachable vertices of (20,4)
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v1->x = 20;
	v1->y = 4;

	ReachableVertices(g1, v1);
	free(v1);

	// Free graph g1
	FreeGraph(g1);

	getchar();
	getchar();
	return 0;
}
