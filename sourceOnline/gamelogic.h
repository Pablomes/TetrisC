#ifndef GAMELOGIC_H
#define GAME_LOGIC_H

#include "utils.h"
#include <conio.h>
#include <string.h>
#include <windows.h>

// PLAYER 1
extern int** board;
extern Piece* piece;
extern char* buff;
extern PieceType* pieces;

extern int level;
extern int spawn;
extern int pieceIdx;
extern int score;
extern int linesCleared;
extern int holdPiece;
extern int changed;
extern int rubbish;

// PLAYER 2

extern int** board2;
extern Piece* piece2;
extern char* buff2;
extern PieceType* pieces2;

extern int level2;
extern int spawn2;
extern int pieceIdx2;
extern int score2;
extern int linesCleared2;
extern int holdPiece2;
extern int changed2;
extern int rubbish2;

// SHARED

extern GameMode gameMode;
extern int winner;
extern int* inputs;
extern int running;
extern double speeds[30];

int initialiseMem(int height, int width);

int initialiseMemPlayer2(int height, int width);

int initialiseGame(int height, int width);

int initialiseGamePlayer2(int height, int width);

int calcScore(int linesCleared);

double calcFallTime(int speed);

void checkInput(int gamemode);

void shufflePieces(PieceType* pieces, int listNum);

int spawnPiece(Piece* piece, int** board, PieceType* pieces, int hold, int height, int width, int player);

void rotatePiece(Piece* piece, int dir, int** board, int height, int width);

int addToBoard(Piece* piece, int** board, int height, int width);

int checkBoard(int** board, int height, int width);

int updatePiece(Piece* piece, int dir, int drop, int** board, int height, int width);

void freeEverything(int height, int gamemode);

int calcRubbish(int linesCleared);

int addRubbish(int** board, int amount, int height, int width);

#endif