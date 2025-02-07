#ifndef UTILS_H
#define UTILS_H

#include "piece.h"
#include <stdlib.h>
#include <stdio.h>

int clamp(int n, int min, int max);

void shuffle(PieceType* array, int start, int end);

enum PlayMode {
    SINGLE,
    TWOPLAYER,
    ONLINE
} typedef GameMode;

#endif //BASICS_H