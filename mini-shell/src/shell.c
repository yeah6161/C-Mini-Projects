#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    char *line = NULL; size_t len = 0;
    while (1) {
        printf("mini-shell$ ");
        if (getline(&line, &len, stdin) == -1) break;
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line,"exit")==0) break;
        // simple tokenization
        char *args[128]; int i=0;
        char *tok = strtok(line, " ");
        while (tok && i<127) { args[i++]=tok; tok = strtok(NULL, " "); }
        args[i]=NULL;
        if (i==0) continue;
        if (strcmp(args[0],"cd")==0) {
            if (args[1]) chdir(args[1]); else chdir(getenv("HOME"));
            continue;
        }
        pid_t pid = fork();
        if (pid==0) {
            execvp(args[0], args);
            perror("exec"); exit(1);
        } else {
            int status; waitpid(pid, &status, 0);
        }
    }
    free(line);
    return 0;
}

