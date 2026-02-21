#include <stdio.h>
int main(void) {
int N;
printf("Enter N: ");
scanf("%d", &N);    
if (N <= 0) {
printf("N must be positive!\n");
return 1;
}
printf("Fibonacci: ");    
if (N >= 1) {
printf("0");
}
if (N >= 2) {
printf(" 1");
}
long long prev1 = 0, prev2 = 1, current;
for (int i = 3; i <= N; i++) {
current = prev1 + prev2;
printf(" %lld", current);
prev1 = prev2;
prev2 = current;
}    
printf("\n");   
return 0;
}