#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "utf8/utf8.h"

#define STR_SIZE 256
#define DOC_SIZE 4096
#define DOCS_SIZE DOC_SIZE*10
#define BUF_SIZE 1024
#define HASH_SIZE 4096
#ifdef _DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif
typedef int32_t idx_t;
typedef int32_t ilen_t;

typedef struct Doc {
    idx_t offset;
    char *doc;
    struct Doc *next;
} Doc;

typedef struct DocCache {
    Doc *docp[HASH_SIZE];
} DocCache;

typedef struct Index {
    idx_t *diskIdx, *bufIdx;
    int diskLen, bufLen;
} Index;

typedef struct DB {
    char path[STR_SIZE];
    char *meta;
    idx_t *diskIdx[HASH_SIZE];
    ilen_t diskLen[HASH_SIZE];
    idx_t bufIdx[HASH_SIZE][BUF_SIZE];
    ilen_t bufLen[HASH_SIZE];
    DocCache dc;
    FILE *dataFile;
} DB;

void dbOpen(DB *db, char *path);
idx_t dbAddDoc(DB *db, char *doc);
void dbSave(DB *db);
void dbClose(DB *db);
char *dbMatch(DB *db, char *q, char *follow, char *docs, int docsMaxSize);

// https://stackoverflow.com/questions/8666378/detect-windows-or-linux-in-c-c
#ifdef _WIN32
#define mkdir1(path) mkdir(path)
#else
#define mkdir1(path) mkdir(path, 744)
#endif
