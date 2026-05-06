#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100005

typedef struct {
    char reports[MAX_SIZE][10];
    int front;
    int rear;
    int size;
    int capacity;
} CircularQueue;

CircularQueue normalQ, priorityQ;
int totalProcessed = 0;
int dRejected = 0;

void initQueue(CircularQueue* q, int cap) {
    q->front = -1;
    q->rear = -1;
    q->size = 0;
    q->capacity = cap;
}

int isEmpty(CircularQueue* q) {
    return q->size == 0;
}

int isFull(CircularQueue* q) {
    return q->size == q->capacity;
}

int enqueue(CircularQueue* q, char* report) {
    if (isFull(q)) {
        return 0;
    }
    if (q->front == -1) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % q->capacity;
    strcpy(q->reports[q->rear], report);
    q->size++;
    return 1;
}

char* dequeue(CircularQueue* q) {
    if (isEmpty(q)) {
        return NULL;
    }
    char* report = q->reports[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    if (q->size == 0) {
        q->front = -1;
        q->rear = -1;
    }
    return report;
}

int main() {
    char id[10];
    int size;
    
    printf("Enter Hospital ID: ");
    scanf("%s", id);
    printf("Enter Queue Size: ");
    scanf("%d", &size);
    
    printf("med4\n");
    
    initQueue(&normalQ, size);
    initQueue(&priorityQ, size);
    
    printf("\n=== Hospital Report Management ===\n");
    printf("Commands available: ENQUEUE, PRIORITY, DEQUEUE, END\n\n");
    
    char operation[10];
    char report[10];
    
    while (1) {
        printf("Enter command: ");
        scanf("%s", operation);
        
        if (strcmp(operation, "END") == 0) {
            break;
        }
        
        if (strcmp(operation, "ENQUEUE") == 0) {
            printf("Enter report ID: ");
            scanf("%s", report);
            
            // FIXED: Total capacity check across BOTH queues
            if (normalQ.size + priorityQ.size >= size) {
                if (strcmp(report, "D") == 0) {
                    dRejected = 1;
                }
                printf(" normal queue full - %s rejected\n", report);
            } else {
                enqueue(&normalQ, report);
                printf(" %s added to normal queue\n", report);
            }
        }
        else if (strcmp(operation, "PRIORITY") == 0) {
            printf("Enter priority report ID: ");
            scanf("%s", report);
            enqueue(&priorityQ, report);
            printf(" %s added to priority queue\n", report);
        }
        else if (strcmp(operation, "DEQUEUE") == 0) {
            char* processed = NULL;
            
            if (!isEmpty(&priorityQ)) {
                processed = dequeue(&priorityQ);
            } else if (!isEmpty(&normalQ)) {
                processed = dequeue(&normalQ);
            }
            
            if (processed != NULL) {
                printf("Processed: %s\n", processed);
                totalProcessed++;
            } else {
                printf("No reports to process\n");
            }
        }
        printf("\n");
    }
    
    if (dRejected) {
        printf("Rejected: D (Queue Full)\n");
    }
    printf("Total Reports Processed: %d\n", totalProcessed);
    
    return 0;
}
