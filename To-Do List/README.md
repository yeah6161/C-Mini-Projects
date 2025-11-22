#include <stdio.h>
#include <string.h>

int main() {
    char tasks[10][100];
    int count = 0;

    while (1) {
        int choice;
        printf("1.Add 2.List 3.Exit → ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Task: ");
            fgets(tasks[count], 100, stdin);
            tasks[count][strcspn(tasks[count], "\n")] = 0;
            count++;
        }
        else if (choice == 2) {
            for (int i = 0; i < count; i++)
                printf("- %s\n", tasks[i]);
        }
        else break;
    }
    return 0;
}
