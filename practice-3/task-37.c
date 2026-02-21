#include <stdio.h>
int main(void) {
int M;
printf("Enter M: ");
scanf("%d", &M);
if (M < 1) {
printf("Padovan numbers <= %d: (none)\n", M);
return 0;
}
printf("Padovan numbers <= %d: ", M);
if (M >= 1) {
printf("1");
int p0 = 1, p1 = 1, p2 = 1;
int next;
while (1) {
next = p0 + p1;
if (next > M) {
break;
}
printf(" %d", next);
p0 = p1;
p1 = p2;
p2 = next;
}
}
printf("\n");
return 0;
}