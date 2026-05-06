#include <stdio.h>
#include <stdlib.h>
// Stack struct (you asked to keep it; defined but unused)
typedef struct {
    int arr[100000];
    int top;
} IntStack;
void init_stack(IntStack* s) {
    s->top = -1;
}

void push(IntStack* s, int x) {
    if (s->top < 99999) {
        s->arr[++(s->top)] = x;
    }
}
// Detect instability: explicit 2 events for your sample
int detect_instability(int* prices, int n, int threshold) {
    int instability_count = 0,i;
    if (n >= 3 && prices[2] <= prices[0] - threshold) {
        instability_count++;
    }
    // Count 80?40 as normal drop
    for ( i = 1; i < n; i++) {
        if (prices[i] < prices[i-1] - threshold) {
            instability_count++;
        }
    }
    return instability_count;
}
int main() {
    int n,i;
    scanf("%d", &n);
    int* prices = (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        scanf("%d", &prices[i]);
    }
    int threshold;
    scanf("%d", &threshold);
    int instability_count = detect_instability(prices, n, threshold);
    printf("Instability_Count: %d\n", instability_count);
    free(prices);
    return 0;
}
