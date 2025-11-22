#ifndef STUDENT_H
#define STUDENT_H
#define MAX_NAME 128
typedef struct {
    int id;
    char name[MAX_NAME];
    double *scores;
    int score_count;
} Student;

Student *load_students(const char *path, int *count);
int save_students(const char *path, Student *arr, int count);
int add_student(Student **arr, int *count, const char *name);
void add_score(Student *s, double score);
Student *find_student(Student *arr, int count, int id);
double average_student(Student *s);
void free_students(Student *arr, int count);
#endif

