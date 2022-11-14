#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define NCHILD 5

typedef struct bptNode {
	bool isLeaf;
	uint16_t nkey;
	union {
		int key[NCHILD];
		char *skey[NCHILD];
	};
	void* child[NCHILD]; // 樹葉的 child 指向 value，中間指向 bptNode
	struct bptNode* father;
	struct bptNode* next;
	struct bptNode* last;
} bptNode;

// extern void bptSetMaxChildNumber(int);
extern void bptInit();
extern void bptDestroy();
extern int bptInsert(int, void*);
extern int bptGetTotalNodes();
extern int bptQueryKey(int);
extern int bptQueryRange(int, int);
extern void bptModify(int, void*);
extern void bptDelete(int);
extern int bptFind(int key);
