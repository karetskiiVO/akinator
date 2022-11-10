#include "solvetree.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define check(ptr) (ptr == NULL)?("(empty)"):(ptr)

tree_t* addNode (const char* text) {
    tree_t* buf = (tree_t*)calloc(1, sizeof(tree_t));
    buf->text   = (char*)calloc(strlen(text) + 1, sizeof(char));
    strcpy(buf->text, text);

    return buf;
}

void changeText (tree_t* node, const char* text) {
    if (strlen(node->text) < strlen(text)){
        free(node->text);
        node->text = (char*)calloc(strlen(text) + 1, sizeof(char));
        strcpy(node->text, text);
        return;
    }

    strcpy(node->text, text);    
}

void findElem (tree_t* tree) {
    if (tree->right == NULL && tree->right == NULL) {
        printf("Я знаю это %s.\n", tree->text); // here
        return;
    }
    printf("%s?\n(y/n):", tree->text);
    ///function
    char ch = tolower(getchar());
    while (!strchr("yn", ch)) ch = tolower(getchar());

    if (ch == 'y') {
        findElem(tree->right);
    } else {
        findElem(tree->left);
    }
}

static tree_t* readTree (FILE* file) {
    char  ch = '"';
    char* string = (char*)calloc(100, sizeof(char)); // const    

    fscanf(file, "%*s \"%[^\"]%s", string);

    tree_t* buf = addNode(string);
    
    while (strchr("\"\n\r \t", ch)) ch = fgetc(file);
    if (ch == '{') {
        ungetc(ch, file);

        buf->left  = readTree(file);
        buf->right = readTree(file);

        //printf("%s <- %s -> %s\n", check(buf->left->text), check(buf->text), check(buf->right->text));

        while (fgetc(file) != '}');
    }
    free(string);
    return buf;
}

tree_t* loadData (const char* filename) {
    FILE* readfile = fopen(filename, "r");
    if (readfile == NULL) {
        return NULL;
    }

    tree_t* buf = readTree(readfile);
    fclose(readfile);
    return buf;
}

static void treeDump_node (tree_t* tree) {
    FILE* file = fopen("tree.dot", "a"); /// change

    fprintf(file, "\t\tstruct%p [label=\"<root>pointer:%p | -%s- | { <lt> %p | <rt> %p}\" color=\"olivedrab1\"];\n", 
            tree, tree, tree->text, tree->left, tree->right);
    fclose(file);

    if (tree->left ) treeDump_node(tree->left); 
    if (tree->right) treeDump_node(tree->right); 
}

static void treeDump_edge (tree_t* tree) {
    FILE* file = fopen("tree.dot", "a"); /// change

    if (tree->left ) fprintf(file, "\tstruct%p -> struct%p [ label = \"Нет\" ]\n", tree, tree->left);
    if (tree->right) fprintf(file, "\tstruct%p -> struct%p [ label = \"Да\" ]\n", tree, tree->right);

    fclose(file);

    if (tree->left ) treeDump_edge(tree->left); 
    if (tree->right) treeDump_edge(tree->right); 
}

void treeDump (tree_t* tree) {
    static size_t cnt = 0;
    FILE* file = fopen("tree.dot", "w");

    fprintf(file, "digraph g {\n{\n\t\tnode [shape=record];\n");
    fclose(file);
    
    treeDump_node(tree);
    file = fopen("tree.dot", "a");
    fprintf(file, "\t}\n");
    fclose(file);
    treeDump_edge(tree);

    file = fopen("tree.dot", "a");
    fprintf(file, "}\n");
    fclose(file);

    system("dot -Tpng tree.dot > pic.png");
}

static void writeTree (FILE* file, tree_t* tree, size_t lvl) {
    for (int i = 0; i < lvl; i++) fprintf(file, "\t"); // change
    if (tree->left == NULL && tree->right == NULL) {
        fprintf(file, "{ \"%s\" }\n", tree->text);
        return;
    }
    fprintf(file, "{ \"%s\"\n", tree->text);

    writeTree(file, tree->left,  lvl + 1);
    writeTree(file, tree->right, lvl + 1);

    for (int i = 0; i < lvl; i++) fprintf(file, "\t");
    fprintf(file, "}\n");
}

void saveData (const char* filename, tree_t* tree) {
    FILE* writefile = fopen(filename, "w");
    writeTree(writefile, tree, 0);
    fclose(writefile);
}



