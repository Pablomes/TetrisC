#ifndef GAMELOGIC_H
#define GAMELOGIC_H // me cago en ti

#include "utils.h"
#include <conio.h>
#include <string.h>
#include <windows.h>

// PLAYER 1
typedef struct {
    int* board;
    Piece* piece;
    char* buff;
    PieceType* pieces;
    int level;
    int spawn;
    int pieceIdx;
    int score;
    int linesCleared;
    int holdPiece;
    int changed;
    int rubbish;
} Player;

// SHARED

typedef struct {
    int twoPlayer;
    int winner;
    int *inputs;
    int running;
    double speeds[30];

    Player players[2];
} GameData;

int initialiseMem(int height, int width, int bufSize);

int initialiseMemPlayer2(int height, int width);

int initialiseGame(int height, int width);

int initialiseGamePlayer2(int height, int width);

int calcScore(int linesCleared);

double calcFallTime(int speed);

void checkInput(int gamemode);

void shufflePieces(PieceType* pieces, int listNum);

int spawnPiece(Piece* piece, int* board, PieceType* pieces, int hold, int height, int width, int player);

void rotatePiece(Piece* piece, int dir, int* board, int height, int width);

int checkBoard(int* board, int height, int width);

int updatePiece(Piece* piece, int dir, int drop, int* board, int height, int width, int val);

void freeEverything(int gamemode);

int calcRubbish(int linesCleared);

int addRubbish(int* board, int amount, int height, int width, int rubbishVal);

#endif