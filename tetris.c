#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <windows.h>

int clamp(int n, int min, int max) {
    if (n < min) {
        n = min;
    } else if (n > max) {
        n = max;
    }

    return n;
}

void clearConsole(void) {
    system("cls");
}

// Function to set the cursor position
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to hide the cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Function to show the cursor
void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = TRUE; // Set cursor visibility to TRUE
    cursorInfo.dwSize = 1; // Set cursor size (1 is the minimum size)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

struct {
    int x;
    int y;
} typedef Block;

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

struct {
    int x;
    int y;
    PieceType type;
    Block** blocks;
} typedef Piece;

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
}

void movePiece(Piece* piece, int x, int y) {
    piece->x = x;
    piece->y = y;
}

void destroyPiece(Piece* piece) {
    for (int i = 0; i < 4; i++) {
        destroyBlock(piece->blocks[i]);
    }

    free(piece->blocks);
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

void shuffle(PieceType* array, int start, int end) {
    for (int i = end - 1; i > start; i--) {
        int j = start + rand() % (i + 1 - start);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
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

int spawnPiece(Piece* piece, int** board, PieceType* pieces, int hold, int height, int width) {
    //PieceType type = (PieceType) (rand() % 7);

    //printf("Piece type = %d\n", type);

    static int idx = 0;
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
            if (board[1][width / 2] == 1 || board[1][width / 2 + 1] == 1 || board[1][width / 2 - 1] == 1) {
                movePiece(piece, width / 2, 0);
            } else {
                movePiece(piece, width / 2, 1);
            }
            break;
    }

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

                if (yPos >= height || yPos < 0 || xPos >= width || xPos < 0 || board[yPos][xPos] == 1) {
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

                if (piece->blocks[i]->y >= height) {
                    if (height - piece->blocks[i]->y + 1 > yOffset) {
                        yOffset = height - piece->blocks[i]->y + 1;
                    }
                } else if (piece->blocks[i]->y < 0) {
                    if (-piece->blocks[i]->y < yOffset) {
                        yOffset = -piece->blocks[i]->y;
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                int xPos = piece->blocks[i]->x + piece->x - xOffset;
                int yPos = piece->blocks[i]->y + piece->y - yOffset;

                if (yPos >= height || yPos < 0 || xPos >= width || xPos < 0 || board[yPos][xPos] == 1) {
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

                if (yPos >= height || yPos < 0 || xPos >= width || xPos < 0 || board[yPos][xPos] == 1) {
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
            board[y][x] = 1;
        }
    }
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

    for (int i = 0; i < width; i++) {
        if (board[0][i] == 1) {
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
            if (blockX >= width || blockX < 0 || (blockPrevY >= 0 && board[blockPrevY][blockX] == 1)) {
                valid = 0;
            }
        

        //printf("2) newX = %d, newY = %d\n", newX, newY);

        //printf("Half Checks done.\n");

            if (blockY >= height) {
                place = 1;
                placeStay = 1;
            } else {
                if (blockY >= 0 && board[blockY][blockX] == 1) {
                    place = 1;
                }
                if (blockY >= 0 && board[blockY][blockPrevX] == 1) {
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
                    board[y][x] = 1;
                }
            }

            linesCleared += checkBoard(board, height, width);
        }

    } while (place == 0 && hardDrop == 1);

    return linesCleared;
}

void clearBuffer(char* buffer) {
    memset(buffer, ' ', sizeof(char) * 750);
}

void render(Piece* piece, int** board, char* buff, int score, int linesCleared, int level, PieceType* pieces, int pieceIdx, int holdPiece, int height, int width) {
    int cnt = 0;
    clearBuffer(buff);

    for (int i = 0; i < 4; i++) {
        int x = piece->blocks[i]->x + piece->x;
        int y = piece->blocks[i]->y + piece->y;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            board[y][x] = 1;
        }
    }

    int nextPiece[16] = { 0 };
    int hold[16] = { 0 };

    switch (pieces[pieceIdx]) {
        case H_LINE:
            nextPiece[8] = 1;
            nextPiece[9] = 1;
            nextPiece[10] = 1;
            nextPiece[11] = 1;
            break;
        case SQUARE:
            nextPiece[5] = 1;
            nextPiece[6] = 1;
            nextPiece[9] = 1;
            nextPiece[10] = 1;
            break;
        case T_BLOCK:
            nextPiece[6] = 1;
            nextPiece[9] = 1;
            nextPiece[10] = 1;
            nextPiece[11] = 1;
            break;
        case L_BLOCK:
            nextPiece[7] = 1;
            nextPiece[9] = 1;
            nextPiece[10] = 1;
            nextPiece[11] = 1;
            break;
        case RL_BLOCK:
            nextPiece[5] = 1;
            nextPiece[9] = 1;
            nextPiece[10] = 1;
            nextPiece[11] = 1;
            break;
        case S_BLOCK:
            nextPiece[6] = 1;
            nextPiece[7] = 1;
            nextPiece[9] = 1;
            nextPiece[10] = 1;
            break;
        case RS_BLOCK:
            nextPiece[5] = 1;
            nextPiece[6] = 1;
            nextPiece[10] = 1;
            nextPiece[11] = 1;
            break;
    }

    switch (holdPiece) {
        case H_LINE:
            hold[8] = 1;
            hold[9] = 1;
            hold[10] = 1;
            hold[11] = 1;
            break;
        case SQUARE:
            hold[5] = 1;
            hold[6] = 1;
            hold[9] = 1;
            hold[10] = 1;
            break;
        case T_BLOCK:
            hold[6] = 1;
            hold[9] = 1;
            hold[10] = 1;
            hold[11] = 1;
            break;
        case L_BLOCK:
            hold[7] = 1;
            hold[9] = 1;
            hold[10] = 1;
            hold[11] = 1;
            break;
        case RL_BLOCK:
            hold[5] = 1;
            hold[9] = 1;
            hold[10] = 1;
            hold[11] = 1;
            break;
        case S_BLOCK:
            hold[6] = 1;
            hold[7] = 1;
            hold[9] = 1;
            hold[10] = 1;
            break;
        case RS_BLOCK:
            hold[5] = 1;
            hold[6] = 1;
            hold[10] = 1;
            hold[11] = 1;
            break;
    }

    /*for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d", board[i][j]);
        }
        printf("\n");
    }*/

    /*while (getchar() != '\n');*/

    for (int i = 0; i < height; i++) {

        switch (i) {
            case 0:
                cnt += sprintf(buff + cnt, " /- H O L D-\\\t<!");
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 1) * 4; j < i * 4; j++) {
                    if (hold[j] == 1) {
                        cnt += sprintf(buff + cnt, "[]");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\t<!");
                break;
            case 5:
                cnt += sprintf(buff + cnt, " \\----------/\t<!");
                break;
            default:
                cnt += sprintf(buff + cnt, "             \t<!");
                break;
        }

        for (int j = 0; j < width; j++) {
            if (board[i][j] == 1) {
                cnt += sprintf(buff + cnt, "[]");
            } else {
                cnt += sprintf(buff + cnt, " .");
            }
        }

        cnt += sprintf(buff + cnt, "!>");

        switch (i) {
            case 1:
                cnt += sprintf(buff + cnt, "\tLevel:\t%d\n", level);
                break;
            case 3:
                cnt += sprintf(buff + cnt, "\tScore:\t%09d\n", score);
                break;
            case 5:
                cnt += sprintf(buff + cnt, "\tLines:\t%04d\n", linesCleared);
                break;
            case 7:
                cnt += sprintf(buff + cnt, "\t /- N E X T-\\\n");
                break;
            case 8:
            case 9:
            case 10:
            case 11:
                cnt += sprintf(buff + cnt, "\t | ");

                for (int j = (i - 8) * 4; j < (i - 7) * 4; j++) {
                    if (nextPiece[j] == 1) {
                        cnt += sprintf(buff + cnt, "[]");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\n");
                break;
            case 12:
                cnt += sprintf(buff + cnt, "\t \\----------/\n");
                break;
            default:
                cnt += sprintf(buff + cnt, "\n");
                break;
        }
    }

    cnt += sprintf(buff + cnt, "             \t<!");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "==");
    }

    cnt += sprintf(buff + cnt, "!>\n\t\t  ");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "\\/");
    }
    cnt += sprintf(buff + cnt, "\n");
    cnt += sprintf(buff + cnt, "\t\t <- ->   MOVE\n");
    cnt += sprintf(buff + cnt, "\t\t DOWN    SOFT DROP\n");
    cnt += sprintf(buff + cnt, "\t\t SPACE   HARD DROP\n");
    cnt += sprintf(buff + cnt, "\t\t C / UP  ROTATE\n");
    cnt += sprintf(buff + cnt, "\t\t X       HOLD");

    buff[cnt] = '\0';
    //clearConsole();
    setCursorPosition(0, 0);
    printf("%s", buff);

    for (int i = 0; i < 4; i++) {
        int x = piece->blocks[i]->x + piece->x;
        int y = piece->blocks[i]->y + piece->y;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            board[y][x] = 0;
        }
    }
}

int checkInput() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case 72:
                    //UP
                    return 2; //ROTATE RIGHT
                case 80:
                    //DOWN
                    return 3; //DROP
                case 75:
                    //LEFT
                    return -1;
                case 77:
                    //RIGHT
                    return 1;
            }
        } else {
            switch (ch) {
                case 32:
                    //SPACE
                    return 4; // HARD DROP
                case 'c':
                case 'C':
                    return 5; // ROTATE LEFT
                case 'x':
                case 'X':
                    return 6; // BANK PIECE
            }
        }
    }

    return 0;
}

