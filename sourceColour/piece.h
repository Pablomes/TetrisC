#ifndef PIECE_H
#define PIECE_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    H_LINE,
    SQUARE,
    T_BLOCK,
    L_BLOCK,
    RL_BLOCK,
    S_BLOCK,
    RS_BLOCK,
    V_LINE,
} PieceType;

typedef struct {
    int x;
    int y;
} Block;

Block* createBlock(int x, int y);

void destroyBlock(Block* block);

int moveBlock(Block* block, int x, int y);

typedef struct {
    int x;
    int y;
    PieceType type;
    Block** blocks;
} Piece;

Piece* createPiece(int x, int y);

void movePiece(Piece* piece, int x, int y);

void destroyPiece(Piece* piece);

void hidePiece(Piece* piece);

void formPiece(Piece* piece, PieceType type);

#endif