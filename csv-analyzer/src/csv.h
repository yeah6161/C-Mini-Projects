#ifndef CSV_H
#define CSV_H
char **parse_line(const char *line, int *cols);
void free_fields(char **f, int cols);
#endif

