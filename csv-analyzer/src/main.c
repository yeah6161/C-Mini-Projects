#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include <ctype.h>

int main(int argc, char **argv) {
    if (argc < 2) { printf("Usage: %s file.csv\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1],"r"); if (!f) { perror("open"); return 1; }
    char line[8192];
    int row=0;
    double *sums=NULL; int *counts=NULL; int cols=0;
    while (fgets(line,sizeof(line),f)) {
        line[strcspn(line,"\r\n")]=0;
        int c; char **fields = parse_line(line, &c);
        if (row==0) { cols=c; sums=calloc(cols,sizeof(double)); counts=calloc(cols,sizeof(int)); }
        for (int i=0;i<c;i++) {
            char *t = fields[i];
            int isnum=1; char *p=t;
            while (*p && isspace((unsigned char)*p)) p++;
            if (*p=='\0') isnum=0;
            else {
                char *q=p; if (*q=='+'||*q=='-') q++;
                int dot=0;
                while (*q) { if (*q=='.') dot++; else if (!isdigit((unsigned char)*q)) { isnum=0; break;} q++; }
                if (dot>1) isnum=0;
            }
            if (isnum) { sums[i] += atof(t); counts[i]++; }
        }
        free_fields(fields, c); row++;
    }
    fclose(f);
    printf("Rows: %d\n", row);
    for (int i=0;i<cols;i++) {
        if (counts[i]>0) printf("Col %d: avg=%.4f count=%d\n", i, sums[i]/counts[i], counts[i]);
        else printf("Col %d: non-numeric or empty\n", i);
    }
    free(sums); free(counts);
    return 0;
}

