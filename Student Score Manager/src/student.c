#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Student *load_students(const char *path, int *count) {
    *count = 0;
    * (void**)& (*count); // silence no-use
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    int n;
    if (fread(&n, sizeof(int), 1, f)!=1) { fclose(f); return NULL;}
    Student *arr = malloc(sizeof(Student)*n);
    for (int i=0;i<n;i++) {
        arr[i].scores = NULL; arr[i].score_count=0;
        fread(&arr[i].id, sizeof(int),1,f);
        fread(arr[i].name, sizeof(char), MAX_NAME, f);
        int sc;
        fread(&sc, sizeof(int),1,f);
        if (sc>0) {
            arr[i].scores = malloc(sizeof(double)*sc);
            fread(arr[i].scores, sizeof(double), sc, f);
            arr[i].score_count = sc;
        }
    }
    fclose(f);
    *count = n;
    return arr;
}

int save_students(const char *path, Student *arr, int count) {
    FILE *f = fopen(path, "wb");
    if (!f) return 0;
    fwrite(&count, sizeof(int), 1, f);
    for (int i=0;i<count;i++) {
        fwrite(&arr[i].id, sizeof(int),1,f);
        fwrite(arr[i].name, sizeof(char), MAX_NAME, f);
        fwrite(&arr[i].score_count, sizeof(int),1,f);
        if (arr[i].score_count>0) fwrite(arr[i].scores, sizeof(double), arr[i].score_count, f);
    }
    fclose(f);
    return 1;
}

int add_student(Student **arr, int *count, const char *name) {
    int id = 1;
    for (int i=0;i<*count;i++) if ((*arr)[i].id >= id) id = (*arr)[i].id + 1;
    *arr = realloc(*arr, sizeof(Student) * (*count + 1));
    Student *s = &(*arr)[*count];
    s->id = id; strncpy(s->name, name, MAX_NAME-1); s->name[MAX_NAME-1]=0;
    s->scores = NULL; s->score_count = 0;
    (*count)++;
    return id;
}

void add_score(Student *s, double score) {
    s->scores = realloc(s->scores, sizeof(double) * (s->score_count + 1));
    s->scores[s->score_count++] = score;
}

Student *find_student(Student *arr, int count, int id) {
    for (int i=0;i<count;i++) if (arr[i].id==id) return &arr[i];
    return NULL;
}

double average_student(Student *s) {
    if (s->score_count==0) return 0;
    double sum=0;
    for (int i=0;i<s->score_count;i++) sum += s->scores[i];
    return sum / s->score_count;
}

void free_students(Student *arr, int count) {
    for (int i=0;i<count;i++) free(arr[i].scores);
    free(arr);
}

