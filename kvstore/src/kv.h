#ifndef KV_H
#define KV_H
int kv_set(const char *k, const char *v);
const char *kv_get(const char *k);
int kv_delete(const char *k);
int kv_save(const char *path);
int kv_load(const char *path);
void kv_free();
#endif

