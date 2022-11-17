#include "cvoice.h"

#include "stdbool.h"

bool voicesolo = true;

void voiceSay (const char* phrase) {
    if (voicesolo == false) {
        return;
    }
}

void voiceStart () {

}