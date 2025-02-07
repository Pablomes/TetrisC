#include "gamelogic.h"

// PLAYER 1

int** board = NULL;
Piece* piece = NULL;
char* buff = NULL;
PieceType* pieces = NULL;

int level = -1;
int spawn = 1;
int pieceIdx = 0;
int score = 0;
int linesCleared = 0;
int holdPiece = -1;
int changed = 0;
int rubbish = 0;

// PLAYER 2

int** board2 = NULL;
Piece* piece2 = NULL;
PieceType* pieces2 = NULL;

int level2 = -1;
int spawn2 = 1;
int pieceIdx2 = 0;
int score2 = 0;
int linesCleared2 = 0;
int holdPiece2 = -1;
int changed2 = 0;
int rubbish2 = 0;

// SHARED

int* inputs = NULL;
int winner = 0;
int running = 1;
GameMode gameMode = SINGLE;
double speeds[30] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

int initialiseMem(int height, int width) {

    board = (int**) malloc(sizeof(int*) * height);

    if (board == NULL) {
        printf("ERROR ALLOCATING MEMORY.");
        return -1;
    }

    for (int i = 0; i < height; i++) {
        board[i] = (int*) malloc(sizeof(int) * width);

        if (board[i] == NULL) {
            printf("ERROR ALLOCATING MEMORY.");
            for (int j = 0; j < i; j++) {
                free(board[i]);
            }

            free(board);

            return -1;
        }

        memset(board[i], 0, sizeof(int) * width);
    }

    piece = createPiece(0, 0);

    if (piece == NULL) {
        printf("ERROR ALLOCATING MEMORY.");

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);
        return -1;
    }

    buff = (char*) malloc(sizeof(char) * 7500);

    if (buff == NULL) {
        printf("ERROR ALLOCATING MEMORY.");
        destroyPiece(piece);

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);

        return -1;
    }

    pieces = (PieceType*) malloc(sizeof(PieceType) * 14);

    if (pieces == NULL) {
        printf("ERROR ALLOCATING MEMORY.\n");
        destroyPiece(piece);

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);
        free(buff);

        return -1;
    }

    inputs = (int*) malloc(sizeof(int) * 8);

    if (inputs == NULL) {
        printf("ERROR ALLOCATING MEMORY.\n");
        destroyPiece(piece);

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);
        free(buff);

        free(pieces);

        return -1;
    }

    return 0;

}

int initialiseMemPlayer2(int height, int width) {

    board2 = (int**) malloc(sizeof(int*) * height);

    if (board2 == NULL) {
        printf("ERROR ALLOCATING MEMORY.");

        freeEverything(height, 0);

        return -1;
    }

    for (int i = 0; i < height; i++) {
        board2[i] = (int*) malloc(sizeof(int) * width);

        if (board2[i] == NULL) {
            printf("ERROR ALLOCATING MEMORY.");
            for (int j = 0; j < i; j++) {
                free(board2[i]);
            }

            free(board2);

            freeEverything(height, 0);

            return -1;
        }

        memset(board2[i], 0, sizeof(int) * width);
    }

    piece2 = createPiece(0, 0);

    if (piece2 == NULL) {
        printf("ERROR ALLOCATING MEMORY.");

        for (int i = 0; i < height; i++) {
            free(board2[i]);
        }

        free(board2);

        freeEverything(height, 0);

        return -1;
    }

    pieces2 = (PieceType*) malloc(sizeof(PieceType) * 14);

    if (pieces2 == NULL) {
        printf("ERROR ALLOCATING MEMORY.\n");
        destroyPiece(piece2);

        for (int i = 0; i < height; i++) {
            free(board2[i]);
        }

        free(board2);

        freeEverything(height, 0);

        return -1;
    }

    return 0;

}

int initialiseGame(int height, int width) {
    for (int i = 0; i < 14; i++) {
        pieces[i] = i % 7;
    }

    memset(inputs, 0, sizeof(int) * 8);

    level = -1;
    running = 1;
    winner = 0;
    spawn = 1;
    pieceIdx = 0;
    score = 0;
    linesCleared = 0;
    holdPiece = -1;
    changed = 0;

    shufflePieces(pieces, 1);

    shufflePieces(pieces, 2);

    for (int i = 0; i < height; i++) {
        memset(board[i], 0, sizeof(int) * width);
    }

    return 0;
}

