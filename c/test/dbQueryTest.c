#include <stdio.h>
#include <string.h>
#include "../docDB.h"

DB ddb;

int main() {
    utf8init();
    DB *db = &ddb;
    dbOpen(db, "../jdb");
    char docs[DOCS_SIZE];
    int maxLen = 300;
    dbMatch(db, "UNIX", NULL, docs, maxLen);
    dbMatch(db, "電磁場", NULL, docs, maxLen);
    dbMatch(db, "中村修二", NULL, docs, maxLen);
    dbClose(db);
}
