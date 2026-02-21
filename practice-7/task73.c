#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
int id;
char name[50];
char email[100];
int age;
double balance;
int is_active;
} Record;
typedef struct {
FILE *file;
int record_count;
int next_id;
char filename[256];
} Database;
Database* db_open(const char *fname) {
Database *db = malloc(sizeof(Database));
strcpy(db->filename, fname);    
db->file = fopen(fname, "rb+");
if (!db->file) {
db->file = fopen(fname, "wb+");
db->record_count = 0;
db->next_id = 1;
printf("Создана новая БД\n");
} else {
fread(&db->record_count, sizeof(int), 1, db->file);
fread(&db->next_id, sizeof(int), 1, db->file);
printf("Открыта БД с %d записями\n", db->record_count);
}
return db;
}
void db_close(Database *db) {
if (db->file) {
fseek(db->file, 0, SEEK_SET);
fwrite(&db->record_count, sizeof(int), 1, db->file);
fwrite(&db->next_id, sizeof(int), 1, db->file);
fclose(db->file);
}
free(db);
}
int db_insert(Database *db, Record *r) {
r->id = db->next_id++;
r->is_active = 1;
fseek(db->file, 0, SEEK_END);
fwrite(r, sizeof(Record), 1, db->file);
db->record_count++;
return r->id;
}
Record* db_get(Database *db, int id) {
fseek(db->file, 2 * sizeof(int), SEEK_SET);  // Пропускаем заголовок
Record r;
for (int i = 0; i < db->record_count; i++) {
fread(&r, sizeof(Record), 1, db->file);
if (r.id == id && r.is_active) {
Record *result = malloc(sizeof(Record));
*result = r;
return result;
}
}
return NULL;
}
void db_print_all(Database *db) {
printf("\n=== База данных %s ===\n", db->filename);
printf("ID | %-20s | %-22s | Возраст | Баланс | Активен\n", "Имя", "Email");    
fseek(db->file, 2 * sizeof(int), SEEK_SET);
Record r;
for (int i = 0; i < db->record_count; i++) {
fread(&r, sizeof(Record), 1, db->file);
printf("%2d | %-20s | %-22s | %5d   | %8.2f | %s\n",
r.id, r.name, r.email, r.age, r.balance,
r.is_active ? "Да" : "Нет");
}
}
int main(void) {
Database *db = db_open("users.db");    
if (db->record_count == 0) {
Record r1 = {0, "Иван Петров", "ivan@email.com", 30, 1500, 1};
Record r2 = {0, "Мария Сидорова", "maria@email.com", 25, 2300.5, 1};
Record r3 = {0, "Пётр Козлов", "petr@email.com", 35, 800, 0};
db_insert(db, &r1);
db_insert(db, &r2);
db_insert(db, &r3);
}    
db_print_all(db);
db_close(db);    
return 0;
}