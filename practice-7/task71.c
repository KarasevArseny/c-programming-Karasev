#include <stdio.h>
#include <string.h>
#include <ctype.h>
typedef struct {
size_t chars;
size_t chars_no_ws;
size_t words;
size_t lines;
size_t empty_lines;
size_t max_line_len;
char longest_word[100];
} FileStats;
int is_word_char(char c) { return isalnum(c) || c == '_' || c == '-'; }
int analyze_file(const char *filename, FileStats *stats) {
FILE *f = fopen(filename, "r");
if (!f) return -1;    
stats->chars = stats->chars_no_ws = stats->words = stats->lines = 
stats->empty_lines = stats->max_line_len = 0;
stats->longest_word[0] = '\0';    
char line[1024];
int in_word = 0;    
while (fgets(line, sizeof(line), f)) {
stats->lines++;
int len = strlen(line);
if (len > 0 && line[len-1] == '\n') len--;        
if (len > stats->max_line_len) stats->max_line_len = len;        
int empty = 1;
for (int i = 0; line[i]; i++) {
stats->chars++;
if (!isspace(line[i])) {
stats->chars_no_ws++;
empty = 0;
}
if (is_word_char(line[i])) {
if (!in_word) { stats->words++; in_word = 1; }
} else in_word = 0;
}
if (empty) stats->empty_lines++;
in_word = 0;
}
fclose(f);
return 0;
}
void print_stats(const FileStats *s) {
printf("Символов: %zu\nСимволов (без пробелов): %zu\nСлов: %zu\nСтрок: %zu\nПустых строк: %zu\nМаксимальная длина строки: %zu\n",
s->chars, s->chars_no_ws, s->words, s->lines, s->empty_lines, s->max_line_len);
}
int main(void) {
FILE *f = fopen("sample.txt", "w");
fprintf(f, "Это пример файла.\nТут две строки.\n\nИ пустая строка выше.\n");
fclose(f);    
FileStats stats;
if (analyze_file("sample.txt", &stats) == 0)
print_stats(&stats);
return 0;
}