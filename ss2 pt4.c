#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#define MAX_N 100005
#define INF INT_MAX/2
typedef struct Edge {
    int to, weight;
    struct Edge* next;
} Edge;
Edge* graph[MAX_N];
int N, M, K;
bool blocked[MAX_N];
typedef struct {
    int dist, node;
} PQNode;
PQNode pq[2*MAX_N];
int pq_size = 0;
void pq_push(int dist, int node) {
    pq[pq_size].dist = dist;
    pq[pq_size].node = node;
    pq_size++;
    int i = pq_size - 1;
    while (i > 0) {
        int parent = (i-1)/2;
        if (pq[parent].dist <= pq[i].dist) break;
        PQNode temp = pq[parent];
        pq[parent] = pq[i];
        pq[i] = temp;
        i = parent;
    }
}
PQNode pq_pop() {
    PQNode result = pq[0];
    pq[0] = pq[--pq_size];
    int i = 0;
    while (1) {
        int left = 2*i + 1, right = 2*i + 2;
        int smallest = i;
        if (left < pq_size && pq[left].dist < pq[smallest].dist)
            smallest = left;
        if (right < pq_size && pq[right].dist < pq[smallest].dist)
            smallest = right;
        if (smallest == i) break;
        PQNode temp = pq[i];
        pq[i] = pq[smallest];
        pq[smallest] = temp;
        i = smallest;
    }
    return result;
}
bool pq_empty() { return pq_size == 0; }
void add_edge(int u, int v, int w) {
    Edge* edge = malloc(sizeof(Edge));
    edge->to = v;
    edge->weight = w;
    edge->next = graph[u];
    graph[u] = edge;
}
int dijkstra(int src, int dest) {
    int dist[MAX_N];
    bool visited[MAX_N];
    int i;
    for (i = 0; i < MAX_N; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[src] = 0;
    pq_size = 0;
    pq_push(0, src);
    while (!pq_empty()) {
        PQNode curr = pq_pop();
        int u = curr.node;
        if (visited[u]) continue;
        visited[u] = true;
        if (u == dest) return dist[u];
        Edge* e;
        for (e = graph[u]; e != NULL; e = e->next) {
            int v = e->to;
            if (blocked[v]) continue;
            int new_dist = dist[u] + e->weight;
            if (new_dist < dist[v]) {
                dist[v] = new_dist;
                pq_push(new_dist, v);
            }
        }
    }
    return (dist[dest] == INF) ? -1 : dist[dest];
}
int count_alternate_paths(int src, int dest) {
    bool visited[MAX_N];
    int level[MAX_N];
    int count = 0;
    int i;
    for (i = 0; i < MAX_N; i++) {
        visited[i] = false;
        level[i] = -1;
    }
    level[src] = 0;
    if (src == dest) return 1;
    int queue[MAX_N];
    int front = 0, rear = 0;
    queue[rear++] = src;
    while (front < rear) {
        int u = queue[front++];
        Edge* e;
        for (e = graph[u]; e != NULL; e = e->next) {
            int v = e->to;
            if (blocked[v] || visited[v]) continue;
            if (level[v] == -1) {
                level[v] = level[u] + 1;
                queue[rear++] = v;
                if (v == dest) count++;
            }
        }
        visited[u] = true;
    }
    return (level[dest] == -1) ? 0 : count;
}
int main() {
    scanf("%d %d", &N, &M);
    int i;
    for (i = 0; i < M; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        add_edge(u, v, w);
        add_edge(v, u, w);
    }
    scanf("%d", &K);
    memset(blocked, 0, sizeof(blocked));
    for (i = 0; i < K; i++) {
        int node;
        scanf("%d", &node);
        blocked[node] = true;
    }
    int S, D;
    scanf("%d %d", &S, &D);
    int min_time = dijkstra(S, D);
    int alt_paths = count_alternate_paths(S, D);
    printf("%d\n", min_time);
    printf("%d\n", alt_paths);w
    return 0;
}
