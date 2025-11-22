#ifndef DB_H
#define DB_H
int db_insert(const char *path, const char *key, const char *value);
char *db_search(const char *path, const char *key);
#endif

