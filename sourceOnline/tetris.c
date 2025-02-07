#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "utils.h"
#include "piece.h"
#include "renderer.h"
#include "gamelogic.h"
#include "settings.h"
#include "signalhandler.h"
#include "online.h"

int main(void) {

    setupSignalHandler();
    
    showCursor();

    srand((unsigned)time(NULL));

mainMenu:

    int memRes = initialiseMem(height, width);

    if (memRes != 0) {
        return 0;
    }

    gameMode = showTitle();

    if (gameMode == TWOPLAYER || gameMode == ONLINE) {
        memRes = initialiseMemPlayer2(height, width);
    }

    if (memRes != 0) {
        return 0;
    }

startGame:

    initialiseGame(height, width);
    if (gameMode == TWOPLAYER || gameMode == ONLINE) {
        initialiseGamePlayer2(height, width);
    }

    if (gameMode == TWOPLAYER || gameMode == ONLINE) {
        level = 0;
        level2 = 0;
        clearConsole();
    } else {
        level = levelMenu();
    }

    int admin = 0;

    if (gameMode == ONLINE) {
        admin = onlineMenu();

        if (admin) {
            ConnectAsServer();
        } else {
            ConnectAsClient();
        }

        clearConsole();
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

    int send = 0;

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

        if (linesCleared > (level + 1) * 10) {
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

        checkInput(gameMode);

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
                rotatePiece(piece, -1, board, height, width); 

                lastRot = current;
            }
        } else if (inputs[2] > 0 && spawn == 0) {
            if (inputs[2] == 2 || (inputs[2] == 1 && sinceRot >= rotateTime)) {
                rotatePiece(piece, 1, board, height, width); 

                lastRot = current;
            }
        }

        if (inputs[3] == 1 && changed == 0 && spawn == 0) {

            if (holdPiece != -1) {
                int temp = piece->type;
                pieceIdx = spawnPiece(piece, board, pieces, holdPiece, height, width, 1);
                holdPiece = temp;
                if ((PieceType)holdPiece == V_LINE) {
                    holdPiece = (int) H_LINE;
                }
            } else {
                holdPiece = piece->type;
                if ((PieceType)holdPiece == V_LINE) {
                    holdPiece = (int) H_LINE;
                }
                pieceIdx = spawnPiece(piece, board, pieces, -1, height, width, 1);
                drop = 0;
                lastDrop = current;
            }
            changed = 1;
        }

        //MANAGE PLAYER 2 INPUTS

        if (gameMode == TWOPLAYER) {
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
                    rotatePiece(piece2, -1, board2, height, width); 

                    lastRot2 = current;
                }
            } else if (inputs[6] > 0 && spawn2 == 0) {
                if (inputs[6] == 2 || (inputs[6] == 1 && sinceRot2 >= rotateTime)) {
                    rotatePiece(piece2, 1, board2, height, width); 

                    lastRot2 = current;
                }
            }

            if (inputs[7] == 1 && changed2 == 0 && spawn2 == 0) {

                if (holdPiece2 != -1) {
                    int temp = piece2->type;
                    pieceIdx2 = spawnPiece(piece2, board2, pieces2, holdPiece2, height, width, 2);
                    holdPiece2 = temp;
                    if ((PieceType)holdPiece2 == V_LINE) {
                        holdPiece2 = (int) H_LINE;
                    }
                } else {
                    holdPiece2 = piece2->type;
                    if ((PieceType)holdPiece2 == V_LINE) {
                        holdPiece2 = (int) H_LINE;
                    }
                    pieceIdx2 = spawnPiece(piece2, board2, pieces2, -1, height, width, 2);
                    drop2 = 0;
                    lastDrop2 = current;
                }
                changed2 = 1;
            }
        }

        // UPDATE PLAYER 1

        if (spawn == 1) {
            if (rubbish > 0) {
                addRubbish(board, rubbish, height, width);
                rubbish = 0;
            }

            pieceIdx = spawnPiece(piece, board, pieces, -1, height, width, 1);
            spawn = 0;
        } else {
            int res;

            if (dir != 0) {
                lastMove = current;
            }

            if (drop == 1 || drop == -1) {
                res = updatePiece(piece, dir, drop, board, height, width);
                drop = 0;
            } else {
                res = updatePiece(piece, dir, 0, board, height, width);
            }

            if (res < 0) {
                running = 0;
                winner = 2;
            } else if (res > 0) {
                // ADD LINES
                changed = 0;
                linesCleared += (res - 1);
                int rub = calcRubbish(res - 1);
                send += rub;
                rubbish2 += rub;
                score += calcScore(res - 1);

                hidePiece(piece);
                spawn = 1;
            }
        }

        // UPDATE PLAYER 2

        if (spawn2 == 1 && gameMode == TWOPLAYER) {
            if (rubbish2 > 0) {
                addRubbish(board2, rubbish2, height, width);
                rubbish2 = 0;
            }

            pieceIdx2 = spawnPiece(piece2, board2, pieces2, -1, height, width, 2);
            spawn2 = 0;
        } else if (gameMode == TWOPLAYER) {
            int res;

            if (dir2 != 0) {
                lastMove2 = current;
            }

            if (drop2 == 1 || drop2 == -1) {
                res = updatePiece(piece2, dir2, drop2, board2, height, width);
                drop = 0;
            } else {
                res = updatePiece(piece2, dir2, 0, board2, height, width);
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

            if (gameMode == ONLINE) {

                for (int i = 0; i < 4; i++) {
                    int x = piece->blocks[i]->x + piece->x;
                    int y = piece->blocks[i]->y + piece->y;

                    if (x >= 0 && x < width && y >= 0 && y < height) {
                        board[y][x] = piece->type + 1;
                    }
                }

                int otherNextPiece = 0;
                int otherRunning = 1;
                
                if (admin) {
                    int temp = 0;
                    SendGameState(board, score, linesCleared, level, pieces[pieceIdx], holdPiece, rubbish, send, running, admin, height, width);
                    send = 0;

                    otherRunning = ReceiveGameState(board2, &score2, &linesCleared2, &level2, &otherNextPiece, &holdPiece2, &rubbish2, &temp, admin, height, width);
                    rubbish += temp;
                } else {
                    int temp = 0;
                    otherRunning = ReceiveGameState(board2, &score2, &linesCleared2, &level2, &otherNextPiece, &holdPiece2, &rubbish2, &temp, admin, height, width);
                    rubbish += temp;

                    SendGameState(board, score, linesCleared, level, pieces[pieceIdx], holdPiece, rubbish, send, running, admin, height, width);
                    send = 0;
                }

                renderOnline(board, board2, buff, score, score2, linesCleared, linesCleared2, level, level2, pieces[pieceIdx], otherNextPiece, holdPiece, holdPiece2, rubbish, rubbish2, height, width);

                for (int i = 0; i < 4; i++) {
                    int x = piece->blocks[i]->x + piece->x;
                    int y = piece->blocks[i]->y + piece->y;

                    if (x >= 0 && x < width && y >= 0 && y < height) {
                        board[y][x] = 0;
                    }
                }

                if (otherRunning == 0  || running == 0) { break; }

            } else if (gameMode == TWOPLAYER) {
                render2Player(piece, piece2, board, board2, buff, score, score2, linesCleared, linesCleared2, level, level2, pieces, pieces2, pieceIdx, pieceIdx2, holdPiece, holdPiece2, rubbish, rubbish2, height, width);
            } else {
                render(piece, board, buff, score, linesCleared, level, pieces, pieceIdx, holdPiece, height, width);
            }

            lastUpdate = current;
        }

    }

    if (gameMode == ONLINE) {
        if (running) { winner = 1; }
        else { winner = 2; }
    }

    char c = gameOverScreen(score, score2, gameMode, winner);

    if (c == '\r' && gameMode != ONLINE) {
        goto startGame;
    }

    freeEverything(height, gameMode);
    if (gameMode == ONLINE) {
        if (admin) {
            CleanServer();
        } else {
            CleanClient();
        }
    }

    if (c == 'm' || (c == '\r' && gameMode == ONLINE)) {
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