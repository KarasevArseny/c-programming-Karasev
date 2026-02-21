#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
char ***data;
int rows;
int cols;
char **headers;
} CSVFile;
CSVFile* csv_load(const char *fname, int has_headers) {
FILE *f = fopen(fname, "r");
if (!f) return NULL;    
CSVFile *csv = malloc(sizeof(CSVFile));
csv->rows = 0;
csv->cols = 0;
csv->data = NULL;
csv->headers = NULL;    
char line[1024];
int first = 1;    
while (fgets(line, sizeof(line), f)) {
line[strcspn(line, "\n")] = 0;        
if (first && has_headers) {
char *token = strtok(line, ",");
while (token) {
csv->headers = realloc(csv->headers, (csv->cols + 1) * sizeof(char*));
csv->headers[csv->cols] = strdup(token);
csv->cols++;
token = strtok(NULL, ",");
}
first = 0;
} else {
csv->data = realloc(csv->data, (csv->rows + 1) * sizeof(char**));
csv->data[csv->rows] = malloc(csv->cols * sizeof(char*));            
char *token = strtok(line, ",");
for (int i = 0; i < csv->cols; i++) {
csv->data[csv->rows][i] = token ? strdup(token) : strdup("");
token = strtok(NULL, ",");
}
csv->rows++;
}
}    
fclose(f);
return csv;
}
const char* csv_get(CSVFile *csv, int row, int col) {
return (row < csv->rows && col < csv->cols) ? csv->data[row][col] : NULL;
}
const char* csv_get_by_header(CSVFile *csv, int row, const char *header) {
for (int i = 0; i < csv->cols; i++)
if (strcmp(csv->headers[i], header) == 0)
return csv_get(csv, row, i);
return NULL;
}
void csv_free(CSVFile *csv) {
for (int i = 0; i < csv->rows; i++)
for (int j = 0; j < csv->cols; j++)
free(csv->data[i][j]);
for (int i = 0; i < csv->cols; i++)
free(csv->headers[i]);
free(csv->data);
free(csv->headers);
free(csv);
}
int main(void) {
FILE *f = fopen("employees.csv", "w");
fprintf(f, "name,age,city,salary\nИван,30,Москва,50000\nМария,25,Санкт-Петербург,45000\nПётр,35,Казань,55000\n");
fclose(f);    
CSVFile *csv = csv_load("employees.csv", 1);
printf("Сотрудников: %d\n", csv->rows);    
const char *age = csv_get_by_header(csv, 1, "age");
printf("Возраст Марии: %s\n", age);    
csv_free(csv);
return 0;
}