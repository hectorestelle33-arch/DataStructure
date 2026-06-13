#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 100

typedef struct {
    int vexnum;
    int arcnum;
    int arcs[MAX_VERTEX_NUM + 1][MAX_VERTEX_NUM + 1];
} MGraph;

typedef struct ArcNode {
    int adjvex;
    struct ArcNode *nextarc;
} ArcNode;

typedef struct {
    int vertex;
    ArcNode *firstarc;
} VNode;

typedef struct {
    int vexnum;
    int arcnum;
    VNode vertices[MAX_VERTEX_NUM + 1];
} ALGraph;

typedef struct {
    int data[MAX_VERTEX_NUM + 1];
    int front;
    int rear;
} Queue;

void InitQueue(Queue *q)
{
    q->front = 0;
    q->rear = 0;
}

int QueueEmpty(Queue *q)
{
    return q->front == q->rear;
}

void EnQueue(Queue *q, int value)
{
    q->data[q->rear++] = value;
}

int DeQueue(Queue *q)
{
    return q->data[q->front++];
}

void InitMGraph(MGraph *G, int vexnum, int arcnum)
{
    int i, j;

    G->vexnum = vexnum;
    G->arcnum = arcnum;
    for (i = 1; i <= vexnum; ++i) {
        for (j = 1; j <= vexnum; ++j) {
            G->arcs[i][j] = 0;
        }
    }
}

void InitALGraph(ALGraph *G, int vexnum, int arcnum)
{
    int i;

    G->vexnum = vexnum;
    G->arcnum = arcnum;
    for (i = 1; i <= vexnum; ++i) {
        G->vertices[i].vertex = i;
        G->vertices[i].firstarc = NULL;
    }
}

void InsertArcToList(ALGraph *G, int from, int to)
{
    ArcNode *node = (ArcNode *)malloc(sizeof(ArcNode));
    ArcNode *p;

    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    node->adjvex = to;
    node->nextarc = NULL;

    if (G->vertices[from].firstarc == NULL ||
        to < G->vertices[from].firstarc->adjvex) {
        node->nextarc = G->vertices[from].firstarc;
        G->vertices[from].firstarc = node;
        return;
    }

    p = G->vertices[from].firstarc;
    while (p->nextarc != NULL && p->nextarc->adjvex < to) {
        p = p->nextarc;
    }
    node->nextarc = p->nextarc;
    p->nextarc = node;
}

void CreateGraph(MGraph *MG, ALGraph *ALG)
{
    int vexnum, arcnum;
    int i;

    printf("Input vertex count n and edge count e: ");
    if (scanf("%d%d", &vexnum, &arcnum) != 2) {
        printf("Input error.\n");
        exit(1);
    }

    if (vexnum <= 0 || vexnum > MAX_VERTEX_NUM || arcnum < 0) {
        printf("Invalid graph size.\n");
        exit(1);
    }

    InitMGraph(MG, vexnum, arcnum);
    InitALGraph(ALG, vexnum, arcnum);

    printf("Input each directed edge as: from to\n");
    for (i = 0; i < arcnum; ++i) {
        int from, to;

        if (scanf("%d%d", &from, &to) != 2) {
            printf("Input error.\n");
            exit(1);
        }
        if (from < 1 || from > vexnum || to < 1 || to > vexnum) {
            printf("Invalid edge: %d -> %d\n", from, to);
            exit(1);
        }

        MG->arcs[from][to] = 1;
        InsertArcToList(ALG, from, to);
    }
}

int FirstAdjVex_M(MGraph G, int v)
{
    int i;

    if (v < 1 || v > G.vexnum) {
        return 0;
    }

    for (i = 1; i <= G.vexnum; ++i) {
        if (G.arcs[v][i] != 0) {
            return i;
        }
    }

    return 0;
}

int NextAdjVex_M(MGraph G, int v, int w)
{
    int i;

    if (v < 1 || v > G.vexnum) {
        return 0;
    }

    for (i = w + 1; i <= G.vexnum; ++i) {
        if (G.arcs[v][i] != 0) {
            return i;
        }
    }

    return 0;
}

int FirstAdjVex_AL(ALGraph G, int v)
{
    if (v < 1 || v > G.vexnum || G.vertices[v].firstarc == NULL) {
        return 0;
    }

    return G.vertices[v].firstarc->adjvex;
}

int NextAdjVex_AL(ALGraph G, int v, int w)
{
    ArcNode *p;

    if (v < 1 || v > G.vexnum) {
        return 0;
    }

    p = G.vertices[v].firstarc;
    while (p != NULL && p->adjvex != w) {
        p = p->nextarc;
    }
    if (p != NULL && p->nextarc != NULL) {
        return p->nextarc->adjvex;
    }

    return 0;
}

