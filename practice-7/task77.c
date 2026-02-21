#include <stdio.h>
#include <stdlib.h>
typedef struct {
int key;
long offset;
} IndexEntry;
long index_search(const char *index_file, int key) {
FILE *idx = fopen(index_file, "rb");
if (!idx) return -1;    
IndexEntry entry;
while (fread(&entry, sizeof(IndexEntry), 1, idx) == 1) {
if (entry.key == key) {
fclose(idx);
return entry.offset;
}
}    
fclose(idx);
return -1;
}
int index_insert(const char *index_file, int key, long offset) {
FILE *idx = fopen(index_file, "ab");
if (!idx) return -1;    
IndexEntry entry = {key, offset};
fwrite(&entry, sizeof(IndexEntry), 1, idx);
fclose(idx);
return 0;
}
int main(void) {
index_insert("index.idx", 10, 100);
index_insert("index.idx", 20, 200);
index_insert("index.idx", 30, 300);
long offset = index_search("index.idx", 20);
printf("Ключ 20 найден по смещению: %ld\n", offset);    
return 0;
}