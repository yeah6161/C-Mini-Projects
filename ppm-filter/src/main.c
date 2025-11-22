#include <stdio.h>
#include "ppm.h"

int main(int argc, char **argv) {
    if (argc<4) { printf("Usage: %s in.ppm out.ppm op\nops: gray\n", argv[0]); return 1; }
    Image *img = read_ppm(argv[1]);
    if (!img) { printf("Failed read\n"); return 1; }
    Image *out = NULL;
    if (strcmp(argv[3],"gray")==0) out = grayscale(img);
    if (!out) { printf("Unknown op\n"); free_image(img); return 1; }
    write_ppm(argv[2], out);
    free_image(img); free_image(out);
    printf("Done\n");
    return 0;
}

