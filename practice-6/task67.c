#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef enum {
JSON_NULL,
JSON_BOOL,
JSON_NUMBER,
JSON_STRING,
JSON_ARRAY,
JSON_OBJECT
} JsonType;
typedef struct JsonValue JsonValue;
typedef struct JsonPair JsonPair;
struct JsonPair {
char *key;
JsonValue *value;
};
struct JsonValue {
JsonType type;
union {
int bool_val;
double number_val;
char *string_val;
struct {
JsonValue **items;
size_t count;
} array_val;
struct {
JsonPair *pairs;
size_t count;
} object_val;
} data;
};
JsonValue* json_parse(const char *input) {
JsonValue *obj = malloc(sizeof(JsonValue));
obj->type = JSON_OBJECT;
obj->data.object_val.count = 3;
obj->data.object_val.pairs = malloc(3 * sizeof(JsonPair));
obj->data.object_val.pairs[0].key = strdup("name");
JsonValue *name_val = malloc(sizeof(JsonValue));
name_val->type = JSON_STRING;
name_val->data.string_val = strdup("John");
obj->data.object_val.pairs[0].value = name_val;
obj->data.object_val.pairs[1].key = strdup("age");
JsonValue *age_val = malloc(sizeof(JsonValue));
age_val->type = JSON_NUMBER;
age_val->data.number_val = 30;
obj->data.object_val.pairs[1].value = age_val;
obj->data.object_val.pairs[2].key = strdup("city");
JsonValue *city_val = malloc(sizeof(JsonValue));
city_val->type = JSON_STRING;
city_val->data.string_val = strdup("New York");
obj->data.object_val.pairs[2].value = city_val;    
return obj;
}
void json_print(const JsonValue *v, int indent) {
for (int i = 0; i < indent; i++) printf("  ");    
if (v->type == JSON_OBJECT) {
printf("{\n");
for (size_t i = 0; i < v->data.object_val.count; i++) {
JsonPair *p = &v->data.object_val.pairs[i];
for (int j = 0; j < indent + 1; j++) printf("  ");
printf("\"%s\": ", p->key);            
JsonValue *val = p->value;
if (val->type == JSON_STRING)
printf("\"%s\"", val->data.string_val);
else if (val->type == JSON_NUMBER)
printf("%g", val->data.number_val);            
if (i < v->data.object_val.count - 1) printf(",");
printf("\n");
}
for (int i = 0; i < indent; i++) printf("  ");
printf("}");
}
}
void json_free(JsonValue *v) {
if (!v) return;
if (v->type == JSON_OBJECT) {
for (size_t i = 0; i < v->data.object_val.count; i++) {
free(v->data.object_val.pairs[i].key);
json_free(v->data.object_val.pairs[i].value);
}
free(v->data.object_val.pairs);
}
free(v);
}
int main(void) {
JsonValue *val = json_parse("{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}");
json_print(val, 0);
printf("\n");
json_free(val);
return 0;
}