int initialiseGamePlayer2(int height, int width) {
    for (int i = 0; i < 14; i++) {
        pieces2[i] = i % 7;
    }

    level2 = -1;
    spawn2 = 1;
    pieceIdx2 = 0;
    score2 = 0;
    linesCleared2 = 0;
    holdPiece2 = -1;
    changed2 = 0;

    shufflePieces(pieces2, 1);

    shufflePieces(pieces2, 2);

    for (int i = 0; i < height; i++) {
        memset(board2[i], 0, sizeof(int) * width);
    }

    return 0;
}

int calcScore(int linesCleared) {
    switch(linesCleared) {
        case 1:
            return 40 * (level + 1);
        case 2:
            return 100 * (level + 1);
        case 3:
            return 300 * (level + 1);
        case 4:
            return 1200 * (level + 1);
    }

    return 0;
}

double calcFallTime(int speed) {
    return (double)speed / (double)60;
}

void checkInput(int gamemode) {
    memset(inputs, 0, sizeof(int) * 8);

    static int hard1 = 0;
    static int hard2 = 0;

    static int hold1 = 0;
    static int hold2 = 0;

    static int move1 = 0;
    static int move2 = 0;

    static int rot1 = 0;
    static int rot2 = 0;

    if (gamemode == SINGLE || gamemode == ONLINE) {

        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            inputs[0]--;
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            inputs[0]++;
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            inputs[2]++;
        }
        if (GetAsyncKeyState('C') & 0x8000) {
            inputs[2]--;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            inputs[1] = 1;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (hard1 == 0) {
                inputs[1] = 2;
                
            }

            hard1 = 1;
        } else {
            hard1 = 0;
        }
        if (GetAsyncKeyState('X') & 0x8000) {
            if (hold1 == 0) {
                inputs[3] = 1;
                
            }

            hold1 = 1;
        } else {
            hold1 = 0;
        }

        if (inputs[0] > 0) {
            if (move1 == 0) {
                inputs[0] = 2;
            } else {
                inputs[0] = 1;
            }
            move1 = 1;
        } else if (inputs[0] < 0) {
            if (move1 == 0) {
                inputs[0] = -2;
            } else {
                inputs[0] = -1;
            }
            move1 = 1;
        } else {
            move1 = 0;
        }

        if (inputs[2] > 0) {
            if (rot1 == 0) {
                inputs[2] = 2;
            } else {
                inputs[2] = 1;
            }
            rot1 = 1;
        } else if (inputs[2] < 0) {
            if (rot1 == 0) {
                inputs[2] = -2;
            } else {
                inputs[2] = -1;
            }
            rot1 = 1;
        } else {
            rot1 = 0;
        }

    } else {
        // PLAYER 1
        if (GetAsyncKeyState('A') & 0x8000) {
            inputs[0]--;
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            inputs[0]++;
        }
        if (GetAsyncKeyState('C') & 0x8000) {
            inputs[2]++;
        }
        if (GetAsyncKeyState('W') & 0x8000) {
            inputs[2]--;
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            inputs[1] = 1;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (hard1 == 0) {
                inputs[1] = 2;
                
            }

            hard1 = 1;
        } else {
            hard1 = 0;
        }
        if (GetAsyncKeyState('B') & 0x8000) {
            if (hold1 == 0) {
                inputs[3] = 1;
                
            }

            hold1 = 1;
        } else {
            hold1 = 0;
        }

        if (inputs[0] > 0) {
            if (move1 == 0) {
                inputs[0] = 2;
            } else {
                inputs[0] = 1;
            }
            move1 = 1;
        } else if (inputs[0] < 0) {
            if (move1 == 0) {
                inputs[0] = -2;
            } else {
                inputs[0] = -1;
            }
            move1 = 1;
        } else {
            move1 = 0;
        }

        if (inputs[2] > 0) {
            if (rot1 == 0) {
                inputs[2] = 2;
            } else {
                inputs[2] = 1;
            }
            rot1 = 1;
        } else if (inputs[2] < 0) {
            if (rot1 == 0) {
                inputs[2] = -2;
            } else {
                inputs[2] = -1;
            }
            rot1 = 1;
        } else {
            rot1 = 0;
        }

        // PLAYER 2

        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            inputs[4]--;
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            inputs[4]++;
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            inputs[6]++;
        }
        if (GetAsyncKeyState('K') & 0x8000) {
            inputs[6]--;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            inputs[5] = 1;
        }
        if (GetAsyncKeyState('L') & 0x8000) {
            if (hard2 == 0) {
                inputs[5] = 2;
                
            }

            hard2 = 1;
        } else {
            hard2 = 0;
        }
        if (GetAsyncKeyState('J') & 0x8000) {
            if (hold2 == 0) {
                inputs[7] = 1;
                
            }

            hold2 = 1;
        } else {
            hold2 = 0;
        }

        if (inputs[4] > 0) {
            if (move2 == 0) {
                inputs[4] = 2;
            } else {
                inputs[4] = 1;
            }
            move2 = 1;
        } else if (inputs[4] < 0) {
            if (move2 == 0) {
                inputs[4] = -2;
            } else {
                inputs[4] = -1;
            }
            move2 = 1;
        } else {
            move2 = 0;
        }

        if (inputs[6] > 0) {
            if (rot2 == 0) {
                inputs[6] = 2;
            } else {
                inputs[6] = 1;
            }
            rot2 = 1;
        } else if (inputs[6] < 0) {
            if (rot2 == 0) {
                inputs[6] = -2;
            } else {
                inputs[6] = -1;
            }
            rot2 = 1;
        } else {
            rot2 = 0;
        }
    }
}

