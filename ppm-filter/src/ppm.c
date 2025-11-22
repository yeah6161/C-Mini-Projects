#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image *read_ppm(const char *path) {
    FILE *f = fopen(path,"rb"); if(!f) return NULL;
    char hdr[3]; int w,h,max;
    if (fscanf(f,"%2s %d %d %d\n", hdr, &w, &h, &max)!=4) { fclose(f); return NULL; }
    if (strcmp(hdr,"P6")!=0) { fclose(f); return NULL; }
    Image *img = malloc(sizeof(Image)); img->w = w; img->h = h;
    img->data = malloc(w*h*3);
    fread(img->data, 1, w*h*3, f);
    fclose(f); return img;
}

int write_ppm(const char *path, Image *img) {
    FILE *f = fopen(path,"wb"); if(!f) return 0;
    fprintf(f,"P6\n%d %d\n255\n", img->w, img->h);
    fwrite(img->data,1,img->w*img->h*3,f);
    fclose(f); return 1;
}

Image *grayscale(Image *img) {
    Image *out = malloc(sizeof(Image)); out->w=img->w; out->h=img->h;
    out->data = malloc(img->w*img->h*3);
    for (int i=0;i<img->w*img->h;i++) {
        unsigned char r = img->data[i*3], g = img->data[i*3+1], b = img->data[i*3+2];
        unsigned char gray = (unsigned char)(0.299*r + 0.587*g + 0.114*b);
        out->data[i*3]=out->data[i*3+1]=out->data[i*3+2]=gray;
    }
    return out;
}

void free_image(Image *img) { if(!img) return; free(img->data); free(img); }

