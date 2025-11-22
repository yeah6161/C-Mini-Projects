#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

#define DB "sorted.db"

int main() {
    while (1) {
        printf("\n1.Insert 2.Search 3.Exit\nChoose: ");
        int c; if (scanf("%d%*c",&c)!=1) break;
        if (c==1) {
            char key[256], val[1024];
            printf("Key: "); fgets(key, sizeof(key), stdin); key[strcspn(key,"\n")]=0;
            printf("Value: "); fgets(val, sizeof(val), stdin); val[strcspn(val,"\n")]=0;
            db_insert(DB, key, val);
        } else if (c==2) {
            char key[256]; printf("Key: "); fgets(key,sizeof(key),stdin); key[strcspn(key,"\n")]=0;
            char *v = db_search(DB, key);
            if (v) { printf("Value: %s\n", v); free(v); } else printf("Not found\n");
        } else break;
    }
    return 0;
}

