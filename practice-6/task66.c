#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
typedef enum {
TOKEN_NUMBER,
TOKEN_IDENTIFIER,
TOKEN_OPERATOR,
TOKEN_LPAREN,
TOKEN_RPAREN,
TOKEN_EOF
} TokenType;
typedef struct {
TokenType type;
union {
double number;
char identifier[64];
char operator;
} value;
int line;
int col;
} Token;
typedef struct {
const char *input;
int pos;
int line;
int col;
} Lexer;
void lex_init(Lexer *l, const char *s) { l->input = s; l->pos = 0; l->line = 1; l->col = 1; }
void skip_space(Lexer *l) {
while (l->input[l->pos] == ' ' || l->input[l->pos] == '\t' || l->input[l->pos] == '\n') {
if (l->input[l->pos] == '\n') { l->line++; l->col = 1; }
else l->col++;
l->pos++;
}
}
Token next_token(Lexer *l) {
skip_space(l);
Token t = {.line = l->line, .col = l->col};
char c = l->input[l->pos];   
if (c == '\0') { t.type = TOKEN_EOF; return t; }
if (isdigit(c) || c == '.') {
t.type = TOKEN_NUMBER;
char buf[32]; int i = 0;
while (isdigit(l->input[l->pos]) || l->input[l->pos] == '.') 
buf[i++] = l->input[l->pos++];
buf[i] = '\0';
t.value.number = atof(buf);
l->col += i;
return t;
}
if (isalpha(c)) {
t.type = TOKEN_IDENTIFIER;
int i = 0;
while (isalnum(l->input[l->pos])) 
t.value.identifier[i++] = l->input[l->pos++];
t.value.identifier[i] = '\0';
l->col += i;
return t;
}
if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
t.type = TOKEN_OPERATOR;
t.value.operator = c;
l->pos++; l->col++;
return t;
}
if (c == '(') { t.type = TOKEN_LPAREN; t.value.operator = c; l->pos++; l->col++; return t; }
if (c == ')') { t.type = TOKEN_RPAREN; t.value.operator = c; l->pos++; l->col++; return t; }    
t.type = TOKEN_EOF;
return t;
}
void print_token(Token *t) {
char *names[] = {"NUMBER", "IDENTIFIER", "OPERATOR", "LPAREN", "RPAREN", "EOF"};
printf("[%s] ", names[t->type]);
if (t->type == TOKEN_NUMBER) printf("%g", t->value.number);
else if (t->type == TOKEN_IDENTIFIER) printf("%s", t->value.identifier);
else if (t->type == TOKEN_OPERATOR || t->type == TOKEN_LPAREN || t->type == TOKEN_RPAREN)
printf("%c", t->value.operator);
printf(" (line %d, col %d)\n", t->line, t->col);
}
int main(void) {
Lexer lex;
lex_init(&lex, "x = 3.14 * (y + 2)");    
printf("Вход: \"x = 3.14 * (y + 2)\"\n\nТокены:\n");
while (1) {
Token t = next_token(&lex);
print_token(&t);
if (t.type == TOKEN_EOF) break;
}
return 0;
}