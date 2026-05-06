#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ========================= MODULE 1: TOPOLOGY (GRAPH) =========================

#define MAX_DEVICES 20

typedef struct {
    int id;
    char ip[16];
    char name[32];
    int connected; 
} Device;

typedef struct {
    Device devices[MAX_DEVICES];
    int adj[MAX_DEVICES][MAX_DEVICES];
    int num_devices;
} Graph;

void init_graph(Graph* g) {
    g->num_devices = 0;
    int i,j;
    for ( i = 0; i < MAX_DEVICES; i++) {
        g->devices[i].connected = 0;
        for ( j = 0; j < MAX_DEVICES; j++) {
            g->adj[i][j] = 0;
        }
    }
}

int add_device(Graph* g, const char* ip, const char* name) {
    if (g->num_devices >= MAX_DEVICES) return -1;
    int id = g->num_devices++;
    g->devices[id].id = id;
    strcpy(g->devices[id].ip, ip);
    strcpy(g->devices[id].name, name);
    g->devices[id].connected = 1;
    return id;
}

void add_connection(Graph* g, int i, int j) {
    if (i < g->num_devices && j < g->num_devices && i != j) {
        g->adj[i][j] = 1;
        g->adj[j][i] = 1;
    }
}

void print_topology(Graph* g) {
    printf("\n=== NETWORK TOPOLOGY (GRAPH) ===\n");
    int i,j;
    for ( i = 0; i < g->num_devices; i++) {
        printf("Device %d: %s [%s]\n", i, g->devices[i].name, g->devices[i].ip);
        printf("  Connected: %d\n", g->devices[i].connected);
        printf("  Neighbors: ");
        for ( j = 0; j < g->num_devices; j++) {
            if (g->adj[i][j]) printf("%d ", j);
        }
        printf("\n");
    }
}

// ========================= MODULE 2: PACKET BUFFER (QUEUE) =========================

#define MAX_BUFFER 50

typedef struct {
    int data;
    int src_id;
    int dest_id;
} Packet;

typedef struct {
    Packet buffer[MAX_BUFFER];
    int front;
    int rear;
    int count;
} PacketBuffer;

void init_buffer(PacketBuffer* pb) {
    pb->front = pb->rear = 0;
    pb->count = 0;
}

int is_full(PacketBuffer* pb) {
    return pb->count == MAX_BUFFER;
}

int is_empty(PacketBuffer* pb) {
    return pb->count == 0;
}

void enqueue(PacketBuffer* pb, Packet* p) {
    if (is_full(pb)) return;
    pb->buffer[pb->rear] = *p;
    pb->rear = (pb->rear + 1) % MAX_BUFFER;
    pb->count++;
}

Packet* dequeue(PacketBuffer* pb) {
    if (is_empty(pb)) return NULL;
    Packet* p = &pb->buffer[pb->front];
    pb->front = (pb->front + 1) % MAX_BUFFER;
    pb->count--;
    return p;
}

void process_packets(PacketBuffer* pb, Graph* g) {
    printf("\n=== PROCESSING PACKETS (QUEUE) ===\n");
    while (!is_empty(pb)) {
        Packet* p = dequeue(pb);
        if (p) {
            printf("Packet: src=%d, dest=%d, data=%d\n",
                   p->src_id, p->dest_id, p->data);
        }
    }
}

// ========================= MODULE 3: IP REGISTRY (BST) =========================

#define MAX_IP_LEN 16

typedef struct IPNode {
    char ip[MAX_IP_LEN];
    int device_id;
    struct IPNode* left;
    struct IPNode* right;
} IPNode;

IPNode* new_ip_node(const char* ip, int id) {
    IPNode* n = (IPNode*)malloc(sizeof(IPNode));
    strcpy(n->ip, ip);
    n->device_id = id;
    n->left = n->right = NULL;
    return n;
}

IPNode* insert_ip(IPNode* root, const char* ip, int id) {
    if (!root) return new_ip_node(ip, id);

    int cmp = strcmp(ip, root->ip);
    if (cmp < 0) {
        root->left = insert_ip(root->left, ip, id);
    } else if (cmp > 0) {
        root->right = insert_ip(root->right, ip, id);
    } else {
        root->device_id = id; 
    }
    return root;
}

int lookup_ip(IPNode* root, const char* ip) {
    if (!root) return -1;
    int cmp = strcmp(ip, root->ip);
    if (cmp == 0) return root->device_id;
    if (cmp < 0) return lookup_ip(root->left, ip);
    return lookup_ip(root->right, ip);
}

void print_ips(IPNode* root) {
    if (!root) return;
    print_ips(root->left);
    printf("  %s -> Device ID %d\n", root->ip, root->device_id);
    print_ips(root->right);
}

