#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"

#define DB "todos.db"

void print_todo(Todo *t) {
    printf("%d | [%s] P:%d Due:%s\n  %s\n", t->id, t->done ? "x" : " ", t->priority, t->due[0]?t->due:"-", t->title);
}

int main() {
    Todo *arr = NULL;
    int count = 0;
    load_todos(DB, &arr, &count);

    while (1) {
        printf("\nTODO CLI\n1) List 2) Add 3) Done 4) Delete 5) Search 6) Save & Exit\nChoose: ");
        char cmd[10];
        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        int c = atoi(cmd);

        if (c == 1) {
            for (int i=0;i<count;i++) print_todo(&arr[i]);
        } else if (c == 2) {
            char title[256], note[512], due[16];
            int pr;
            printf("Title: "); fgets(title, sizeof(title), stdin); title[strcspn(title,"\n")]=0;
            printf("Note: "); fgets(note, sizeof(note), stdin); note[strcspn(note,"\n")]=0;
            printf("Priority (1-5): "); scanf("%d%*c", &pr);
            printf("Due (YYYY-MM-DD or empty): "); fgets(due, sizeof(due), stdin); due[strcspn(due,"\n")]=0;
            int id = add_todo(&arr, &count, title, note, pr, due[0]?due:NULL);
            printf("Added with id %d\n", id);
        } else if (c == 3) {
            int id; printf("ID to mark done: "); scanf("%d%*c", &id);
            Todo *t = find_todo(arr, count, id);
            if (t) { t->done = 1; printf("Marked.\n"); } else printf("Not found\n");
        } else if (c == 4) {
            int id; printf("ID to delete: "); scanf("%d%*c", &id);
            int idx=-1;
            for (int i=0;i<count;i++) if (arr[i].id==id) idx=i;
            if (idx>=0) {
                for (int j=idx;j<count-1;j++) arr[j]=arr[j+1];
                count--;
                arr = realloc(arr, sizeof(Todo)* (count>0?count:1));
                printf("Deleted.\n");
            } else printf("Not found\n");
        } else if (c == 5) {
            char kw[256]; printf("Keyword: "); fgets(kw, sizeof(kw), stdin); kw[strcspn(kw,"\n")]=0;
            for (int i=0;i<count;i++) if (strstr(arr[i].title, kw) || strstr(arr[i].note, kw)) print_todo(&arr[i]);
        } else if (c == 6) {
            save_todos(DB, arr, count);
            printf("Saved. Bye.\n");
            break;
        } else printf("Invalid\n");
    }

    free(arr);
    return 0;
}

