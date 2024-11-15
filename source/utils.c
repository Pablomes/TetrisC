#include "utils.h"

void shuffle(PieceType* array, int start, int end) {
    for (int i = end - 1; i > start; i--) {
        int j = start + rand() % (i + 1 - start);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}