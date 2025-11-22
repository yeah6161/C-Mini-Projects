#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#define DB "students.db"

int main() {
    Student *arr = NULL; int count = 0;
    arr = load_students(DB, &count);
    if (!arr) { arr = NULL; count = 0; }
    while (1) {
        printf("\nStudent Grades\n1.Add Student 2.Add Score 3.List 4.Ranking 5.Export CSV 6.Save & Exit\nChoose: ");
        int c; if (scanf("%d%*c",&c)!=1) break;
        if (c==1) {
            char name[128]; printf("Name: "); fgets(name, sizeof(name), stdin); name[strcspn(name,"\n")]=0;
            int id = add_student(&arr, &count, name);
            printf("Added id %d\n", id);
        } else if (c==2) {
            int id; double sc; printf("Student ID: "); scanf("%d%*c",&id);
            Student *s = find_student(arr, count, id);
            if (!s) { printf("Not found\n"); continue; }
            printf("Score: "); scanf("%lf%*c",&sc);
            add_score(s, sc);
        } else if (c==3) {
            for (int i=0;i<count;i++) {
                printf("%d | %s | avg: %.2f\n", arr[i].id, arr[i].name, average_student(&arr[i]));
            }
        } else if (c==4) {
            // simple ranking copy
            Student *copy = malloc(sizeof(Student)*count);
            for (int i=0;i<count;i++) copy[i]=arr[i];
            for (int i=0;i<count;i++) {
                for (int j=i+1;j<count;j++) {
                    if (average_student(&copy[j]) > average_student(&copy[i])) {
                        Student tmp = copy[i]; copy[i]=copy[j]; copy[j]=tmp;
                    }
                }
            }
            for (int i=0;i<count;i++) printf("%d | %s | avg: %.2f\n", copy[i].id, copy[i].name, average_student(&copy[i]));
            free(copy);
        } else if (c==5) {
            FILE *f = fopen("students.csv","w");
            fprintf(f,"id,name,avg\n");
            for (int i=0;i<count;i++) fprintf(f,"%d,%s,%.2f\n", arr[i].id, arr[i].name, average_student(&arr[i]));
            fclose(f); printf("Exported students.csv\n");
        } else if (c==6) {
            save_students(DB, arr, count);
            printf("Saved. Bye.\n"); break;
        } else printf("Invalid\n");
    }
    free_students(arr, count);
    return 0;
}

