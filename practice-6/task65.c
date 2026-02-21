#include <stdio.h>
#include <string.h>
typedef enum {
PERM_NONE   = 0,
PERM_READ   = 1 << 0,
PERM_WRITE  = 1 << 1,
PERM_ADMIN  = 1 << 4,
PERM_ALL    = PERM_READ | PERM_WRITE | PERM_ADMIN
} Permission;
typedef struct {
unsigned int id;
char name[50];
Permission permissions;
} User;
typedef struct {
char name[100];
Permission required;
} Resource;
int has_perm(Permission user, Permission req) {
return (user & req) == req;
}
void perm_str(Permission p, char *buf) {
buf[0] = (p & PERM_READ)  ? 'r' : '-';
buf[1] = (p & PERM_WRITE) ? 'w' : '-';
buf[2] = '-';
buf[3] = '-';
buf[4] = (p & PERM_ADMIN) ? 'a' : '-';
buf[5] = '\0';
}
int main(void) {
User users[3] = {
{1, "admin",   PERM_ALL},
{2, "editor",  PERM_READ | PERM_WRITE},
{3, "viewer",  PERM_READ}
};
Resource res = {"/data/secret.txt", PERM_READ | PERM_WRITE};    
printf("Ресурс: %s (требуются: rw---)\n\n", res.name);    
for (int i = 0; i < 3; i++) {
char pstr[6];
perm_str(users[i].permissions, pstr);
printf("%s (%s) -> %s\n", 
users[i].name, 
pstr,
has_perm(users[i].permissions, res.required) ? "разрешён" : "ЗАПРЕЩЁН");
}    
return 0;
}