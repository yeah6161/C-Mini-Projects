#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int db_insert(const char *path, const char *key, const char *value) {
    // read all lines into memory, each "key|value\n", keep sorted by key
    FILE *f = fopen(path,"r");
    char **lines = NULL; int n=0;
    if (f) {
        char buf[4096];
        while (fgets(buf,sizeof(buf),f)) {
            lines = realloc(lines, sizeof(char*)*(n+1));
            lines[n++] = strdup(buf);
        }
        fclose(f);
    }
    // insert or replace
    int pos = 0;
    while (pos < n) {
        char *k = strdup(lines[pos]);
        char *p = strchr(k,'|'); if (p) *p=0;
        int cmp = strcmp(k, key); free(k);
        if (cmp==0) { // replace
            free(lines[pos]);
            char nl[4096]; snprintf(nl,sizeof(nl), "%s|%s\n", key, value);
            lines[pos] = strdup(nl);
            // write back
            FILE *fw = fopen(path,"w");
            for (int i=0;i<n;i++) fputs(lines[i], fw);
            fclose(fw);
            for (int i=0;i<n;i++) free(lines[i]); free(lines);
            return 1;
        } else if (cmp>0) break;
        pos++;
    }
    // insert at pos
    lines = realloc(lines, sizeof(char*)*(n+1));
    for (int i=n;i>pos;i--) lines[i]=lines[i-1];
    char nl[4096]; snprintf(nl,sizeof(nl), "%s|%s\n", key, value);
    lines[pos] = strdup(nl);
    n++;
    FILE *fw = fopen(path,"w");
    for (int i=0;i<n;i++) fputs(lines[i], fw);
    fclose(fw);
    for (int i=0;i<n;i++) free(lines[i]); free(lines);
    return 1;
}

char *db_search(const char *path, const char *key) {
    FILE *f = fopen(path,"r"); if (!f) return NULL;
    char buf[4096];
    while (fgets(buf,sizeof(buf),f)) {
        char *p = strchr(buf,'|'); if (!p) continue;
        *p = 0;
        if (strcmp(buf, key)==0) {
            char *val = strdup(p+1);
            val[strcspn(val,"\r\n")]=0;
            fclose(f);
            return val;
        }
    }
    fclose(f); return NULL;
}

