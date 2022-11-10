#include "docDb.h"

char *strLower(char *s) {
    for (char *p=s; *p; p++)
        *p = tolower(*p);
    return s;
}

int strHash(char *s, int len) {
    int h = 371;
    for (int i=0; i<len; i++) {
        int c = (uint8_t) s[i];
        h = ((h << 5) - h + c)%HASH_SIZE;
    }
    return h;
}

int intHash(int i) {
    return i%HASH_SIZE;
}

int fileSize(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    return ftell(fp);
}


bool isDir(char *path) {
    struct stat sb;
    return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

char *dbReadDoc(DB *db, idx_t offset) {
    fseek(db->dataFile, offset, SEEK_SET);
    char doc[DOC_SIZE];
    fgets(doc, DOC_SIZE-1, db->dataFile);
    return strdup(doc);
}

char *dbGetDoc(DB *db, idx_t offset) {
    int h = intHash(offset);
    Doc *docp = db->dc.docp[h];
    if (docp) {
        for (Doc *p = docp; p; p=p->next) {
            if (p->offset == offset) return p->doc;
        }
    }
    Doc *d1 = malloc(sizeof(Doc));
    d1->doc = dbReadDoc(db, offset);
    d1->next = docp;
    db->dc.docp[h] = d1;
    return d1->doc;
}

idx_t dbWriteDoc(DB *db, char *doc) {
    fseek(db->dataFile, 0, SEEK_END);
    idx_t offset = ftell(db->dataFile);
    fprintf(db->dataFile, "%s\n", doc);
    return offset;
}

void dbReadIndex(DB *db, int h) {
    if (db->diskIdx[h]) return; // 已經讀過了，直接傳回
    char idxFileName[STR_SIZE+20];
    sprintf(idxFileName, "%s/idx/%d", db->path, h);
    FILE *idxFile = fopen(idxFileName, "r+b");
    if (!idxFile) return; // 檔案開啟失敗，該檔案不存在！
    int size = fileSize(idxFile);
    printf("fileSize()=%d\n", size);
    db->diskIdx[h] = malloc(size);
    db->diskLen[h] = size/sizeof(idx_t);
    fseek(idxFile, 0L, SEEK_SET);
    fread(db->diskIdx[h], sizeof(idx_t), db->diskLen[h], idxFile);
    fclose(idxFile);
}

void dbFlushBuffer(DB *db, int h) {
    char idxFileName[STR_SIZE+20];
    sprintf(idxFileName, "%s/idx/%d", db->path, h);
    FILE *idxFile = fopen(idxFileName, "a+b");
    assert(idxFile);
    assert(db->bufLen[h] == BUF_SIZE);
    fwrite(db->bufIdx[h], sizeof(db->bufIdx[h]), 1, idxFile);
    fclose(idxFile);
}

void dbIndexWord(DB *db, char *word, int wordLen, idx_t offset) {
    int h = strHash(word, wordLen);
    ilen_t blen = db->bufLen[h];
    idx_t *buf = db->bufIdx[h];
    if (blen < BUF_SIZE) {
        buf[blen] = offset;
        db->bufLen[h]++;
    } else {
        dbFlushBuffer(db, h);
        db->bufLen[h]=0;
    }
}

void dbIndexDoc(DB *db, char *doc, idx_t offset) {
    assert(strlen(doc)<DOC_SIZE);
    char doc1[DOC_SIZE];
    strcpy(doc1, doc);
    strLower(doc1);
    char *dp = doc1;
    while (*dp) {
        char *p = dp;
        if (isalpha(*dp)) { // english word
            while (isalpha(*p)) p++;
            dbIndexWord(db, dp, p-dp, offset);
            dp = p;
        } else if (isdigit(*dp)) { // number
            while (isdigit(*p)) p++;
            dbIndexWord(db, dp, p-dp, offset);
            dp = p;
        } else if (memcmp(dp, "\":", 2)==0) { // json field: "name":int|"..."
            char *p0 = dp-1;
            while (*p0 != '"') p0--;
            char *p1 = dp+2;
            if (isdigit(*p1)) {
                while (*p1 != ',' && *p1 != '}') p1++;
                dbIndexWord(db, p0, p1-p0, offset);
                // debug("index field:%.*s\n", (int) (p1-p0), p0);
            } else if (*p1=='"') {
                p1++;
                while (*p1 != '"') p1++;
                p1++;
                dbIndexWord(db, p0, p1-p0, offset);
                // debug("index field:%.*s\n", (int) (p1-p0), p0);
            }
            dp++;
        } else if (*dp >= 0) { // other ASCII
            dp++;
        } else { // Non ASCII UTF8 code bytes.
            for (int i=0; i<4; i++) {
                int len = utf8len(p); 
                p += len;
                dbIndexWord(db, dp, p-dp, offset);
            }
            dp += utf8len(dp);
        }
    }
}

idx_t dbAddDoc(DB *db, char *doc) {
    idx_t offset = dbWriteDoc(db, doc);
    dbIndexDoc(db, doc, offset);
    return offset;
}

void dbSaveBuffer(DB *db) {
    char fname[STR_SIZE+20];
    sprintf(fname, "%s/buf.idx", db->path);
    FILE *fp = fopen(fname, "wb");
    fwrite(db->diskLen, sizeof(db->diskLen), 1, fp);
    fwrite(db->bufLen, sizeof(db->bufLen), 1, fp);
    fwrite(db->bufIdx, sizeof(db->bufIdx), 1, fp);
    fclose(fp);
}

void dbLoadBuffer(DB *db) {
    char fname[STR_SIZE+20];
    sprintf(fname, "%s/buf.idx", db->path);
    FILE *fp = fopen(fname, "rb");
    fread(db->diskLen, sizeof(db->diskLen), 1, fp);
    fread(db->bufLen, sizeof(db->bufLen), 1, fp);
    fread(db->bufIdx, sizeof(db->bufIdx), 1, fp);
    fclose(fp);
}

void dbOpen(DB *db, char *path) {
    memset(db, 0, sizeof(DB));
    strcpy(db->path, path);
    char dataFileName[STR_SIZE];
    sprintf(dataFileName, "%s/jdb.data", path);
    if (isDir(path)) {
        db->dataFile = fopen(dataFileName, "r+");
        db->meta = dbGetDoc(db, 0);
        dbLoadBuffer(db);
    } else {
        mkdir1(path);
        char idxPath[STR_SIZE];
        sprintf(idxPath, "%s/idx", path);
        mkdir1(idxPath);
        db->dataFile = fopen(dataFileName, "w+");
        db->meta = strdup("{db:jdb}");
        dbAddDoc(db, db->meta);
    }
}

void dbClose(DB *db) {
    fclose(db->dataFile);
}

void dbSave(DB *db) {
    dbSaveBuffer(db);
}

int intersect(idx_t *rIdx, idx_t *idx1, ilen_t len1, idx_t *idx2, ilen_t len2) {
    int i1=0, i2=0, ri=0;
    while (i1<len1 && i2<len2) {
        if (idx1[i1]<idx2[i2]) {
            i1++;
        } else if (idx1[i1]>idx2[i2]) {
            i2++;
        } else if (idx1[i1]==idx2[i2]) {
            rIdx[ri++] = idx1[i1];
            i1++; i2++;
        }
    }
    return ri;
}

int idxAppend(DB *db, idx_t *idx, ilen_t ilen, int h) {
    printf("h=%d diskLen=%d bufLen=%d\n", h, db->diskLen[h], db->bufLen[h]);
    memcpy(idx, db->diskIdx[h], (size_t) (db->diskLen[h]*sizeof(idx_t)));
    ilen += db->diskLen[h];
    memcpy(&idx[ilen], db->bufIdx[h], (size_t) (db->bufLen[h]*sizeof(idx_t)));
    ilen += db->bufLen[h];
    return ilen;
}

int idxIntersection(DB *db, idx_t *idx, ilen_t ilen, int h) {
    idx_t *dIdx = db->diskIdx[h], *bIdx = db->bufIdx[h];
    ilen_t dLen = db->diskLen[h], bLen = db->bufLen[h];
    ilen_t len = dLen+bLen, rLen=0;
    idx_t rIdx[len], hIdx[len];
    memcpy(hIdx, dIdx, dLen*sizeof(idx_t));
    memcpy(hIdx+dLen, bIdx, bLen*sizeof(idx_t));
    rLen = intersect(rIdx, idx, ilen, hIdx, len);
    memcpy(idx, rIdx, rLen*sizeof(idx_t));
    return rLen;
}

int dbQueryIdx(DB *db, char *q, int *idx) {
    int ilen = 0;
    char *p = q, *word = q;
    printf("dbQueryIdx\n");
    while (1) {
        if (*p == ' ' || *p=='\0') {
            printf("word=%.*s\n", (int) (p-word), word);
            int h = strHash(word, p-word);
            dbReadIndex(db, h);
            if (word == q) {
                ilen = idxAppend(db, idx, 0, h);
            } else {
                ilen = idxIntersection(db, idx, ilen, h);
            }
            printf("ilen=%d\n", ilen);
            while (*p == ' ') p++;
            word = p;
            if (*p == '\0') break;
        } else {
            p++;
        }
    }
    return ilen;
}

int dbAddMatch(DB *db, idx_t *idx, ilen_t ilen, char *q1, char *docs, int maxLen) {
    int count = 0;
    char *dp = docs+strlen(docs);
    for (int i=0; i<ilen; i++) {
        // debug("idx[%d]=%d\n", i, index->idx[i]);
        char *doc = dbGetDoc(db, idx[i]);
        // debug("doc=%s\n", doc);
        char doc1[DOC_SIZE];
        strcpy(doc1, doc);
        strLower(doc1);
        char *qs = strstr(doc1, q1);
        if (qs) {
            if (dp-docs+strlen(doc) >= maxLen-1) break;
            sprintf(dp, "%s", doc);
            dp += strlen(dp);
            count ++;
        }
    }
    return count;
}

char *dbMatch(DB *db, char *q, char *follow, char *docs, int maxLen) {
    assert(strlen(q)<STR_SIZE);
    char q1[STR_SIZE];
    strcpy(q1, q);
    strLower(q1);
    debug("q1=%s\n", q1);
    idx_t idx[BUF_SIZE*10];
    int ilen = dbQueryIdx(db, q1, idx);
    // int h = strHash(q1, strlen(q1));
    // dbReadIndex(db, h);
    debug("ilen=%d\n", ilen);
    docs[0] = '\0';
    int count = dbAddMatch(db, idx, ilen, q1, docs, maxLen);
    debug("docs=%s\ncount=%d\n", docs, count);
    return docs;
}

/*
char *dbMatch(DB *db, char *q, char *follow, char *docs, int maxLen) {
    assert(strlen(q)<STR_SIZE);
    char q1[STR_SIZE];
    strcpy(q1, q);
    strLower(q1);
    debug("q1=%s\n", q1);
    int h = strHash(q1, strlen(q1));
    dbReadIndex(db, h);
    // ilen = dbQueryIdx(db, q, idx) 這裡要測試！
    docs[0] = '\0';
    int iCount = dbAddMatch(db, db->diskIdx[h], db->diskLen[h], q1, follow, docs, maxLen);
    int bCount = dbAddMatch(db, db->bufIdx[h], db->bufLen[h], q1, follow, docs, maxLen);
    debug("docs=%s\niCount=%d\nbCount=%d\n", docs, iCount, bCount);
    return docs;
}
*/