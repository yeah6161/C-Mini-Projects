#ifndef PPM_H
#define PPM_H
typedef struct { int w,h; unsigned char *data; } Image;
Image *read_ppm(const char *path);
int write_ppm(const char *path, Image *img);
Image *grayscale(Image *img);
void free_image(Image *img);
#endif

