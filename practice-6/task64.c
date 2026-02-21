#include <stdio.h>
#include <string.h>
#include <time.h>
typedef enum {
MSG_TEXT,
MSG_IMAGE,
MSG_LOCATION
} MessageType;
typedef struct {
char content[500];
} TextMessage;
typedef struct {
char filename[100];
int width;
int height;
size_t size_bytes;
} ImageMessage;
typedef struct {
double latitude;
double longitude;
char address[200];
} LocationMessage;
typedef struct {
unsigned int sender_id;
unsigned int chat_id;
time_t timestamp;
MessageType type;
union {
TextMessage text;
ImageMessage image;
LocationMessage location;
} content;
} Message;
time_t now() { return time(NULL); }
Message text_msg(unsigned int sender, unsigned int chat, const char *text) {
Message m = {sender, chat, now(), MSG_TEXT};
strcpy(m.content.text.content, text);
return m;
}
Message image_msg(unsigned int sender, unsigned int chat, const char *fname, int w, int h, size_t size) {
Message m = {sender, chat, now(), MSG_IMAGE};
strcpy(m.content.image.filename, fname);
m.content.image.width = w;
m.content.image.height = h;
m.content.image.size_bytes = size;
return m;
}
Message loc_msg(unsigned int sender, unsigned int chat, double lat, double lon, const char *addr) {
Message m = {sender, chat, now(), MSG_LOCATION};
m.content.location.latitude = lat;
m.content.location.longitude = lon;
strcpy(m.content.location.address, addr);
return m;
}
void print_msg(const Message *m) {
char time_str[20];
struct tm *tm_info = localtime(&m->timestamp);
strftime(time_str, 20, "%H:%M", tm_info);    
printf("[%s] User#%u: ", time_str, m->sender_id);   
switch (m->type) {
case MSG_TEXT:
printf("%s", m->content.text.content);
break;
case MSG_IMAGE:
printf("[Изображение: %s (%dx%d, %.1f MB)]", 
m->content.image.filename,
m->content.image.width,
m->content.image.height,
m->content.image.size_bytes / (1024.0 * 1024.0));
break;
case MSG_LOCATION:
printf("[Локация: %.4f, %.4f - %s]", 
m->content.location.latitude,
m->content.location.longitude,
m->content.location.address);
break;
}
printf("\n");
}
int main(void) {
Message msgs[5];
msgs[0] = text_msg(1, 42, "Привет! Как дела?");
msgs[1] = text_msg(2, 42, "Отлично! Смотри что нашёл:");
msgs[2] = image_msg(2, 42, "photo.jpg", 1920, 1080, 2.5 * 1024 * 1024);
msgs[3] = text_msg(1, 42, "Круто! Где это?");
msgs[4] = loc_msg(2, 42, 55.7558, 37.6173, "Москва, Красная площадь");    
printf("=== Чат #42 ===\n");
for (int i = 0; i < 5; i++) {
print_msg(&msgs[i]);
}    
return 0;
}