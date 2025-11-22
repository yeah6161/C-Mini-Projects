#include "todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_todos(const char *path, Todo **arr, int *count) {
    *count = 0;
    *arr = NULL;
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    while (!feof(f)) {
        Todo t;
        if (fscanf(f, "%d|%255[^|]|%511[^|]|%d|%10[^|]|%d\n",
                   &t.id, t.title, t.note, &t.priority, t.due, &t.done) == 6) {
            *arr = realloc(*arr, sizeof(Todo) * (*count + 1));
            (*arr)[*count] = t;
            (*count)++;
        } else break;
    }
    fclose(f);
    return 1;
}

int save_todos(const char *path, Todo *arr, int count) {
    FILE *f = fopen(path, "w");
    if (!f) return 0;
    for (int i = 0; i < count; ++i) {
        Todo *t = &arr[i];
        fprintf(f, "%d|%s|%s|%d|%s|%d\n", t->id, t->title, t->note, t->priority, t->due, t->done);
    }
    fclose(f);
    return 1;
}

int add_todo(Todo **arr, int *count, const char *title, const char *note, int priority, const char *due) {
    int id = 1;
    for (int i = 0; i < *count; ++i) if ((*arr)[i].id >= id) id = (*arr)[i].id + 1;
    *arr = realloc(*arr, sizeof(Todo) * (*count + 1));
    Todo *t = &(*arr)[*count];
    t->id = id;
    strncpy(t->title, title, 255); t->title[255]=0;
    strncpy(t->note, note, 511); t->note[511]=0;
    t->priority = priority;
    if (due) strncpy(t->due, due, 10); else t->due[0]=0;
    t->due[10]=0;
    t->done = 0;
    (*count)++;
    return id;
}

Todo *find_todo(Todo *arr, int count, int id) {
    for (int i=0;i<count;i++) if (arr[i].id==id) return &arr[i];
    return NULL;
}

