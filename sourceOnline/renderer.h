#ifndef RENDER_H
#define RENDER_H

#include "piece.h"
#include "utils.h"
#include "online.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

extern char* colours[8];

void clearConsole(void);

void setCursorPosition(int x, int y);

void hideCursor();

void showCursor();

void clearBuffer(char* buffer);

void render(Piece* piece, int** board, char* buff, int score, int linesCleared, int level, PieceType* pieces, int pieceIdx, int holdPiece, int height, int width);

void render2Player(Piece* piece1, Piece* piece2, int** board1, int** board2, char* buff, int score1, int score2, int linesCleared1, int linesCleared2, int level1, int level2, PieceType* pieces1, PieceType* pieces2, int pieceIdx1, int pieceIdx2, int holdPiece1, int holdPiece2, int rubbish1, int rubbish2, int height, int width);

void renderOnline(int** board1, int** board2, char* buff, int score1, int score2, int linesCleared1, int linesCleared2, int level1, int level2, int nextPieceNum1, int nextPieceNum2, int holdPiece1, int holdPiece2, int rubbish1, int rubbish2, int height, int width);

GameMode showTitle();

int levelMenu();

int onlineMenu();

char gameOverScreen(int score, int score2, int gamemode, int winner);

#endif