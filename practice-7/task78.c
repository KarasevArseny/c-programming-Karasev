#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int (*ChunkProcessor)(const char *chunk, size_t size, void *context);
int process_file_chunked(const char *filename, size_t chunk_size,
ChunkProcessor processor, void *context) {
FILE *file = fopen(filename, "rb");
if (!file) return -1;    
char *buffer = (char*)malloc(chunk_size);
if (!buffer) {
fclose(file);
return -1;
}    
size_t bytes_read;
while ((bytes_read = fread(buffer, 1, chunk_size, file)) > 0) {
int result = processor(buffer, bytes_read, context);
if (result != 0) break;
}    
free(buffer);
fclose(file);
return 0;
}
typedef struct {
size_t count;
} CountContext;
int count_processor(const char *chunk, size_t size, void *context) {
CountContext *ctx = (CountContext*)context;
ctx->count += size;
return 0;
}
int main(void) {
FILE *file = fopen("test.txt", "w");
fprintf(file, "Hello\nWorld\n");
fclose(file);
CountContext ctx = {0};
process_file_chunked("test.txt", 10, count_processor, &ctx);
printf("Всего символов: %zu\n", ctx.count);    
return 0;
}