#include "piece.h"

Block* createBlock(int x, int y) {
    Block* res = (Block*) malloc(sizeof(Block));

    if (res == NULL) {
        return NULL;
    }

    res->x = x;
    res->y = y;

    return res;
}

void destroyBlock(Block* block) {
    free(block);
}

int moveBlock(Block* block, int x, int y) {
    block->x = x;
    block->y = y;

    return 0;
}

Piece* createPiece(int x, int y) {
    Piece* res = (Piece*) malloc(sizeof(Piece));
    if (res == NULL) { return NULL; }
    
    res->blocks = (Block**) malloc(sizeof(Block*) * 4);

    if (res->blocks == NULL) {
        free(res);
        return NULL;
    }

    for (int i = 0; i < 4; i++) {
        res->blocks[i] = createBlock(0, 0);

        if (res->blocks[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(res->blocks[j]);
            }

            free(res->blocks);
            free(res);
            return NULL;
        }
    }

    res->x = x;
    res->y = y;
    res->type = 0;

    return res;
}

void movePiece(Piece* piece, int x, int y) {
    piece->x = x;
    piece->y = y;
}

void destroyPiece(Piece* piece) {
    if (piece == NULL) { return; }
    if (piece->blocks == NULL) { return; }

    for (int i = 0; i < 4; i++) {
        if (piece->blocks[i] == NULL) { continue; }
        destroyBlock(piece->blocks[i]);
        piece->blocks[i] = NULL;
    }

    free(piece->blocks);
    piece->blocks = NULL;
    free(piece);
}

void hidePiece(Piece* piece) {
    piece->x = -5;
    piece->y = -5;
}

void formPiece(Piece* piece, PieceType type) {
    piece->type = type;

    switch (type) {
        case H_LINE:
            moveBlock(piece->blocks[0], -2, 0);
            moveBlock(piece->blocks[1], -1, 0);
            moveBlock(piece->blocks[2], 0, 0);
            moveBlock(piece->blocks[3], 1, 0);
            break;
        case V_LINE:
            moveBlock(piece->blocks[0], 0, -2);
            moveBlock(piece->blocks[1], 0, -1);
            moveBlock(piece->blocks[2], 0, 0);
            moveBlock(piece->blocks[3], 0, 1);
            break;
        case SQUARE:
            moveBlock(piece->blocks[0], 0, 0);
            moveBlock(piece->blocks[1], -1, 0);
            moveBlock(piece->blocks[2], 0, -1);
            moveBlock(piece->blocks[3], -1, -1);
            break;
        case T_BLOCK:
            moveBlock(piece->blocks[0], -1, 0);
            moveBlock(piece->blocks[1], 0, 0);
            moveBlock(piece->blocks[2], 1, 0);
            moveBlock(piece->blocks[3], 0, -1);
            break;
        case L_BLOCK:
            moveBlock(piece->blocks[0], -1, 0);
            moveBlock(piece->blocks[1], 0, 0);
            moveBlock(piece->blocks[2], 1, 0);
            moveBlock(piece->blocks[3], 1, -1);
            break;
        case RL_BLOCK:
            moveBlock(piece->blocks[0], -1, 0);
            moveBlock(piece->blocks[1], 0, 0);
            moveBlock(piece->blocks[2], 1, 0);
            moveBlock(piece->blocks[3], -1, -1);
            break;
        case S_BLOCK:
            moveBlock(piece->blocks[0], -1, 0);
            moveBlock(piece->blocks[1], 0, 0);
            moveBlock(piece->blocks[2], 0, -1);
            moveBlock(piece->blocks[3], 1, -1);
            break;
        case RS_BLOCK:
            moveBlock(piece->blocks[0], -1, -1);
            moveBlock(piece->blocks[1], 0, -1);
            moveBlock(piece->blocks[2], 0, 0);
            moveBlock(piece->blocks[3], 1, 0);
            break;
    }
}