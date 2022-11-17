#include "solvetree.h"
#include "cvoice.h"
#include "cstack.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>

const char defaultfile[] = "data.akt";

void LOOP (bool* working, tree_t** tree);

int main () {
    tree_t* base = NULL;
    setlocale(LC_ALL, "");

    bool working = true;
    while (working) LOOP(&working, &base);

    return 0;
}

void LOOP (bool* working, tree_t** tree) {
    SAY(  "%s\n", "Введите команду:");
    SAY("\t%s\n", "(l) - загрузить последнюю версию Акинатора с диска");
    SAY("\t%s\n", "(L) - загрузить версию Акинатора из фаила");
    SAY("\t%s\n", "(s) - сохранить текущую версию Акинатора на диск");
    SAY("\t%s\n", "(S) - сохранить текущую версию Акинатора в фаил");
    SAY("\t%s\n", "(g) - включить режим угадывания");
    SAY("\t%s\n", "(d) - включить режим определения");
    SAY("\t%s\n", "(c) - включить режим сравнения");
    SAY("\t%s\n", "(D) - вывести отладочную информацию");
    SAY("\t%s\n", "(q) - сохранить и выйти");
    SAY("\t%s\n", "(Q) - сохранить в указанный фаил и выйти");
    SAY("\t%s\n", "(E) - выйти, без сохранения");
    
    int com = '\0';
    char filename[STR_LEN] = ""; // const
    char bufname1[STR_LEN] = "";
    char bufname2[STR_LEN] = "";
    char* bufn = NULL;
    bool process = true;
    static Stack* defenition1 = NULL;
    static Stack* defenition2 = NULL;

    while (process) {
        switch (com = getchar()) {
            case EOF:
                SAY("\t%s\n", "Неверная команда");
                return;
            case (int)'l':                // make 'L' and other
                removeTree(*tree);
                *tree = loadData(defaultfile);
                if (*tree == NULL) {
                    SAY("%s %s\n", "Ошибка при чтении фаила", defaultfile);
                    return;
                }
                SAY("%s %s\n", "Фаил считан успешно", defaultfile);
                return;
            case (int)'L':
                removeTree(*tree);
                SAY("%s: ", "Введите название фаила сохранения");
                scanf("%s", filename);

                *tree = loadData(filename);
                if (*tree == NULL) {
                    SAY("%s %s\n", "Ошибка при чтении фаила", filename);
                    return;
                }
                SAY("%s %s\n", "Фаил считан успешно", filename);
                return;
            case (int)'d':
                while (getchar() != '\n');
                SAY("%s: ", "Введите название, для определения");
                fgets(bufname1, STR_LEN, stdin);
                if ((bufn = strpbrk(bufname1, "\n\r"))) *bufn = '\0';
                defenition1 = findElem(*tree, bufname1);

                if (!defenition1) { 
                    SAY("\t%s ", bufname1);
                    SAY("%s\n", "отсутствует"); 
                } else {
                    defElem(*tree, defenition1, bufname1);
                }
                return;
            case (int)'c':
                while (getchar() != '\n');
                SAY("%s: ", "Введите первое название, для сравнения");
                fgets(bufname1, STR_LEN, stdin);
                if ((bufn = strpbrk(bufname1, "\n\r"))) *bufn = '\0';
                defenition1 = findElem(*tree, bufname1);

                SAY("%s: ", "Введите второе название, для сравнения");
                fgets(bufname2, STR_LEN, stdin);
                if ((bufn = strpbrk(bufname2, "\n\r"))) *bufn = '\0';
                defenition2 = findElem(*tree, bufname2);

                if (!defenition1) { 
                    SAY("\t%s ", bufname1);
                    SAY("%s\n", "отсутствует"); 
                } else if (!defenition2) { 
                    SAY("\t%s ", bufname2);
                    SAY("%s\n", "отсутствует"); 
                } else {
                    cmpElem(*tree, defenition1, defenition2, bufname1, bufname2);
                }

            case (int)'s':
                saveData(defaultfile, *tree);
                SAY("%s\n", "успешно сохранено");
                return;
            case (int)'S':
                SAY("%s: ", "Введите название фаила сохранения");
                scanf("%s", filename);
                saveData(filename, *tree);
                SAY("%s ", "успешно сохранено в фаил");
                SAY("%s\n", filename);
                return;
            case (int)'g':
                guesElem(*tree);
                return;
            case (int)'D':
                treeDump(*tree);
                return;
            case (int)'E':
                *working = false;
                return;
            case (int)'q':
                saveData(defaultfile, *tree);
                *working = false;
                return;
            case (int)'Q':
                SAY("%s: ", "Введите название фаила сохранения");
                scanf("%s", filename);
                saveData(filename, *tree);
                *working = false;
                return;
            case (int)' ':
                break;
            case (int)'\n':
                break;
            default:
                SAY("\t%s\n", "Неверная команда");
                while (getchar() != '\n');               
                break;
        }   
    }
}