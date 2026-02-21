#include <stdio.h>
void reverse(char *str) {
if (str == NULL) {
return;
}
int length = 0;
while (str[length] != '\0') {
length++;
}
for (int i = 0; i < length / 2; i++) {
char temp = str[i];
str[i] = str[length - 1 - i];
str[length - 1 - i] = temp;
}
}
int main(void) {
printf("=== Функция reverse() для переворачивания строки ===\n\n");
char str1[] = "Hello";
printf("Пример 1:\n");
printf("До reverse: \"%s\"\n", str1);
reverse(str1);
printf("После reverse: \"%s\"\n\n", str1);
return 0;
}