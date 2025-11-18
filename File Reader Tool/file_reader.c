#include <stdio.h>

int main() {
    char name[50];
    printf("File name: ");
    scanf("%s", name);

    FILE *f = fopen(name, "r");
    if (!f) {
        printf("File not found.\n");
        return 0;
    }

    char line[200];
    while (fgets(line, sizeof(line), f))
        printf("%s", line);

    fclose(f);
    return 0;
}