double calcFallTime(int speed) {
    return (double)speed / (double)60;
}

int main(void) {
    srand((unsigned)time(NULL));

    showCursor();

    int width = 10;
    int height = 20;
    double timeForDrop;
    double updateTime = (double)1 / (double)60;

    int** board = (int**) malloc(sizeof(int*) * height);

    if (board == NULL) {
        printf("ERROR ALLOCATING MEMORY.");
        return 0;
    }

    for (int i = 0; i < height; i++) {
        board[i] = (int*) malloc(sizeof(int) * width);

        if (board[i] == NULL) {
            printf("ERROR ALLOCATING MEMORY.");
            for (int j = 0; j < i; j++) {
                free(board[i]);
            }

            free(board);

            return 0;
        }

        memset(board[i], 0, sizeof(int) * width);
    }

    Piece* piece = createPiece(0, 0);

    if (piece == NULL) {
        printf("ERROR ALLOCATING MEMORY.");

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);
        return 0;
    }

    char* buff = (char*) malloc(sizeof(char) * 2000);

    if (buff == NULL) {
        printf("ERROR ALLOCATING MEMORY.");
        destroyPiece(piece);

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);

        return 0;
    }

    PieceType* pieces = (PieceType*) malloc(sizeof(PieceType) * 14);

    if (pieces == NULL) {
        printf("ERROR ALLOCATING MEMORY.\n");
        destroyPiece(piece);

        for (int i = 0; i < height; i++) {
            free(board[i]);
        }

        free(board);
        free(buff);

        return 0;
    }

    double speeds[30] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

    printf("ALL initialised\n");

    clearConsole();
    printf("_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| \n    _|      _|            _|      _|    _|    _|    _|       \n    _|      _|_|_|        _|      _|_|_|      _|      _|_|   \n    _|      _|            _|      _|    _|    _|          _| \n    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   ");
    printf("\n\n\tPress ENTER to start game.");

    while (getchar() != '\n');

