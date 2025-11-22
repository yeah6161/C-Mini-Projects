#include <stdio.h>
#include <string.h>
#include "kv.h"

#define DB "kv.db"

int main() {
    kv_load(DB);
    char cmd[256];
    while (1) {
        printf("kv> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        cmd[strcspn(cmd,"\n")]=0;
        if (strncmp(cmd,"SET ",4)==0) {
            char *rest = cmd+4;
            char *eq = strchr(rest,' ');
            if (!eq) { printf("Usage: SET key value\n"); continue; }
            *eq=0;
            kv_set(rest, eq+1);
            printf("OK\n");
        } else if (strncmp(cmd,"GET ",4)==0) {
            char *k = cmd+4;
            const char *v = kv_get(k);
            if (v) printf("%s\n", v); else printf("(nil)\n");
        } else if (strncmp(cmd,"DEL ",4)==0) {
            char *k = cmd+4; if (kv_delete(k)) printf("OK\n"); else printf("NotFound\n");
        } else if (strcmp(cmd,"SAVE")==0) {
            kv_save(DB); printf("Saved\n");
        } else if (strcmp(cmd,"EXIT")==0) {
            kv_save(DB); break;
        } else {
            printf("Commands: SET key value | GET key | DEL key | SAVE | EXIT\n");
        }
    }
    kv_free();
    return 0;
}