void shufflePieces(PieceType* pieces, int listNum) {
    int start = 0;
    int end = 0;
    
    switch (listNum) {
        case 1:
            start = 0;
            end = 7;
            break;
        case 2:
            start = 7;
            end = 14;
            break;
        default:
            return;
    }

    for (int i = start; i < end; i++) {
        pieces[i] = i - start;
    }

    shuffle(pieces, start, end);
}

int spawnPiece(Piece* piece, int** board, PieceType* pieces, int hold, int height, int width, int player) {
    //PieceType type = (PieceType) (rand() % 7);

    //printf("Piece type = %d\n", type);

    static int idx1 = 0;
    static int idx2 = 0;
    int idx = 0;

    if (player == 1) { idx = idx1; }
    else if (player == 2) { idx = idx2; }

    PieceType type;

    if (hold == -1) {
        type = pieces[idx];

        idx = (idx + 1) % 14;

        if (idx == 0) {
            shufflePieces(pieces, 2);
        } else if (idx == 7) {
            shufflePieces(pieces, 1);
        }

    } else {
        type = (PieceType) hold;
    }

    formPiece(piece, type);

    //printf("Piece formed.\n");

    switch (type) {
        case H_LINE:
            movePiece(piece, width / 2, 0);
            break;
        case T_BLOCK:
        case L_BLOCK:
        case RL_BLOCK:
        case S_BLOCK:
        case RS_BLOCK:
        case SQUARE:
            if (board[1][width / 2] != 0 || board[1][width / 2 + 1] != 0 || board[1][width / 2 - 1] != 0) {
                movePiece(piece, width / 2, 0);
            } else {
                movePiece(piece, width / 2, 1);
            }
            break;
        default:
            break;
    }

    if (player == 1) { idx1 = idx; }
    else if (player == 2) { idx2 = idx; }

    return idx;

    //printf("Pos = (%d, %d)\n", piece->x, piece->y);
}