startGame:

    for (int i = 0; i < 14; i++) {
        pieces[i] = i % 7;
    }

    int level = -1;
    int running = 1;
    int spawn = 1;
    int pieceIdx = 0;
    int score = 0;
    int linesCleared = 0;
    int holdPiece = -1;
    int changed = 0;

    shufflePieces(pieces, 1);
    shufflePieces(pieces, 2);

    clearConsole();
    printf("_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| \n    _|      _|            _|      _|    _|    _|    _|       \n    _|      _|_|_|        _|      _|_|_|      _|      _|_|   \n    _|      _|            _|      _|    _|    _|          _| \n    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   ");
    printf("\n\nEnter a number from 0 - 9 inclusive as the starting level.\n-> ");
    while (level < 0 || level > 9) {
        scanf("%d", &level);

        if (level < 0 || level > 9) {
            printf("Not a valid number. Try again.\n-> ");
        }
    }

    hideCursor();
    clearConsole();

    clock_t lastUpdate = clock();
    clock_t lastDrop = lastUpdate;

    while (running) {
        clock_t current = clock();
        double elapsed = (double)(current - lastUpdate) / CLOCKS_PER_SEC;
        double sinceDrop = (double)(current - lastDrop) / CLOCKS_PER_SEC;
        int drop = 0;
        int dir = 0;

        if (linesCleared > (level + 1) * 10) {
            level++;
        }

        int tempLevel = level > 29 ? 29 : level;

        timeForDrop = (double) speeds[tempLevel] / (double) 60;

        //timeForDrop = (double)1 / (double)60;

        if (sinceDrop >= timeForDrop) {
            drop = 1;
            lastDrop = current;
        }

        //if (elapsed >= updateTime) {
            //rintf("\nspawn = %d\n", spawn);
        if (spawn == 1) {
            pieceIdx = spawnPiece(piece, board, pieces, -1, height, width);
            spawn = 0;
        } else {
                //printf("Updating\n");

            int input = checkInput();

            switch (input) {
                case 1:
                case -1:
                    dir = input;
                    break;
                case 3:
                    drop = 1;
                    break;
                case 4:
                    drop = -1;
                    break;
                case 2:
                    rotatePiece(piece, 1, board, height, width);
                    break;
                case 5:
                    rotatePiece(piece, -1, board, height, width);
                    break;
                case 6:
                    if (changed) { break; }

                    if (holdPiece != -1) {
                        int temp = piece->type;
                        pieceIdx = spawnPiece(piece, board, pieces, holdPiece, height, width);
                        holdPiece = temp;
                    } else {
                        holdPiece = piece->type;
                        pieceIdx = spawnPiece(piece, board, pieces, -1, height, width);
                        drop = 0;
                        lastDrop = current;
                    }
                    changed = 1;
                    break;
            }

            int res;

            if (drop == 1 || drop == -1) {
                res = updatePiece(piece, dir, drop, board, height, width);
                drop = 0;
            } else {
                res = updatePiece(piece, dir, 0, board, height, width);
            }

                //printf("res = %d\n", res);
        /*
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        printf("%d", board[i][j]);
                    }
                    printf("\n");
                }*/

                //printf("piecePos = (%d, %d)\n", piece->x, piece->y);
                //printf("res = %d\n", res);

            if (res < 0) {
                running = 0;
            } else if (res > 0) {
                // ADD LINES
                changed = 0;
                linesCleared += (res - 1);
                switch(res - 1) {
                    case 1:
                        score += 40 * (level + 1);
                        break;
                    case 2:
                        score += 100 * (level + 1);
                        break;
                    case 3:
                        score += 300 * (level + 1);
                        break;
                    case 4:
                        score += 1200 * (level + 1);
                        break;
                }

                hidePiece(piece);
                spawn = 1;
            }
        }

            //while (getchar() != '\n');

        if (elapsed >= updateTime) {

            render(piece, board, buff, score, linesCleared, level, pieces, pieceIdx, holdPiece, height, width);

            lastUpdate = current;
        }

    }

    showCursor();

    printf("\n\n\t\033[0;31mGAME OVER!\n\033[0mYour score was \033[0;32m%05d!\n\n\033[0mPress \033[0;32mENTER \033[0mto \033[0;32mplay again\033[0m or hit \033[0;31mq \033[0mto \033[0;31mquit\033[0m.", score);

    char c;
    while((c = _getch()) != 'q' && c != '\r');

    if (c == '\r') {
        for (int i = 0; i < height; i++) {
            memset(board[i], 0, sizeof(int) * width);
        }

        goto startGame;
    }

    destroyPiece(piece);

    for (int i = 0; i < height; i++) {
        free(board[i]);
    }

    free(board);
    free(pieces);
    free(buff);

    return 0;
}

/*
_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| 
    _|      _|            _|      _|    _|    _|    _|       
    _|      _|_|_|        _|      _|_|_|      _|      _|_|   
    _|      _|            _|      _|    _|    _|          _| 
    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   
*/