// ========================= MODULE 4: ACTIVITY LOG (LINKED LIST) =========================

#define MAX_LOG_MSG 128

typedef struct LogNode {
    char msg[MAX_LOG_MSG];
    time_t timestamp;
    struct LogNode* next;
} LogNode;

typedef struct {
    LogNode* head;
} ActivityLog;

void init_log(ActivityLog* log) {
    log->head = NULL;
}

void log_event(ActivityLog* log, const char* msg) {
    LogNode* n = (LogNode*)malloc(sizeof(LogNode));
    strncpy(n->msg, msg, MAX_LOG_MSG-1);
    n->msg[MAX_LOG_MSG-1] = '\0';
    n->timestamp = time(NULL);
    n->next = log->head;
    log->head = n;
}

void print_log(ActivityLog* log) {
    printf("\n=== ACTIVITY LOG (LINKED LIST) ===\n");
    LogNode* curr = log->head;
    while (curr) {
        printf("[%s] %s\n", ctime(&curr->timestamp), curr->msg);
        curr = curr->next;
    }
}

// ========================= MODULE 5: SYSTEM REVERT (STACK) =========================

#define MAX_SNAPSHOTS 10

typedef struct {
    char config[100];
    int version;
} Snapshot;

typedef struct {
    Snapshot stack[MAX_SNAPSHOTS];
    int top;
} ConfigStack;

void init_stack(ConfigStack* cs) {
    cs->top = -1;
}

int is_stack_empty(ConfigStack* cs) {
    return cs->top == -1;
}

int is_stack_full(ConfigStack* cs) {
    return cs->top == MAX_SNAPSHOTS - 1;
}

void push_snapshot(ConfigStack* cs, const char* cfg, int ver) {
    if (is_stack_full(cs)) return;
    cs->top++;
    strcpy(cs->stack[cs->top].config, cfg);
    cs->stack[cs->top].version = ver;
}

char* pop_snapshot(ConfigStack* cs) {
    if (is_stack_empty(cs)) return NULL;
    return cs->stack[cs->top--].config;
}

// ========================= MAIN: PROBLEM 1 RUN =========================

int main() {
    printf("=== SMART OFFICE NETWORK MONITORING SYSTEM (PROBLEM 1) ===\n");

    Graph g;
    init_graph(&g);
    IPNode* ip_root = NULL;
    ActivityLog log;
    init_log(&log);
    ConfigStack cstack;
    init_stack(&cstack);
    PacketBuffer pb;
    init_buffer(&pb);

    // Add devices
    int id1 = add_device(&g, "192.168.1.10", "PC-A");
    int id2 = add_device(&g, "192.168.1.11", "PC-B");
    int id3 = add_device(&g, "192.168.1.12", "Printer");
    int id4 = add_device(&g, "192.168.1.13", "Server");

    if (id1 == -1 || id2 == -1 || id3 == -1 || id4 == -1) {
        printf("Error: too many devices.\n");
        return 1;
    }

    // Build connections
    add_connection(&g, id1, id2);
    add_connection(&g, id2, id3);
    add_connection(&g, id3, id4);

    // Insert IPs into BST
    ip_root = insert_ip(ip_root, "192.168.1.10", id1);
    ip_root = insert_ip(ip_root, "192.168.1.11", id2);
    ip_root = insert_ip(ip_root, "192.168.1.12", id3);
    ip_root = insert_ip(ip_root, "192.168.1.13", id4);

    // Log activity
    log_event(&log, "Network started: all devices connected.");
    log_event(&log, "Server 192.168.1.13 joined the network.");
    log_event(&log, "Printer 192.168.1.12 added.");

    // Take configuration snapshots
    push_snapshot(&cstack, "Config_v1: base network setup", 1);
    push_snapshot(&cstack, "Config_v2: firewall rules added", 2);
    push_snapshot(&cstack, "Config_v3: QoS updated", 3);

    // Simulate packets
    Packet p1 = {.data = 1001, .src_id = id1, .dest_id = id3};
    Packet p2 = {.data = 1002, .src_id = id2, .dest_id = id4};

    enqueue(&pb, &p1);
    enqueue(&pb, &p2);

    // Show topology
    print_topology(&g);

    // Show IP registry (BST)
    printf("\n=== IP REGISTRY (BINARY SEARCH TREE) ===\n");
    print_ips(ip_root);

    // Print activity log
    print_log(&log);

    // Show current and rollback configs
    printf("\n=== CONFIG HISTORY (STACK) ===\n");
    printf("Current config: %s\n", pop_snapshot(&cstack));
    printf("Rollback to: %s\n", pop_snapshot(&cstack));

    // Process buffered packets
    process_packets(&pb, &g);

    return 0;
}
