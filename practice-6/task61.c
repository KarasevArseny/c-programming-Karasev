#include <stdio.h>
#include <string.h>
typedef enum { PHONE_HOME, PHONE_WORK, PHONE_MOBILE } PhoneType;
typedef struct {
PhoneType type;
char number[20];
} Phone;
typedef struct {
int day;
int month;
int year;
} Date;
typedef struct {
char first_name[30];
char last_name[30];
char email[50];
Phone phones[3];
int phone_count;
Date birthday;
} Contact;
typedef struct {
Contact contacts[100];
int count;
} AddressBook;
const char* phone_type_str(PhoneType t) {
switch(t) {
case PHONE_HOME: return "Домашний";
case PHONE_WORK: return "Рабочий";
case PHONE_MOBILE: return "Мобильный";
default: return "?";
}
}
void book_init(AddressBook *b) { b->count = 0; }
int book_add(AddressBook *b, const Contact *c) {
if (b->count >= 100) return -1;
b->contacts[b->count++] = *c;
return b->count - 1;
}
int book_find_by_lastname(const AddressBook *b, const char *last) {
for (int i = 0; i < b->count; i++)
if (strcmp(b->contacts[i].last_name, last) == 0)
return i;
return -1;
}
int book_find_by_email(const AddressBook *b, const char *email) {
for (int i = 0; i < b->count; i++)
if (strcmp(b->contacts[i].email, email) == 0)
return i;
return -1;
}
int book_remove(AddressBook *b, int idx) {
if (idx < 0 || idx >= b->count) return -1;
for (int i = idx; i < b->count - 1; i++)
b->contacts[i] = b->contacts[i + 1];
b->count--;
return 0;
}
void book_print(const AddressBook *b) {
printf("\n=== Адресная книга ===\n");
for (int i = 0; i < b->count; i++) {
Contact *c = &b->contacts[i];
printf("%d %s %s\n", i + 1, c->last_name, c->first_name);
printf("   Email: %s\n", c->email);
for (int j = 0; j < c->phone_count; j++)
printf("   [%s] %s\n", phone_type_str(c->phones[j].type), c->phones[j].number);
printf("   День рождения: %02d.%02d.%04d\n", c->birthday.day, c->birthday.month, c->birthday.year);
}
}
void book_print_birthdays(const AddressBook *b, int month) {
printf("\nКонтакты с днём рождения в месяце %d:\n", month);
for (int i = 0; i < b->count; i++) {
Contact *c = &b->contacts[i];
if (c->birthday.month == month)
printf("  - %s %s (%02d.%02d)\n", c->last_name, c->first_name, c->birthday.day, c->birthday.month);
}
}
void book_sort_by_lastname(AddressBook *b) {
for (int i = 0; i < b->count - 1; i++) {
for (int j = 0; j < b->count - i - 1; j++) {
if (strcmp(b->contacts[j].last_name, b->contacts[j+1].last_name) > 0) {
Contact temp = b->contacts[j];
b->contacts[j] = b->contacts[j+1];
b->contacts[j+1] = temp;
}
}
}
}
int main(void) {
AddressBook book;
book_init(&book);
Contact c1 = {.first_name = "Пётр", .last_name = "Иванов", 
.email = "petrov@email.com", .phone_count = 2};
c1.phones[0] = (Phone){PHONE_MOBILE, "+7-999-123-4567"};
c1.phones[1] = (Phone){PHONE_WORK, "+7-495-123-4567"};
c1.birthday = (Date){15, 3, 1990};
Contact c2 = {.first_name = "Мария", .last_name = "Сидорова", 
.email = "sidorova@email.com", .phone_count = 1};
c2.phones[0] = (Phone){PHONE_HOME, "+7-495-987-6543"};
c2.birthday = (Date){22, 7, 1985};    
book_add(&book, &c1);
book_add(&book, &c2);    
book_print(&book);
book_print_birthdays(&book, 3);    
return 0;
}