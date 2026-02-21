#include <stdio.h>
int main(void) {
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;    
for (int i = 0; i < 5; i++) {
printf("arr[%d] = %d\n", i, arr[i]);
}    
for (int i = 0; i < 5; i++) {
printf("*(p + %d) = %d\n", i, *(p + i));
}    
return 0;
}