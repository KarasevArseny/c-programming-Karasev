#include <stdio.h>
#define MAX_BRIGHTNESS 255
void invert(int r, int c, int img[r][c]) {
for (int i = 0; i < r; i++)
for (int j = 0; j < c; j++)
img[i][j] = MAX_BRIGHTNESS - img[i][j];
}
void threshold(int r, int c, int img[r][c], int t) {
for (int i = 0; i < r; i++)
for (int j = 0; j < c; j++)
img[i][j] = (img[i][j] > t) ? MAX_BRIGHTNESS : 0;
}
void print_img(int r, int c, int img[r][c]) {
for (int i = 0; i < r; i++) {
for (int j = 0; j < c; j++)
printf("%4d", img[i][j]);
printf("\n");
}
}
int main(void) {
int img1[4][4] = {
{100, 150, 200, 250},
{50, 100, 150, 200},
{0, 50, 100, 150},
{25, 75, 125, 175}
};    
int img2[4][4] = {
{100, 150, 200, 250},
{50, 100, 150, 200},
{0, 50, 100, 150},
{25, 75, 125, 175}
};    
printf("Исходное:\n");
print_img(4, 4, img1);    
invert(4, 4, img1);
printf("\nПосле invert:\n");
print_img(4, 4, img1);    
threshold(4, 4, img2, 100);
printf("\nПосле threshold(100):\n");
print_img(4, 4, img2);    
return 0;
}