#include "docDB.h"

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

void indexRead(Index *index, FILE *fp) {
    if (index->loaded) return; // 已經讀過了，直接傳回
    int size = fileSize(fp);
    index->idx = malloc(size);
    index->len = size/sizeof(idx_t);
    fseek(fp, 0L, SEEK_SET);
    fread(index->idx, sizeof(idx_t), index->len, fp);
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

Index *dbReadIndex(DB *db, int h) {
    Index *index = &db->index[h];
    if (index->loaded) return index;
    char idxFileName[STR_SIZE+20];
    sprintf(idxFileName, "%s/idx/%d", db->path, h);
    FILE *idxFile = fopen(idxFileName, "r+b");
    if (!idxFile) return index; // 檔案開啟失敗，該檔案不存在！
    indexRead(index, idxFile);
    fclose(idxFile);
    return index;
}

void dbAppendIndexFile(DB *db, int h, IndexBuffer *ibuf) {
    char idxFileName[STR_SIZE+20];
    sprintf(idxFileName, "%s/idx/%d", db->path, h);
    FILE *idxFile = fopen(idxFileName, "a+b");
    assert(idxFile);
    assert(ibuf->len == BUF_SIZE);
    fwrite(ibuf, sizeof(IndexBuffer), 1, idxFile);
    fclose(idxFile);
}

void dbIndexWord(DB *db, char *word, int wordLen, idx_t offset) {
    int h = strHash(word, wordLen);
    IndexBuffer *ibuf = &db->ibuf[h];
    if (ibuf->len < BUF_SIZE) {
        ibuf->idx[ibuf->len++] = offset;
    } else {
        dbAppendIndexFile(db, h, ibuf);
        ibuf->len = 0;
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
    fwrite(db->ibuf, sizeof(db->ibuf), 1, fp);
    fclose(fp);
}

void dbLoadBuffer(DB *db) {
    char fname[STR_SIZE+20];
    sprintf(fname, "%s/buf.idx", db->path);
    FILE *fp = fopen(fname, "rb");
    fread(db->ibuf, sizeof(db->ibuf), 1, fp);
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

char *dbAddMatch(DB *db, Index *index, char *q1, char *follow, char *docs, char *dp, int maxLen) {
    for (int i=0; i<index->len; i++) {
        // debug("idx[%d]=%d\n", i, index->idx[i]);
        char *doc = dbGetDoc(db, index->idx[i]);
        // debug("doc=%s\n", doc);
        char doc1[DOC_SIZE];
        strcpy(doc1, doc);
        strLower(doc1);
        char *qs = strstr(doc1, q1);
        if (qs && (follow == NULL || strchr(follow, qs[strlen(q1)]))) {
            if (dp-docs+strlen(doc) >= maxLen-1) break;
            sprintf(dp, "%s", doc);
            dp += strlen(dp);
        }
    }
    return dp;
}

char *dbMatch(DB *db, char *q, char *follow, char *docs, int maxLen) {
    assert(strlen(q)<STR_SIZE);
    char q1[STR_SIZE];
    strcpy(q1, q);
    strLower(q1);
    debug("q1=%s\n", q1);
    int h = strHash(q1, strlen(q1));
    Index *index = dbReadIndex(db, h);
    char *dp = docs;
    dp = dbAddMatch(db, index, q1, follow, docs, dp, maxLen);
    Index bufIndex;
    bufIndex.len = db->ibuf[h].len;
    bufIndex.idx = db->ibuf[h].idx;
    dp = dbAddMatch(db, &bufIndex, q1, follow, docs, dp, maxLen);
    debug("docs=%s\n", docs);
    return docs;
}
