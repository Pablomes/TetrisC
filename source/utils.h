#ifndef UTILS_H
#define UTILS_H

#include "piece.h"
#include <stdlib.h>
#include <stdio.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))
#define clamp(n, bot, top) min(top, min(n, bot))

void shuffle(PieceType* array, int start, int end);

#endif //BASICS_H