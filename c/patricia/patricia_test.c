#include "patricia.h"

int main() {
    // char text[] = "abcabababacba";
    char text[] = "abcab";
    int n = strlen(text);
    patNode *root = patInsert(NULL, text, 1*8);
    for (int i=0; i<n; i++) {
        for (int len=1; len<=4; len++) {
            if (i+len <= n) {
                // printf("main:patInsert:%.*s\n", len, &text[i]);
                patInsert(root, &text[i], len*8);
            }
        }
    }
    patNode *node = patSearch(root, "ab", 2*8);
    printf("search(ab,2):"); patPrint(node);
    printf("========== dump ==============\n");
    patDump(root, 0);
}
