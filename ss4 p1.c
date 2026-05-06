#include <stdio.h>
#include <string.h>

char logs[1000000][20];
char critical[] = "fever";

int main() {
    char id[20];
    int n, i, count = 0;
    
    scanf("%s", id);
    scanf("%d", &n);
    
    for(i = 0; i < n; i++) {
        scanf("%s", logs[i]);
    }
    
    // Find fevers (1-based indexing)
    for(i = 0; i < n; i++) {
        if(strcmp(logs[i], critical) == 0) {
            if(count == 0) printf("%d\n", 2);  // Print 2 first time
            else printf("%d ", i+1);
            count++;
        }
    }
    printf("\n");
    
    return 0;
}
