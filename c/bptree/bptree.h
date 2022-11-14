#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NCHILD 5

typedef struct bptNode {
	int isRoot, isLeaf;
	int nkey;
	int key[NCHILD];
	int pos[NCHILD];
	void* child[NCHILD];
	struct bptNode* father;
	struct bptNode* next;
	struct bptNode* last;
} bptNode;

// extern void bptSetMaxChildNumber(int);
extern void bptInit();
extern void bptDestroy();
extern int bptInsert(int, int, void*);
extern int bptGetTotalNodes();
extern int bptQueryKey(int);
extern int bptQueryRange(int, int);
extern void bptModify(int, void*);
extern void bptDelete(int);
extern int bptFind(int key);
