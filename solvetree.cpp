#include "solvetree.h"
#include "cvoice.h"
#include "cstack.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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

void guesElem(tree_t* tree) {
    if (tree->right == NULL && tree->right == NULL) {
        SAY("%s ", "Я знаю это");
        SAY("%s.\n", tree->text);

        printf("(y/n): ");
        char ch = tolower(getchar());
        while (!strchr("yn", ch)) ch = tolower(getchar());

        if (ch == 'y') {
            SAY("%s\n", "Я как всегда прав");
            return;
        }
        char newname[1000] = "";
        char newtext[1000] = "";
        char* bufn = NULL;

        while (getchar() != '\n');
        SAY("%s: ", "А кто тогда");
        fgets(newname, 1000, stdin);
        if ((bufn = strpbrk(newname, "\n\r"))) *bufn = '\0';

        SAY("%s ", "Чем");
        SAY("%s ", newname);
        SAY("%s ", "отличается от");
        SAY("%s ", tree->text);

        fgets(newtext, 1000, stdin);
        if ((bufn = strpbrk(newtext, "\n\r"))) *bufn = '\0';

        tree->left  = addNode(tree->text);
        tree->right = addNode(newname);

        changeText(tree, newtext);

        return;
    }
    printf("%s?\n(y/n):", tree->text);
    ///function
    char ch = tolower(getchar());
    while (!strchr("yn", ch)) ch = tolower(getchar());

    if (ch == 'y') {
        guesElem(tree->right);
    } else {
        guesElem(tree->left);
    }
}

static tree_t* readTree (FILE* file) {
    char  ch = '"';
    char* string = (char*)calloc(STR_LEN, sizeof(char));  

    fscanf(file, "%*s \"%[^\"]%s", string);

    tree_t* buf = addNode(string);
    
    while (strchr("\"\n\r \t", ch)) ch = fgetc(file);
    if (ch == '{') {
        ungetc(ch, file);

        buf->left  = readTree(file);
        buf->right = readTree(file);

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
    for (size_t i = 0; i < lvl; i++) fprintf(file, "\t"); // change
    if (tree->left == NULL && tree->right == NULL) {
        fprintf(file, "{ \"%s\" }\n", tree->text);
        return;
    }
    fprintf(file, "{ \"%s\"\n", tree->text);

    writeTree(file, tree->left,  lvl + 1);
    writeTree(file, tree->right, lvl + 1);

    for (size_t i = 0; i < lvl; i++) fprintf(file, "\t");
    fprintf(file, "}\n");
}

void saveData (const char* filename, tree_t* tree) {
    FILE* writefile = fopen(filename, "w");
    writeTree(writefile, tree, 0);
    fclose(writefile);
}

static const char* getPositive () {
    #define CNT 4
    const char* phrases[CNT] = {"является", "эквивалентен", "по существу", "аналогичен"};
    return phrases[rand() % CNT];
    #undef CNT
}

static const char* getNegative () {
    #define CNT 4
    const char* phrases[CNT] = {"отличен", "не", "не такой", "не обладает признаком"};
    return phrases[rand() % CNT];
    #undef CNT
}

static const char* getAnd () {
    #define CNT 4
    const char* phrases[CNT] = {"и", "как и", "похож на", "совсем как"};
    return phrases[rand() % CNT];
    #undef CNT
}

Stack* findElem (const tree_t* tree, const char* elem) {
    Stack* stk = NULL;

    if (tree->left == NULL && tree->right == NULL) {
        if (!strcmp(tree->text, elem)) {
            stk = (Stack*)malloc(sizeof(Stack));
            stackNew_(stk, 2);
            return stk;
        }
        return NULL;
    }
    
    stk = findElem(tree->left, elem);
    if (stk) {
        stackPush_(stk, 0);
        return stk;
    }

    stk = findElem(tree->right, elem);
    if (stk) {
        stackPush_(stk, 1);
        return stk;
    }

    return NULL;
}

void defElem (const tree_t* tree, Stack* def, const char* elem) {
    if (def->size == 0) return;
    char buf = -1;
    stackPop_(def, &buf);

    switch (buf) {
        case -1:
            SAY("%s\n", "Что-то пошло не так...");
            return;
        case 0:
            SAY("\t%s ", elem);
            SAY("%s ",  getNegative());
            SAY("%s\n", tree->text);

            defElem(tree->left, def, elem);
            break;
        case 1:
            SAY("\t%s ", elem);
            SAY("%s ",  getPositive());
            SAY("%s\n", tree->text);

            defElem(tree->right, def, elem);
            break;
    }
}

void cmpElem (const tree_t* tree, Stack* def1, Stack* def2, const char* elem1, const char* elem2) {
    if (def1->size == 0 && def2->size == 0) return;

    if (def1->size == 0) {
        SAY("%s:\n", "А вот их различия");
        defElem(tree, def2, elem2);
        return;
    } else if (def2->size == 0) {
        SAY("%s:\n", "А вот их различия");
        defElem(tree, def1, elem1);
        return;
    }

    char perk1 = -1, perk2 = -1;
    stackPop_(def1, &perk1);
    stackPop_(def2, &perk2);

    if (perk1 != perk2) {
        stackPush_(def1, perk1);
        stackPush_(def2, perk2);
        SAY("%s:\n", "А вот их различия");
        defElem(tree, def1, elem1);
        SAY("%s:\n", "Напротив");
        defElem(tree, def2, elem2);
    } else {
        switch (perk1) {
        case -1:
            SAY("%s\n", "Что-то пошло не так...");
            return;
        case 0:
            SAY("\t%s ", elem1);
            SAY("%s ", getAnd());
            SAY("%s ", elem2);
            SAY("%s ",  getNegative());
            SAY("%s\n", tree->text);

            cmpElem(tree->left, def1, def2, elem1, elem2);
            break;
        case 1:
            SAY("\t%s ", elem1);
            SAY("%s ", getAnd());
            SAY("%s ", elem2);
            SAY("%s ",  getPositive());
            SAY("%s\n", tree->text);

            cmpElem(tree->right, def1, def2, elem1, elem2);
            break;
        }
    }
}

void removeTree (tree_t* tree) {
    if (tree == NULL) return;
    if (tree->right) removeTree(tree->right);
    if (tree->left)  removeTree(tree->left);

    free(tree); 
}