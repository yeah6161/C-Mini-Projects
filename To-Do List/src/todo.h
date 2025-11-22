#ifndef TODO_H
#define TODO_H

#include <time.h>

typedef struct {
    int id;
    char title[256];
    char note[512];
    int priority; // 1-5
    char due[11]; // YYYY-MM-DD or empty
    int done; // 0/1
} Todo;

int load_todos(const char *path, Todo **arr, int *count);
int save_todos(const char *path, Todo *arr, int count);
int add_todo(Todo **arr, int *count, const char *title, const char *note, int priority, const char *due);
Todo *find_todo(Todo *arr, int count, int id);

#endif

