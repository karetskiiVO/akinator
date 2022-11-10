#ifndef SOLVETREE
#define SOLVETREE

#include <stdio.h>

typedef struct tree_t {
    char* text;
    tree_t* left;
    tree_t* right;
};

tree_t* addNode (const char* text);
void changeText (tree_t* node, const char* text);
void findElem (tree_t* tree);
tree_t* loadData (const char* filename);
void saveData (const char* filename, tree_t* tree);
void treeDump (tree_t* tree);

#endif