#ifndef SOLVETREE
#define SOLVETREE

#define STR_LEN 1000

#include "cstack.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct tree_t {
    char* text;
    tree_t* left;
    tree_t* right;
};

tree_t* addNode (const char* text);
void changeText (tree_t* node, const char* text);
void guesElem(tree_t* tree);
tree_t* loadData (const char* filename);
void saveData (const char* filename, tree_t* tree);
void treeDump (tree_t* tree);
void defElem (const tree_t* tree, Stack* def, const char* elem);
Stack* findElem (const tree_t* tree, const char* elem);
void removeTree (tree_t* tree);
void cmpElem (const tree_t* tree, Stack* def1, Stack* def2, const char* elem1, const char* elem2);

#endif