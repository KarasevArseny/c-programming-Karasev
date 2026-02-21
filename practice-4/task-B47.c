#include <stdio.h>
int my_strlen(char *str) {
int length = 0;
while (str[length] != '\0') {
length++;
}    
return length;
}
int main(void) {
printf("=== Функция my_strlen() для вычисления длины строки ===\n\n");
printf("Тестовые строки:\n");
printf("1. \"Hello\" - длина: %d\n", my_strlen("Hello"));      
printf("2. \"\" (пустая строка) - длина: %d\n", my_strlen("")); 
printf("3. \"C Programming\" - длина: %d\n", my_strlen("C Programming")); 
char str[100];
printf("\nВведите строку для вычисления длины: ");
fgets(str, sizeof(str), stdin);
int i = 0;
while (str[i] != '\0') {
if (str[i] == '\n') {
str[i] = '\0';
break;
}
i++;
}    
printf("Длина введенной строки: %d\n", my_strlen(str));    
return 0;
}