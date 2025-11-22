#include "csv.h"
#include <stdlib.h>
#include <string.h>

char **parse_line(const char *line, int *cols) {
    char *tmp = strdup(line);
    char *p = tmp;
    int cap = 8; char **arr = malloc(sizeof(char*)*cap); int n=0;
    char *token;
    while ((token = strsep(&p, ",")) != NULL) {
        if (n>=cap) { cap*=2; arr = realloc(arr, sizeof(char*)*cap); }
        arr[n++] = strdup(token);
    }
    free(tmp);
    *cols = n;
    return arr;
}

void free_fields(char **f, int cols) {
    for (int i=0;i<cols;i++) free(f[i]);
    free(f);
}