void rotatePiece(Piece* piece, int dir, int** board, int height, int width) { // dir = 1 CLOCKWISE. dir = -1 ANTICLOCKWISE
    int xOffset = 0;
    int yOffset = 0;

    switch (piece->type) {
        case H_LINE:
            formPiece(piece, V_LINE);
            for (int i = 0; i < 4; i++) {
                int xPos = piece->blocks[i]->x + piece->x;
                int yPos = piece->blocks[i]->y + piece->y;
                if (xPos >= width) {
                    if (width - xPos + 1 > xOffset) {
                        xOffset = width - xPos + 1;
                    }
                } else if (xPos < 0) {
                    if (xPos < xOffset) {
                        xOffset = xPos;
                    }
                }

                if (yPos >= height) {
                    if (height - yPos + 1 > yOffset) {
                        yOffset = height - yPos + 1;
                    }
                } else if (yPos < 0) {
                    if (yPos < yOffset) {
                        yOffset = yPos;
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                int xPos = piece->blocks[i]->x + piece->x - xOffset;
                int yPos = piece->blocks[i]->y + piece->y - yOffset;

                if (yPos >= height || yPos < 0 || xPos >= width || xPos < 0 || board[yPos][xPos] != 0) {
                    formPiece(piece, H_LINE);
                    return;
                }
            }
            break;
        case V_LINE:
            formPiece(piece, H_LINE);

            for (int i = 0; i < 4; i++) {
                int xPos = piece->blocks[i]->x + piece->x;
                int yPos = piece->blocks[i]->y + piece->y;

                if (xPos >= width) {
                    if (width - xPos + 1 > xOffset) {
                        xOffset = width - xPos + 1;
                    }
                } else if (xPos < 0) {
                    if (xPos < xOffset) {
                        xOffset = xPos;
                    }
                }

                if (yPos >= height) {
                    if (height - yPos + 1 > yOffset) {
                        yOffset = height - yPos + 1;
                    }
                } else if (yPos < 0) {
                    if (yPos < yOffset) {
                        yOffset = yPos;
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                int xPos = piece->blocks[i]->x + piece->x - xOffset;
                int yPos = piece->blocks[i]->y + piece->y - yOffset;

                if (yPos >= height || yPos < 0 || xPos >= width || xPos < 0 || board[yPos][xPos] != 0) {
                    formPiece(piece, V_LINE);
                    return;
                }
            }
            break;
        case SQUARE:
            break;

        case T_BLOCK:
        case L_BLOCK:
        case RL_BLOCK:
        case S_BLOCK:
        case RS_BLOCK:
            for (int i = 0; i < 4; i++) {
                int temp = piece->blocks[i]->x;
                piece->blocks[i]->x = -piece->blocks[i]->y * dir;
                piece->blocks[i]->y = temp * dir;

                int xPos = piece->blocks[i]->x + piece->x;
                int yPos = piece->blocks[i]->y + piece->y;

                if (xPos >= width) {
                    if (width - xPos + 1 > xOffset) {
                        xOffset = width - xPos + 1;
                    }
                } else if (xPos < 0) {
                    if (xPos < xOffset) {
                        xOffset = xPos;
                    }
                }

                if (yPos >= height) {
                    if (height - yPos + 1 > yOffset) {
                        yOffset = height - yPos + 1;
                    }
                } else if (yPos < 0) {
                    if (yPos < yOffset) {
                        yOffset = yPos;
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                int xPos = piece->blocks[i]->x + piece->x - xOffset;
                int yPos = piece->blocks[i]->y + piece->y - yOffset;

                if (yPos >= height || yPos < 0 || xPos >= width || xPos < 0 || board[yPos][xPos] != 0) {
                    for (int j = 0; j < 4; j++) {
                        int temp = piece->blocks[j]->x;
                        piece->blocks[j]->x = piece->blocks[j]->y * dir;
                        piece->blocks[j]->y = -temp * dir;
                    }
                    return;
                }
            }

            break;
    }
    movePiece(piece, piece->x - xOffset, piece->y - yOffset);
}

int addToBoard(Piece* piece, int** board, int height, int width) {
    for (int i = 0; i < 4; i++) {
        int x = piece->blocks[i]->x + piece->x;
        int y = piece->blocks[i]->y + piece->y;

        if (x < width && x >= 0 && y < height && y >= 0) {
            board[y][x] = (int)piece->type + 1;
        }
    }

    return 0;
}

int checkBoard(int** board, int height, int width) {
    int row = height - 1;
    int cleared = 0;
    
    while (row - cleared >= 0) {
        int clear = 1;
        for (int i = 0; i < width; i++) {
            board[row][i] = board[row - cleared][i];
            if (board[row][i] == 0) {
                clear = 0;
            }
        }

        if (clear) {
            cleared++;
        } else {
            row--;
        }
    }

    while (row >= 0) {
        memset(board[row], 0, width * sizeof(int));
        row--;
    }

    for (int i = 0; i < width; i++) {
        if (board[0][i] != 0) {
            return -2;
        }
    }

    return cleared;
}

int updatePiece(Piece* piece, int dir, int drop, int** board, int height, int width) {
    int linesCleared = 0;
    int hardDrop = 0;
    int place = 0;

    //printf("1) newX = %d, newY = %d\n", newX, newY);

    //printf("Start checks.\n");

    if (drop == -1) { 
        dir = 0;
        drop = 1;
        hardDrop = 1;
    }

    do {
        int newY = piece->y + drop;
        int newX = piece->x + dir;

        place = 0;
        int placeStay = 0;
        int valid = 1;

        for (int i = 0; i < 4; i++) {
            int blockX = newX + piece->blocks[i]->x;
            int blockY = newY + piece->blocks[i]->y;
            int blockPrevY = piece->y + piece->blocks[i]->y;
            int blockPrevX = piece->x + piece->blocks[i]->x;

            //printf("(%d, %d)\n", newX + piece->blocks[i]->x, newY + piece->blocks[i]->y);
            if (blockX >= width || blockX < 0 || (blockPrevY >= 0 && board[blockPrevY][blockX] != 0)) {
                valid = 0;
            }
        

        //printf("2) newX = %d, newY = %d\n", newX, newY);

        //printf("Half Checks done.\n");

            if (blockY >= height) {
                place = 1;
                placeStay = 1;
            } else {
                if (blockY >= 0 && board[blockY][blockX] != 0) {
                    place = 1;
                }
                if (blockY >= 0 && board[blockY][blockPrevX] != 0) {
                    placeStay = 1;
                }
            }
        }

        //printf("3) newX = %d, newY = %d\n", newX, newY);

        //printf("Checks done.\n");

        piece->x = valid == 1 ? newX : piece->x;
        piece->y = (valid && place) || (!valid && placeStay) ? piece->y : newY;

        if ((valid && place) || (!valid && placeStay)) {
            linesCleared++;

            for (int i = 0; i < 4; i++) {
                int x = piece->blocks[i]->x + piece->x;
                int y = piece->blocks[i]->y + piece->y;

                if (x < width && x >= 0 && y < height && y >= 0) {
                    board[y][x] = (int)piece->type + 1;
                }
            }

            linesCleared += checkBoard(board, height, width);
        }

    } while (place == 0 && hardDrop == 1);

    return linesCleared;
}

void freeEverything(int height, int gamemode) {
    destroyPiece(piece);
    piece = NULL;

    if (board != NULL) {
        for (int i = 0; i < height; i++) {
            if (board[i] == NULL) { continue; }
            free(board[i]);
            board[i] = NULL;
        }

        free(board);
        board = NULL;
    }
    if (buff != NULL) {
        free(buff);
        buff = NULL;
    }

    if (pieces != NULL) {
        free(pieces);
        pieces = NULL;
    }

    if (inputs != NULL) {
        free(inputs);
        inputs = NULL;
    }

    if (gamemode == 0) { return; }

    destroyPiece(piece2);
    piece2 = NULL;

    if (pieces2 != NULL) {
        free(pieces2);
        pieces2 = NULL;
    }

    if (board2 == NULL) { return; }

    for (int i = 0; i < height; i++) {
        if (board2[i] == NULL) { continue; }
        free(board2[i]);
        board2[i] = NULL;
    }

    free(board2);
    board2 = NULL;
}

int calcRubbish(int lines) {
    if (lines == 4) {
        return 4;
    }

    if (lines <= 0) {
        return 0;
    }

    return lines - 1;
}

int addRubbish(int** board, int amount, int height, int width) {
    
    for (int i = 0; i < height - amount; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j] = board[i + amount][j];
        }
    }

    for (int i = height - amount; i < height; i++) {
        int k = rand() % width;

        for (int j = 0; j < width; j++) {
            if (j != k) {
                board[i][j] = -1;
            } else {
                board[i][j] = 0;
            }
        }
    }

    return 0;
}