#include <stdio.h>
int main(void) {
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;
int *end = arr + 5;
while (p < end) {
printf("%d ", *p);
p++;
}
return 0;
}