void DFS_M(MGraph G, int v, int visited[])
{
    int w;

    visited[v] = 1;
    printf("%d ", v);

    for (w = FirstAdjVex_M(G, v); w != 0; w = NextAdjVex_M(G, v, w)) {
        if (!visited[w]) {
            DFS_M(G, w, visited);
        }
    }
}

void DFSTraverse_M(MGraph G)
{
    int i;
    int visited[MAX_VERTEX_NUM + 1] = {0};

    for (i = 1; i <= G.vexnum; ++i) {
        if (!visited[i]) {
            DFS_M(G, i, visited);
        }
    }
}

void BFSTraverse_M(MGraph G)
{
    int i, w;
    int visited[MAX_VERTEX_NUM + 1] = {0};
    Queue q;

    InitQueue(&q);
    for (i = 1; i <= G.vexnum; ++i) {
        if (!visited[i]) {
            visited[i] = 1;
            printf("%d ", i);
            EnQueue(&q, i);

            while (!QueueEmpty(&q)) {
                int v = DeQueue(&q);
                for (w = FirstAdjVex_M(G, v); w != 0; w = NextAdjVex_M(G, v, w)) {
                    if (!visited[w]) {
                        visited[w] = 1;
                        printf("%d ", w);
                        EnQueue(&q, w);
                    }
                }
            }
        }
    }
}

void DFS_AL(ALGraph G, int v, int visited[])
{
    int w;

    visited[v] = 1;
    printf("%d ", v);

    for (w = FirstAdjVex_AL(G, v); w != 0; w = NextAdjVex_AL(G, v, w)) {
        if (!visited[w]) {
            DFS_AL(G, w, visited);
        }
    }
}

void DFSTraverse_AL(ALGraph G)
{
    int i;
    int visited[MAX_VERTEX_NUM + 1] = {0};

    for (i = 1; i <= G.vexnum; ++i) {
        if (!visited[i]) {
            DFS_AL(G, i, visited);
        }
    }
}

void BFSTraverse_AL(ALGraph G)
{
    int i, w;
    int visited[MAX_VERTEX_NUM + 1] = {0};
    Queue q;

    InitQueue(&q);
    for (i = 1; i <= G.vexnum; ++i) {
        if (!visited[i]) {
            visited[i] = 1;
            printf("%d ", i);
            EnQueue(&q, i);

            while (!QueueEmpty(&q)) {
                int v = DeQueue(&q);
                for (w = FirstAdjVex_AL(G, v); w != 0; w = NextAdjVex_AL(G, v, w)) {
                    if (!visited[w]) {
                        visited[w] = 1;
                        printf("%d ", w);
                        EnQueue(&q, w);
                    }
                }
            }
        }
    }
}

void PrintMGraph(MGraph G)
{
    int i, j;

    printf("\nAdjacency matrix:\n");
    printf("    ");
    for (i = 1; i <= G.vexnum; ++i) {
        printf("%3d", i);
    }
    printf("\n");

    for (i = 1; i <= G.vexnum; ++i) {
        printf("%3d:", i);
        for (j = 1; j <= G.vexnum; ++j) {
            printf("%3d", G.arcs[i][j]);
        }
        printf("\n");
    }
}

void PrintALGraph(ALGraph G)
{
    int i;

    printf("\nAdjacency list:\n");
    for (i = 1; i <= G.vexnum; ++i) {
        ArcNode *p = G.vertices[i].firstarc;

        printf("%d:", i);
        while (p != NULL) {
            printf(" -> %d", p->adjvex);
            p = p->nextarc;
        }
        printf("\n");
    }
}


int main(void)
{
    MGraph MG;
    ALGraph ALG;
    int v;

    CreateGraph(&MG, &ALG);

    PrintMGraph(MG);
    PrintALGraph(ALG);

    printf("\nInput v to test FirstAdjVex(G, v): ");
    if (scanf("%d", &v) == 1) {
        printf("Matrix FirstAdjVex(G, %d) = %d\n", v, FirstAdjVex_M(MG, v));
        printf("List   FirstAdjVex(G, %d) = %d\n", v, FirstAdjVex_AL(ALG, v));
    }

    printf("\nMatrix DFS: ");
    DFSTraverse_M(MG);
    printf("\nMatrix BFS: ");
    BFSTraverse_M(MG);

    printf("\nList DFS  : ");
    DFSTraverse_AL(ALG);
    printf("\nList BFS  : ");
    BFSTraverse_AL(ALG);
    printf("\n");

    return 0;
}
