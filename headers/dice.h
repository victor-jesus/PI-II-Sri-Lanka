#ifndef DICE_H
#define DICE_H

typedef enum{
    D_20,
    D_10,
    D_8,
    D_6,
    D_4
} Dice;

int roll(Dice dice);

#endif