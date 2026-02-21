#include <stdio.h>
int main(void) {
int nums[4] = {1, 2, 3, 4};
int *p = nums;
for (int i = 0; i < 4; i++) {
*(p + i) *= 2;
}
return 0;
}