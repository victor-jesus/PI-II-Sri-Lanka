#include "dice.h"
#include <time.h>
#include <stdlib.h>

int roll(Dice dice){
    switch (dice) {
    case D_20:
        return rand() % 20 + 1;
        break;
    case D_8:
        return rand() % 8 + 1;
        break;
    case D_4:
        return rand() % 4 + 1;
        break;
    }
}