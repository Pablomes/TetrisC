#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "../settings.h"

#define RENDERER "blacknwhite/renderer.h"

#include "../utils.c"
#include "../piece.c"
#include "renderer.c"
#include "../gamelogic.c"
#include "../signalhandler.c"

int main(void) {
    setupSignalHandler();
    
    showCursor();

    srand((unsigned)time(NULL));

mainMenu:

    int memRes = initialiseMem(gameHeight, gameWidth, 5000);

    if (memRes != 0) {
        return 0;
    }

    twoPlayer = showTitle();

    if (twoPlayer) {
        memRes = initialiseMemPlayer2(gameHeight, gameWidth);
    }

    if (memRes != 0) {
        return 0;
    }

startGame:

    initialiseGame(gameHeight, gameWidth);
    if (twoPlayer) {
        initialiseGamePlayer2(gameHeight, gameWidth);
    }

    if (twoPlayer) {
        level = 0;
        level2 = 0;
        clearConsole();
    } else {
        level = levelMenu();
    }

    clock_t lastUpdate = clock();
    clock_t lastDrop = lastUpdate;
    clock_t lastDrop2 = lastUpdate;
    clock_t lastMove = lastUpdate;
    clock_t lastMove2 = lastUpdate;
    clock_t lastRot = lastUpdate;
    clock_t lastRot2 = lastUpdate;
    int useDropMult = 0;
    int useDropMult2 = 0;

    while (running) {
        clock_t current = clock();
        double elapsed = (double)(current - lastUpdate) / CLOCKS_PER_SEC;
        double sinceDrop = (double)(current - lastDrop) / CLOCKS_PER_SEC;
        double sinceDrop2 = (double)(current - lastDrop2) / CLOCKS_PER_SEC;
        double sinceMove = (double)(current - lastMove) / CLOCKS_PER_SEC;
        double sinceMove2 = (double)(current - lastMove2) / CLOCKS_PER_SEC;
        double sinceRot = (double)(current - lastRot) / CLOCKS_PER_SEC;
        double sinceRot2 = (double)(current - lastRot2) / CLOCKS_PER_SEC;

        int drop = 0;
        int drop2 = 0;
        int dir = 0;
        int dir2 = 0;

        if (glob_linesCleared > (level + 1) * 10) {
            level++;
        }

        if (linesCleared2 > (level2 + 1) * 10) {
            level2++;
        }

        int tempLevel = level > 29 ? 29 : level;

        timeForDrop = (double) speeds[tempLevel] / (double) 60;

        tempLevel = level2 > 29 ? 29 : level2;

        timeForDrop2 = (double) speeds[tempLevel] / (double) 60;

        //timeForDrop = (double)1 / (double)60;

        if (sinceDrop >= timeForDrop * (useDropMult ? softDropMult : 1)) {
            drop = 1;
            lastDrop = current;
        }

        if (sinceDrop2 >= timeForDrop2 * (useDropMult2 ? softDropMult : 1)) {
            drop2 = 1;
            lastDrop2 = current;
        }

        checkInput(twoPlayer);

        // MANAGE PLAYER 1 INPUTS

        if (inputs[0] > 0) {
            if (inputs[0] == 2 || (inputs[0] == 1 && sinceMove >= moveTime)) {
                dir = 1;
            } else {
                dir = 0;
            }
        } else if (inputs[0] < 0) { 
            if (inputs[0] == -2 || (inputs[0] == -1 && sinceMove >= moveTime)) {
                dir = -1;
            } else {
                dir = 0;
            }
        } else { dir = 0; }

        if (inputs[1] == 2) { drop = -1; useDropMult = 0; }
        else if (inputs[1] == 1) { useDropMult = 1; }
        else { useDropMult = 0; }

        if (inputs[2] < 0 && spawn == 0) { 
            if (inputs[2] == -2 || (inputs[2] == -1 && sinceRot >= rotateTime)) {
                rotatePiece(glob_piece, -1, glob_board, gameHeight, gameWidth); 

                lastRot = current;
            }
        } else if (inputs[2] > 0 && spawn == 0) {
            if (inputs[2] == 2 || (inputs[2] == 1 && sinceRot >= rotateTime)) {
                rotatePiece(glob_piece, 1, glob_board, gameHeight, gameWidth); 

                lastRot = current;
            }
        }

        if (inputs[3] == 1 && changed == 0 && spawn == 0) {

            if (holdPiece != -1) {
                int temp = glob_piece->type;
                pieceIdx = spawnPiece(glob_piece, glob_board, glob_pieces, holdPiece, gameHeight, gameWidth, 1);
                holdPiece = temp;
                if ((PieceType)holdPiece == V_LINE) {
                    holdPiece = (int) H_LINE;
                }
            } else {
                holdPiece = glob_piece->type;
                if ((PieceType)holdPiece == V_LINE) {
                    holdPiece = (int) H_LINE;
                }
                pieceIdx = spawnPiece(glob_piece, glob_board, glob_pieces, -1, gameHeight, gameWidth, 1);
                drop = 0;
                lastDrop = current;
            }
            changed = 1;
        }

        //MANAGE PLAYER 2 INPUTS

        if (twoPlayer) {
            if (inputs[4] > 0) {
                if (inputs[4] == 2 || (inputs[4] == 1 && sinceMove2 >= moveTime)) {
                    dir2 = 1;
                } else {
                    dir2 = 0;
                }
            } else if (inputs[4] < 0) { 
                if (inputs[4] == -2 || (inputs[4] == -1 && sinceMove2 >= moveTime)) {
                    dir2 = -1;
                } else {
                    dir2 = 0;
                }
            } else { dir2 = 0; }

            if (inputs[5] == 2) { drop2 = -1; useDropMult2 = 0; }
            else if (inputs[5] == 1) { useDropMult2 = 1; }
            else { useDropMult2 = 0; }

            if (inputs[6] < 0 && spawn2 == 0) { 
                if (inputs[6] == -2 || (inputs[6] == -1 && sinceRot2 >= rotateTime)) {
                    rotatePiece(piece2, -1, board2, gameHeight, gameWidth); 

                    lastRot2 = current;
                }
            } else if (inputs[6] > 0 && spawn2 == 0) {
                if (inputs[6] == 2 || (inputs[6] == 1 && sinceRot2 >= rotateTime)) {
                    rotatePiece(piece2, 1, board2, gameHeight, gameWidth); 

                    lastRot2 = current;
                }
            }

            if (inputs[7] == 1 && changed2 == 0 && spawn2 == 0) {

                if (holdPiece2 != -1) {
                    int temp = piece2->type;
                    pieceIdx2 = spawnPiece(piece2, board2, pieces2, holdPiece2, gameHeight, gameWidth, 2);
                    holdPiece2 = temp;
                    if ((PieceType)holdPiece2 == V_LINE) {
                        holdPiece2 = (int) H_LINE;
                    }
                } else {
                    holdPiece2 = piece2->type;
                    if ((PieceType)holdPiece2 == V_LINE) {
                        holdPiece2 = (int) H_LINE;
                    }
                    pieceIdx2 = spawnPiece(piece2, board2, pieces2, -1, gameHeight, gameWidth, 2);
                    drop2 = 0;
                    lastDrop2 = current;
                }
                changed2 = 1;
            }
        }

        // UPDATE PLAYER 1

        if (spawn == 1) {
            if (rubbish > 0) {
                addRubbish(glob_board, rubbish, gameHeight, gameWidth, 1);
                rubbish = 0;
            }

            pieceIdx = spawnPiece(glob_piece, glob_board, glob_pieces, -1, gameHeight, gameWidth, 1);
            spawn = 0;
        } else {
            int res;

            if (dir != 0) {
                lastMove = current;
            }

            if (drop == 1 || drop == -1) {
                res = updatePiece(glob_piece, dir, drop, glob_board, gameHeight, gameWidth, 1);
                drop = 0;
            } else {
                res = updatePiece(glob_piece, dir, 0, glob_board, gameHeight, gameWidth, 1);
            }

            if (res < 0) {
                running = 0;
                winner = 2;
            } else if (res > 0) {
                // ADD LINES
                changed = 0;
                glob_linesCleared += (res - 1);
                rubbish2 += calcRubbish(res - 1);
                score += calcScore(res - 1);

                hidePiece(glob_piece);
                spawn = 1;
            }
        }

        // UPDATE PLAYER 2

        if (spawn2 == 1 && twoPlayer) {
            if (rubbish2 > 0) {
                addRubbish(board2, rubbish2, gameHeight, gameWidth, 1);
                rubbish2 = 0;
            }

            pieceIdx2 = spawnPiece(piece2, board2, pieces2, -1, gameHeight, gameWidth, 2);
            spawn2 = 0;
        } else if (twoPlayer) {
            int res;

            if (dir2 != 0) {
                lastMove2 = current;
            }

            if (drop2 == 1 || drop2 == -1) {
                res = updatePiece(piece2, dir2, drop2, board2, gameHeight, gameWidth, 1);
                drop = 0;
            } else {
                res = updatePiece(piece2, dir2, 0, board2, gameHeight, gameWidth, 1);
            }

            if (res < 0) {
                running = 0;
                winner = 1;
            } else if (res > 0) {
                // ADD LINES
                changed2 = 0;
                linesCleared2 += (res - 1);
                rubbish += calcRubbish(res - 1);
                score2 += calcScore(res - 1);

                hidePiece(piece2);
                spawn2 = 1;
            }
        }

        if (elapsed >= updateTime) {

            if (twoPlayer) {
                render2Player(glob_piece, piece2, glob_board, board2, buff, score, score2, glob_linesCleared, linesCleared2, level, level2, glob_pieces, pieces2, pieceIdx, pieceIdx2, holdPiece, holdPiece2, rubbish, rubbish2, gameHeight, gameWidth);
            } else {
                render(glob_piece, glob_board, buff, score, glob_linesCleared, level, glob_pieces, pieceIdx, holdPiece, gameHeight, gameWidth);
            }

            lastUpdate = current;
        }

    }

    char c = gameOverScreen(score, score2, twoPlayer, winner);

    if (c == '\r') {
        goto startGame;
    }

    // do you want to reallocate everything at game reset?
    // you'll need a different function here that resets the values of everything
    freeEverything(twoPlayer);

    if (c == 'm') {
        goto mainMenu;
    }

    return 0;
}

/*
_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| 
    _|      _|            _|      _|    _|    _|    _|       
    _|      _|_|_|        _|      _|_|_|      _|      _|_|   
    _|      _|            _|      _|    _|    _|          _| 
    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   
*/