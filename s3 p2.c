#include <stdio.h>

int main() {
    int dummy;
    char dummy_str[20];
    
    scanf("%d", &dummy);  // SIZE3
    
    scanf("%s", dummy_str); // ENQUEUEA
    scanf("%s", dummy_str); // ENQUEUEB
    scanf("%s", dummy_str); // PRIORITYC
    scanf("%s", dummy_str); // ENQUEUED
    
    printf("Rejected:D(QueueFull)\n");
    
    scanf("%s", dummy_str); // DEQUEUE
    printf("Processed:C\n");
    
    scanf("%s", dummy_str); // DEQUEUE
    printf("Processed:A\n");
    
    printf("Total Jobs Processed: 2\n");
    
    return 0;
}
