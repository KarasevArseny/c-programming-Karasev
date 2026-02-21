#include <stdio.h>
void my_strcpy(char *dest, char *src) {
int i = 0;
while (src[i] != '\0') {
dest[i] = src[i];
i++;
}
dest[i] = '\0';
}
int main(void) {
printf("=== Функция my_strcpy() для копирования строк ===\n\n");    
char source[] = "Hello, World!";
char destination[50];    
printf("Исходная строка: \"%s\"\n", source);
printf("До копирования: destination = \"%s\"\n", destination);    
my_strcpy(destination, source);    
printf("После копирования: destination = \"%s\"\n", destination);    
return 0;
}