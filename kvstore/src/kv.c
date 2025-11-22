#include "kv.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SIZE 1024
typedef struct Node {
    char *k; char *v;
    struct Node *next;
} Node;

static Node *table[SIZE];

unsigned int hash(const char *s) {
    unsigned int h=5381;
    while (*s) h = ((h<<5) + h) + (unsigned char)(*s++);
    return h % SIZE;
}

int kv_set(const char *k, const char *v) {
    unsigned int h = hash(k);
    Node *n = table[h];
    while (n) { if (strcmp(n->k,k)==0) { free(n->v); n->v=strdup(v); return 1;} n=n->next;}
    Node *nn = malloc(sizeof(Node));
    nn->k = strdup(k); nn->v = strdup(v); nn->next = table[h]; table[h]=nn;
    return 1;
}

const char *kv_get(const char *k) {
    unsigned int h = hash(k);
    Node *n = table[h];
    while (n) { if (strcmp(n->k,k)==0) return n->v; n=n->next; }
    return NULL;
}

int kv_delete(const char *k) {
    unsigned int h = hash(k);
    Node **p = &table[h];
    while (*p) {
        if (strcmp((*p)->k,k)==0) {
            Node *t = *p;
            *p = t->next;
            free(t->k); free(t->v); free(t);
            return 1;
        }
        p=&(*p)->next;
    }
    return 0;
}

int kv_save(const char *path) {
    FILE *f = fopen(path,"w"); if (!f) return 0;
    for (int i=0;i<SIZE;i++) {
        Node *n = table[i];
        while (n) { fprintf(f,"%s=%s\n", n->k, n->v); n = n->next; }
    }
    fclose(f); return 1;
}

int kv_load(const char *path) {
    FILE *f = fopen(path,"r"); if (!f) return 0;
    char line[2048];
    while (fgets(line,sizeof(line),f)) {
        line[strcspn(line,"\n")]=0;
        char *eq = strchr(line,'=');
        if (!eq) continue;
        *eq=0;
        kv_set(line, eq+1);
    }
    fclose(f); return 1;
}

void kv_free() {
    for (int i=0;i<SIZE;i++) {
        Node *n = table[i];
        while (n) { Node *t=n; n=n->next; free(t->k); free(t->v); free(t); }
        table[i]=NULL;
    }
}

