#include "solvetree.h"
#include "cvoice.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>

const char defaultfile[] = "data.akt";

void LOOP (bool* working, tree_t** tree);

int main () {
    tree_t* base = loadData("data.akt");
    setlocale(LC_ALL, "");

    bool working = true;
    while (working) LOOP(&working, &base);

    treeDump(base);

    return 0;
}

void LOOP (bool* working, tree_t** tree) {
    SAY(  "%s\n", "Введите команду:");
    SAY("\t%s\n", "(l) - загрузить последнюю версию Акинатора с диска");
    SAY("\t%s\n", "(s) - сохранить текущую версию Акинатора на диск");
    SAY("\t%s\n", "(g) - включить режим угадывания");
    SAY("\t%s\n", "(d) - включить режим определения");
    SAY("\t%s\n", "(c) - включить режим сравнения");
    SAY("\t%s\n", "(D) - вывести отладочную информацию");
    SAY("\t%s\n", "(q) - сохранить и выйти");
    SAY("\t%s\n", "(E) - выйти, без сохранения");
    
    int com = '\0';
    char filename[100] = ""; // const
    bool process = true;
    while (process) {
        switch (com = getchar()) {
            case EOF:
                SAY("\t%s\n", "Неверная команда");
                return;
            case (int)'l':                // make 'L' and other
                *tree = loadData(defaultfile);
                if (*tree == NULL) {
                    SAY("%s %s\n", "Ошибка при чтении фаила", defaultfile);
                    return;
                }

                SAY("%s %s\n", "Фаил считан успешно", defaultfile);

                return;


            case (int)'s':
                saveData(defaultfile, *tree);
                SAY("%s\n", "успешно сохранено");
                return;
            case (int)'g':
                findElem(*tree);
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