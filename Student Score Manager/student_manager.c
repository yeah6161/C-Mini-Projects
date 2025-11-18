#include <stdio.h>

struct Student {
    char name[50];
    int score;
};

int main() {
    struct Student s;

    printf("Name: ");
    scanf("%s", s.name);

    printf("Score: ");
    scanf("%d", &s.score);

    printf("\nStudent Info:\n");
    printf("Name: %s\nScore: %d\n", s.name, s.score);

    return 0;
}
