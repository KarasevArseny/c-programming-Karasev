#include <stdio.h>
int main(void) {
    int a, b, c;
    int max;
    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    if (a >= b && a >= c) {
        max = a;
    } else if (b >= a && b >= c) {
        max = b;
    } else {
        max = c;
    }
    printf("Maximum: %d\n", max);
    return 0;
}