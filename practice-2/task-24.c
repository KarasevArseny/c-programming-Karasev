#include <stdio.h>
int main(void) {
    float a, b, c;
    printf("Enter sides a, b, c: ");
    scanf("%f %f %f", &a, &b, &c);
    if (a + b <= c || a + c <= b || b + c <= a) {
    printf("Triangle does not exist\n");
    }
    else if (a == b && b == c) {
    printf("Equilateral triangle\n");
    }
    else if (a == b || a == c || b == c) {
    printf("Isosceles triangle\n");
    }
    else {
    printf("Scalene triangle\n");
    }
    return 0;
}