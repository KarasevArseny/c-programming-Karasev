#include <stdio.h>
#include <stdbool.h>
int main(void) {
int n;
printf("Enter N: ");
scanf("%d", &n);
bool is_prime[n + 1];
for (int i = 0; i <= n; i++) {
is_prime[i] = true;
}
is_prime[0] = is_prime[1] = false; 
for (int i = 2; i * i <= n; i++) { 
if (is_prime[i]) {
for (int j = i * i; j <= n; j += i) {
is_prime[j] = false;
}
}
}
printf("Primes: ");
for (int i = 2; i <= n; i++) {
if (is_prime[i]) {
printf("%d ", i);
}
}
printf("\n");
return 0;
}