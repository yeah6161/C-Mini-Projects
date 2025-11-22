#define _GNU_SOURCE
#include "fsearch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

typedef struct {
    char filepath[1024];
    char pattern[256];
} Job;

#define MAXQ 1024
static Job q[MAXQ];
static int qh=0, qt=0;
static pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t qcond = PTHREAD_COND_INITIALIZER;
static int active_workers = 0;

void enqueue(const char *path, const char *pattern) {
    pthread_mutex_lock(&qlock);
    strncpy(q[qt].filepath, path, 1023); q[qt].filepath[1023]=0;
    strncpy(q[qt].pattern, pattern, 255); q[qt].pattern[255]=0;
    qt = (qt+1)%MAXQ;
    pthread_cond_signal(&qcond);
    pthread_mutex_unlock(&qlock);
}

int dequeue(Job *out) {
    pthread_mutex_lock(&qlock);
    while (qh==qt) { // empty
        if (active_workers==0) { pthread_mutex_unlock(&qlock); return 0; }
        pthread_cond_wait(&qcond, &qlock);
    }
    *out = q[qh];
    qh = (qh+1)%MAXQ;
    pthread_mutex_unlock(&qlock);
    return 1;
}

void *worker(void *arg) {
    Job job;
    while (dequeue(&job)) {
        FILE *f = fopen(job.filepath, "r");
        if (!f) continue;
        char line[2048];
        int lineno=0;
        while (fgets(line, sizeof(line), f)) {
            lineno++;
            if (strstr(line, job.pattern)) {
                printf("FOUND %s:%d: %s", job.filepath, lineno, line);
            }
        }
        fclose(f);
    }
    return NULL;
}

void walk(const char *base, const char *pattern) {
    DIR *d = opendir(base);
    if (!d) return;
    struct dirent *ent;
    while ((ent = readdir(d))) {
        if (strcmp(ent->d_name,".")==0||strcmp(ent->d_name,"..")==0) continue;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", base, ent->d_name);
        if (ent->d_type == DT_DIR) walk(path, pattern);
        else enqueue(path, pattern);
    }
    closedir(d);
}

void search_in_dir(const char *dirpath, const char *pattern) {
    int threads = 4;
    pthread_t tids[threads];
    active_workers = threads;
    for (int i=0;i<threads;i++) pthread_create(&tids[i], NULL, worker, NULL);
    walk(dirpath, pattern);
    // mark no more jobs by setting active_workers to 0 and signaling
    pthread_mutex_lock(&qlock);
    active_workers = 0;
    pthread_cond_broadcast(&qcond);
    pthread_mutex_unlock(&qlock);
    for (int i=0;i<threads;i++) pthread_join(tids[i], NULL);
}

