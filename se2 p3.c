#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NAME 100
#define MAX_HASH 64
#define MAX_FILES 100000
// 1. CUSTODY CHAIN (LINKED LIST)
typedef struct CustodyNode {
    char person[MAX_NAME];
    char timestamp[50];
    struct CustodyNode* next;
} CustodyNode;
CustodyNode* chain_head = NULL;
void add_handler(char* person) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    CustodyNode* new_node = malloc(sizeof(CustodyNode));
    strftime(new_node->timestamp, sizeof(new_node->timestamp), "%Y-%m-%d %H:%M:%S", t);
    strncpy(new_node->person, person, MAX_NAME - 1);
    new_node->person[MAX_NAME-1] = '\0';
    new_node->next = chain_head;
    chain_head = new_node;
}
void print_chain() {
    CustodyNode* curr = chain_head;
    printf("\nCHAIN OF CUSTODY (Linked List - Reverse Chronological):\n");
    while (curr) {
        printf("- %s at %s\n", curr->person, curr->timestamp);
        curr = curr->next;
    }
}
// 2. DECRYPTION TASK-LIST (QUEUE)
typedef struct FileTask {
    char filename[MAX_NAME];
    struct FileTask* next;
} FileTask;
typedef struct Queue {
    FileTask* front;
    FileTask* rear;
} Queue;
Queue decrypt_q = {NULL, NULL};
void enqueue_task(char* filename) {
    FileTask* new_task = malloc(sizeof(FileTask));
    strncpy(new_task->filename, filename, MAX_NAME - 1);
    new_task->filename[MAX_NAME-1] = '\0';
    new_task->next = NULL;
    if (!decrypt_q.rear) {
        decrypt_q.front = decrypt_q.rear = new_task;
    } else {
        decrypt_q.rear->next = new_task;
        decrypt_q.rear = new_task;
    }
}
char* dequeue_task() {
    if (!decrypt_q.front) return NULL;
    FileTask* task = decrypt_q.front;
    char* filename = strdup(task->filename);
    decrypt_q.front = task->next;
    if (!decrypt_q.front) decrypt_q.rear = NULL;
    free(task);
    return filename;
}
void print_queue() {
    printf("\nDECRYPTION QUEUE (FIFO):\n");
    FileTask* curr = decrypt_q.front;
    while (curr) {
        printf("- %s\n", curr->filename);
        curr = curr->next;
    }
}
// 3. FILE FINGERPRINT INDEX (BST)
typedef struct HashNode {
    char hash[MAX_HASH];
    int illegal;  // 1 if known illegal
    struct HashNode* left;
    struct HashNode* right;
} HashNode;
HashNode* bst_root = NULL;
int hash_compare(const char* h1, const char* h2) {
    return strcmp(h1, h2);
}
HashNode* bst_insert(HashNode* node, char* hash, int illegal) {
    if (!node) {
        node = malloc(sizeof(HashNode));
        strncpy(node->hash, hash, MAX_HASH - 1);
        node->hash[MAX_HASH-1] = '\0';
        node->illegal = illegal;
        node->left = node->right = NULL;
        return node;
    }
    int cmp = hash_compare(hash, node->hash);
    if (cmp < 0) node->left = bst_insert(node->left, hash, illegal);
    else if (cmp > 0) node->right = bst_insert(node->right, hash, illegal);
    return node;
}
int bst_search(HashNode* node, char* hash) {
    if (!node) return 0;
    int cmp = hash_compare(hash, node->hash);
    if (cmp == 0) return node->illegal;
    if (cmp < 0) return bst_search(node->left, hash);
    return bst_search(node->right, hash);
}
void print_bst(HashNode* node) {
    if (node) {
        print_bst(node->left);
        printf("Hash: %s (Illegal: %s)\n", node->hash, node->illegal ? "YES" : "NO");
        print_bst(node->right);
    }
}
// 4. DIRECTORY BRANCHING (GRAPH)
#define MAX_DIRS 1000
typedef struct DirGraph {
    int dirs[MAX_DIRS][MAX_DIRS];  
    int num_dirs;
} DirGraph;
DirGraph dir_g = {0};
void add_dir_link(int parent, int child) {
    dir_g.dirs[parent][child] = 1;
}
int find_deepest(int start) {
    int depth[MAX_DIRS];
    int visited[MAX_DIRS] = {0};
    int i,d,j;
    for ( i = 0; i < MAX_DIRS; i++) depth[i] = -1;
    depth[start] = 0;
    // BFS for max depth
    int max_d = 0, deepest = start;
    for (d = 0; d < MAX_DIRS; d++) {
        for ( i = 0; i < dir_g.num_dirs; i++) {
            if (depth[i] == d) {
                for ( j = 0; j < dir_g.num_dirs; j++) {
                    if (dir_g.dirs[i][j] && depth[j] == -1) {
                        depth[j] = d + 1;
                        if (depth[j] > max_d) {
                            max_d = depth[j];
                            deepest = j;
                        }
                    }
                }
            }
        }
    }
    return deepest;
}
// 5. COMMAND UNDO (STACK)
typedef struct Command {
    char cmd_desc[200];
    struct Command* next;
} Command;
typedef struct Stack {
    Command* top;
} Stack;
Stack cmd_stack = {NULL};
void push_cmd(char* desc) {
    Command* new_cmd = malloc(sizeof(Command));
    strncpy(new_cmd->cmd_desc, desc, 199);
    new_cmd->cmd_desc[199] = '\0';
    new_cmd->next = cmd_stack.top;
    cmd_stack.top = new_cmd;
}
char* pop_cmd() {
    if (!cmd_stack.top) return NULL;
    Command* cmd = cmd_stack.top;
    char* desc = strdup(cmd->cmd_desc);
    cmd_stack.top = cmd->next;
    free(cmd);
    return desc;
}
void print_stack() {
    printf("\nCOMMAND STACK (LIFO):\n");
    Command* curr = cmd_stack.top;
    while (curr) {
        printf("- %s\n", curr->cmd_desc);
        curr = curr->next;
    }
}
int main() {
    // Demo: Custody Chain
    add_handler("Officer Jane Doe");
    add_handler("Analyst John Smith");
    print_chain(); 
    // Demo: Decryption Queue
    enqueue_task("encrypted_file1.enc");
    enqueue_task("encrypted_file2.enc");
    enqueue_task("encrypted_file3.enc");
    printf("\nProcessing decryption: %s\n", dequeue_task());  // [web:4][web:9]
    print_queue();
    // Demo: Hash BST
    bst_root = bst_insert(bst_root, "da39a3ee5e6b4b0d3255bfef95601890afd80709", 0);  // empty file, good
    bst_root = bst_insert(bst_root, "5f4dcc3b5aa765d61d8327deb882cf99", 1);  // password123, illegal
    printf("Is '5f4dcc3b5aa765d61d8327deb882cf99' illegal? %s\n", 
           bst_search(bst_root, "5f4dcc3b5aa765d61d8327deb882cf99") ? "YES" : "NO");  // [web:5][web:10]
    print_bst(bst_root);
    // Demo: Directory Graph
    dir_g.num_dirs = 5;
    add_dir_link(0,1); add_dir_link(0,2); add_dir_link(1,3); add_dir_link(2,3); add_dir_link(3,4);
    printf("Deepest nested folder from root 0: %d\n", find_deepest(0));  // [web:6]
    // Demo: Command Stack
    push_cmd("Run hash scan");
    push_cmd("Decrypt folder /evidence");
    push_cmd("Mount seized drive");
    printf("Undo last command: %s\n", pop_cmd());  // [web:7][web:12]
    print_stack();
    // Cleanup would go here in production
    printf("\nDigital Forensics Lab System Demo Complete. All modules operational.[cite:1][cite:2]\n");
    return 0;
}
