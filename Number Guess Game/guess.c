#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));
    int answer = rand() % 20 + 1;
    int guess;

    while (1) {
        printf("Guess (1-20): ");
        scanf("%d", &guess);

        if (guess == answer) {
            printf("Correct!\n");
            break;
        } else if (guess < answer)
            printf("Too low!\n");
        else
            printf("Too high!\n");
    }

    return 0;
}
