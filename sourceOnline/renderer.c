#include "renderer.h"

char* colours[8] = { "\033[96m", "\033[93m", "\033[95m", "\033[97m", "\033[94m", "\033[92m", "\033[91m", "\033[96m" };
char rubbishColour[10] = "\033[90m";

void clearConsole(void) {
    system("cls");
}

// Function to set the cursor position
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
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
            board[y][x] = piece->type + 1;
        }
    }

    int nextPiece[16] = { 0 };
    int hold[16] = { 0 };

    switch (pieces[pieceIdx]) {
        case H_LINE:
            nextPiece[8] = (int)pieces[pieceIdx] + 1;
            nextPiece[9] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            nextPiece[11] = (int)pieces[pieceIdx] + 1;
            break;
        case SQUARE:
            nextPiece[5] = (int)pieces[pieceIdx] + 1;
            nextPiece[6] = (int)pieces[pieceIdx] + 1;
            nextPiece[9] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            break;
        case T_BLOCK:
            nextPiece[6] = (int)pieces[pieceIdx] + 1;
            nextPiece[9] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            nextPiece[11] = (int)pieces[pieceIdx] + 1;
            break;
        case L_BLOCK:
            nextPiece[7] = (int)pieces[pieceIdx] + 1;
            nextPiece[9] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            nextPiece[11] = (int)pieces[pieceIdx] + 1;
            break;
        case RL_BLOCK:
            nextPiece[5] = (int)pieces[pieceIdx] + 1;
            nextPiece[9] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            nextPiece[11] = (int)pieces[pieceIdx] + 1;
            break;
        case S_BLOCK:
            nextPiece[6] = (int)pieces[pieceIdx] + 1;
            nextPiece[7] = (int)pieces[pieceIdx] + 1;
            nextPiece[9] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            break;
        case RS_BLOCK:
            nextPiece[5] = (int)pieces[pieceIdx] + 1;
            nextPiece[6] = (int)pieces[pieceIdx] + 1;
            nextPiece[10] = (int)pieces[pieceIdx] + 1;
            nextPiece[11] = (int)pieces[pieceIdx] + 1;
            break;
        default:
            break;
    }

    switch (holdPiece) {
        case H_LINE:
            hold[8] = (int)holdPiece + 1;
            hold[9] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            hold[11] = (int)holdPiece + 1;
            break;
        case SQUARE:
            hold[5] = (int)holdPiece + 1;
            hold[6] = (int)holdPiece + 1;
            hold[9] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            break;
        case T_BLOCK:
            hold[6] = (int)holdPiece + 1;
            hold[9] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            hold[11] = (int)holdPiece + 1;
            break;
        case L_BLOCK:
            hold[7] = (int)holdPiece + 1;
            hold[9] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            hold[11] = (int)holdPiece + 1;
            break;
        case RL_BLOCK:
            hold[5] = (int)holdPiece + 1;
            hold[9] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            hold[11] = (int)holdPiece + 1;
            break;
        case S_BLOCK:
            hold[6] = (int)holdPiece + 1;
            hold[7] = (int)holdPiece + 1;
            hold[9] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            break;
        case RS_BLOCK:
            hold[5] = (int)holdPiece + 1;
            hold[6] = (int)holdPiece + 1;
            hold[10] = (int)holdPiece + 1;
            hold[11] = (int)holdPiece + 1;
            break;
        default:
            break;
    }

//////////////////////////////////////

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
                    if (hold[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[hold[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]");
                        cnt += sprintf(buff + cnt, "\033[0m");
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
            if (board[i][j] != 0) {
                cnt += sprintf(buff + cnt, "%s", colours[board[i][j] - 1]);
                cnt += sprintf(buff + cnt, "[]\033[0m");
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
                    if (nextPiece[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[nextPiece[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
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

void render2Player(Piece* piece1, Piece* piece2, int** board1, int** board2, char* buff, int score1, int score2, int linesCleared1, int linesCleared2, int level1, int level2, PieceType* pieces1, PieceType* pieces2, int pieceIdx1, int pieceIdx2, int holdPiece1, int holdPiece2, int rubbish1, int rubbish2, int height, int width) {
    int cnt = 0;
    clearBuffer(buff);

    for (int i = 0; i < 4; i++) {
        int x = piece1->blocks[i]->x + piece1->x;
        int y = piece1->blocks[i]->y + piece1->y;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            board1[y][x] = (int)piece1->type + 1;
        }

        x = piece2->blocks[i]->x + piece2->x;
        y = piece2->blocks[i]->y + piece2->y;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            board2[y][x] = (int)piece2->type + 1;
        }
    }

    int nextPiece[16] = { 0 };
    int nextPiece2[16] = { 0 };
    int hold[16] = { 0 };
    int hold2[16] = { 0 };

    switch (pieces1[pieceIdx1]) {
        case H_LINE:
            nextPiece[8] = pieces1[pieceIdx1] + 1;
            nextPiece[9] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            nextPiece[11] = pieces1[pieceIdx1] + 1;
            break;
        case SQUARE:
            nextPiece[5] = pieces1[pieceIdx1] + 1;
            nextPiece[6] = pieces1[pieceIdx1] + 1;
            nextPiece[9] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            break;
        case T_BLOCK:
            nextPiece[6] = pieces1[pieceIdx1] + 1;
            nextPiece[9] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            nextPiece[11] = pieces1[pieceIdx1] + 1;
            break;
        case L_BLOCK:
            nextPiece[7] = pieces1[pieceIdx1] + 1;
            nextPiece[9] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            nextPiece[11] = pieces1[pieceIdx1] + 1;
            break;
        case RL_BLOCK:
            nextPiece[5] = pieces1[pieceIdx1] + 1;
            nextPiece[9] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            nextPiece[11] = pieces1[pieceIdx1] + 1;
            break;
        case S_BLOCK:
            nextPiece[6] = pieces1[pieceIdx1] + 1;
            nextPiece[7] = pieces1[pieceIdx1] + 1;
            nextPiece[9] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            break;
        case RS_BLOCK:
            nextPiece[5] = pieces1[pieceIdx1] + 1;
            nextPiece[6] = pieces1[pieceIdx1] + 1;
            nextPiece[10] = pieces1[pieceIdx1] + 1;
            nextPiece[11] = pieces1[pieceIdx1] + 1;
            break;
        default:
            break;
    }

    switch (pieces2[pieceIdx2]) {
        case H_LINE:
            nextPiece2[8] = pieces2[pieceIdx2] + 1;
            nextPiece2[9] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            nextPiece2[11] = pieces2[pieceIdx2] + 1;
            break;
        case SQUARE:
            nextPiece2[5] = pieces2[pieceIdx2] + 1;
            nextPiece2[6] = pieces2[pieceIdx2] + 1;
            nextPiece2[9] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            break;
        case T_BLOCK:
            nextPiece2[6] = pieces2[pieceIdx2] + 1;
            nextPiece2[9] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            nextPiece2[11] = pieces2[pieceIdx2] + 1;
            break;
        case L_BLOCK:
            nextPiece2[7] = pieces2[pieceIdx2] + 1;
            nextPiece2[9] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            nextPiece2[11] = pieces2[pieceIdx2] + 1;
            break;
        case RL_BLOCK:
            nextPiece2[5] = pieces2[pieceIdx2] + 1;
            nextPiece2[9] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            nextPiece2[11] = pieces2[pieceIdx2] + 1;
            break;
        case S_BLOCK:
            nextPiece2[6] = pieces2[pieceIdx2] + 1;
            nextPiece2[7] = pieces2[pieceIdx2] + 1;
            nextPiece2[9] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            break;
        case RS_BLOCK:
            nextPiece2[5] = pieces2[pieceIdx2] + 1;
            nextPiece2[6] = pieces2[pieceIdx2] + 1;
            nextPiece2[10] = pieces2[pieceIdx2] + 1;
            nextPiece2[11] = pieces2[pieceIdx2] + 1;
            break;
        default:
            break;
    }

    switch (holdPiece1) {
        case H_LINE:
            hold[8] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case SQUARE:
            hold[5] = holdPiece1 + 1;
            hold[6] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            break;
        case T_BLOCK:
            hold[6] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case L_BLOCK:
            hold[7] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case RL_BLOCK:
            hold[5] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case S_BLOCK:
            hold[6] = holdPiece1 + 1;
            hold[7] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            break;
        case RS_BLOCK:
            hold[5] = holdPiece1 + 1;
            hold[6] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        default:
            break;
    }

    switch (holdPiece2) {
        case H_LINE:
            hold2[8] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case SQUARE:
            hold2[5] = holdPiece2 + 1;
            hold2[6] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            break;
        case T_BLOCK:
            hold2[6] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case L_BLOCK:
            hold2[7] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case RL_BLOCK:
            hold2[5] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case S_BLOCK:
            hold2[6] = holdPiece2 + 1;
            hold2[7] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            break;
        case RS_BLOCK:
            hold2[5] = holdPiece2 + 1;
            hold2[6] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        default:
            break;
    }

    ///////////////////////////////////////

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
                    if (hold[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[hold[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\t<!");
                break;
            case 5:
                cnt += sprintf(buff + cnt, " \\----------/\t<!");
                break;
            case 6:
                cnt += sprintf(buff + cnt, " /- N E X T-\\\t<!");
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 7) * 4; j < (i - 6) * 4; j++) {
                    if (nextPiece[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[nextPiece[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\t<!");
                break;
            case 11:
                cnt += sprintf(buff + cnt, " \\----------/\t<!");
                break;
            default:
                cnt += sprintf(buff + cnt, "             \t<!");
                break;
        }

        for (int j = 0; j < width; j++) {
            if (board1[i][j] != 0) {
                if (board1[i][j] == -1) {
                    cnt += sprintf(buff + cnt, "%s", rubbishColour);
                } else {
                    cnt += sprintf(buff + cnt, "%s", colours[board1[i][j] - 1]);
                }
                cnt += sprintf(buff + cnt, "[]\033[0m");
            } else {
                cnt += sprintf(buff + cnt, " .");
            }
        }

        cnt += sprintf(buff + cnt, "!>");

        switch (i) {
            case 1:
                cnt += sprintf(buff + cnt, "\tLevel:\t%d\t\t||\tLevel:\t%d\t\t<!", level1, level2);
                break;
            case 3:
                cnt += sprintf(buff + cnt, "\tScore:\t%09d\t||\tScore:\t%09d\t<!", score1, score2);
                break;
            case 5:
                cnt += sprintf(buff + cnt, "\tLines:\t%04d\t\t||\tLines:\t%04d\t\t<!", linesCleared1, linesCleared2);
                break;
            case 7:
                cnt += sprintf(buff + cnt, "\tTrash:\t%d\t\t||\tTrash:\t%d\t\t<!", rubbish1, rubbish2);
                break;
            default:
                cnt += sprintf(buff + cnt, "\t      \t         \t||\t      \t         \t<!");
                break;
        }

        for (int j = 0; j < width; j++) {
            if (board2[i][j] != 0) {
                if (board2[i][j] == -1) {
                    cnt += sprintf(buff + cnt, "%s", rubbishColour);
                } else {
                    cnt += sprintf(buff + cnt, "%s", colours[board2[i][j] - 1]);
                }
                cnt += sprintf(buff + cnt, "[]\033[0m");
            } else {
                cnt += sprintf(buff + cnt, " .");
            }
        }

        cnt += sprintf(buff + cnt, "!>\t");

        switch (i) {
            case 0:
                cnt += sprintf(buff + cnt, " /- H O L D-\\\n");
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 1) * 4; j < i * 4; j++) {
                    if (hold2[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[hold2[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\n");
                break;
            case 5:
                cnt += sprintf(buff + cnt, " \\----------/\n");
                break;
            case 6:
                cnt += sprintf(buff + cnt, " /- N E X T-\\\n");
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 7) * 4; j < (i - 6) * 4; j++) {
                    if (nextPiece2[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[nextPiece2[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\n");
                break;
            case 11:
                cnt += sprintf(buff + cnt, " \\----------/\n");
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

    cnt += sprintf(buff + cnt, "!>\t      \t         \t||\t      \t         \t<!");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "==");
    }

    cnt += sprintf(buff + cnt, "!>\n\t\t  ");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "\\/");
    }

    cnt += sprintf(buff + cnt, "  \t      \t         \t||\t      \t         \t  ");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "\\/");
    }

    cnt += sprintf(buff + cnt, "\n\n");
    cnt += sprintf(buff + cnt, "\t\t PLAYER 1    \t\t\t\t\t\t\t\t\t\t PLAYER 2\n");
    cnt += sprintf(buff + cnt, "\t\t A / W   MOVE\t\t\t\t\t\t\t\t\t\t <- ->   MOVE\n");
    cnt += sprintf(buff + cnt, "\t\t S       SOFT DROP\t\t\t\t\t\t\t\t\t DOWN    SOFT DROP\n");
    cnt += sprintf(buff + cnt, "\t\t SPACE   HARD DROP\t\t\t\t\t\t\t\t\t L       HARD DROP\n");
    cnt += sprintf(buff + cnt, "\t\t W / C   ROTATE\t\t\t\t\t\t\t\t\t\t K / UP  ROTATE\n");
    cnt += sprintf(buff + cnt, "\t\t B       HOLD\t\t\t\t\t\t\t\t\t\t J       HOLD\n");

///////////////////////////////////////////////

    buff[cnt] = '\0';
    //clearConsole();
    setCursorPosition(0, 0);
    printf("%s", buff);

    for (int i = 0; i < 4; i++) {
        int x = piece1->blocks[i]->x + piece1->x;
        int y = piece1->blocks[i]->y + piece1->y;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            board1[y][x] = 0;
        }

        x = piece2->blocks[i]->x + piece2->x;
        y = piece2->blocks[i]->y + piece2->y;

        if (x >= 0 && x < width && y >= 0 && y < height) {
            board2[y][x] = 0;
        }
    }
}

void renderOnline(int** board1, int** board2, char* buff, int score1, int score2, int linesCleared1, int linesCleared2, int level1, int level2, int nextPieceNum1, int nextPieceNum2, int holdPiece1, int holdPiece2, int rubbish1, int rubbish2, int height, int width) {
    int cnt = 0;
    clearBuffer(buff);

    int nextPiece[16] = { 0 };
    int nextPiece2[16] = { 0 };
    int hold[16] = { 0 };
    int hold2[16] = { 0 };

    switch (nextPieceNum1) {
        case H_LINE:
            nextPiece[8] = nextPieceNum1 + 1;
            nextPiece[9] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            nextPiece[11] = nextPieceNum1 + 1;
            break;
        case SQUARE:
            nextPiece[5] = nextPieceNum1 + 1;
            nextPiece[6] = nextPieceNum1 + 1;
            nextPiece[9] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            break;
        case T_BLOCK:
            nextPiece[6] = nextPieceNum1 + 1;
            nextPiece[9] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            nextPiece[11] = nextPieceNum1 + 1;
            break;
        case L_BLOCK:
            nextPiece[7] = nextPieceNum1 + 1;
            nextPiece[9] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            nextPiece[11] = nextPieceNum1 + 1;
            break;
        case RL_BLOCK:
            nextPiece[5] = nextPieceNum1 + 1;
            nextPiece[9] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            nextPiece[11] = nextPieceNum1 + 1;
            break;
        case S_BLOCK:
            nextPiece[6] = nextPieceNum1 + 1;
            nextPiece[7] = nextPieceNum1 + 1;
            nextPiece[9] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            break;
        case RS_BLOCK:
            nextPiece[5] = nextPieceNum1 + 1;
            nextPiece[6] = nextPieceNum1 + 1;
            nextPiece[10] = nextPieceNum1 + 1;
            nextPiece[11] = nextPieceNum1 + 1;
            break;
        default:
            break;
    }

    switch (nextPieceNum2) {
        case H_LINE:
            nextPiece2[8] = nextPieceNum2 + 1;
            nextPiece2[9] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            nextPiece2[11] = nextPieceNum2 + 1;
            break;
        case SQUARE:
            nextPiece2[5] = nextPieceNum2 + 1;
            nextPiece2[6] = nextPieceNum2 + 1;
            nextPiece2[9] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            break;
        case T_BLOCK:
            nextPiece2[6] = nextPieceNum2 + 1;
            nextPiece2[9] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            nextPiece2[11] = nextPieceNum2 + 1;
            break;
        case L_BLOCK:
            nextPiece2[7] = nextPieceNum2 + 1;
            nextPiece2[9] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            nextPiece2[11] = nextPieceNum2 + 1;
            break;
        case RL_BLOCK:
            nextPiece2[5] = nextPieceNum2 + 1;
            nextPiece2[9] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            nextPiece2[11] = nextPieceNum2 + 1;
            break;
        case S_BLOCK:
            nextPiece2[6] = nextPieceNum2 + 1;
            nextPiece2[7] = nextPieceNum2 + 1;
            nextPiece2[9] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            break;
        case RS_BLOCK:
            nextPiece2[5] = nextPieceNum2 + 1;
            nextPiece2[6] = nextPieceNum2 + 1;
            nextPiece2[10] = nextPieceNum2 + 1;
            nextPiece2[11] = nextPieceNum2 + 1;
            break;
        default:
            break;
    }

    switch (holdPiece1) {
        case H_LINE:
            hold[8] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case SQUARE:
            hold[5] = holdPiece1 + 1;
            hold[6] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            break;
        case T_BLOCK:
            hold[6] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case L_BLOCK:
            hold[7] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case RL_BLOCK:
            hold[5] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        case S_BLOCK:
            hold[6] = holdPiece1 + 1;
            hold[7] = holdPiece1 + 1;
            hold[9] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            break;
        case RS_BLOCK:
            hold[5] = holdPiece1 + 1;
            hold[6] = holdPiece1 + 1;
            hold[10] = holdPiece1 + 1;
            hold[11] = holdPiece1 + 1;
            break;
        default:
            break;
    }

    switch (holdPiece2) {
        case H_LINE:
            hold2[8] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case SQUARE:
            hold2[5] = holdPiece2 + 1;
            hold2[6] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            break;
        case T_BLOCK:
            hold2[6] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case L_BLOCK:
            hold2[7] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case RL_BLOCK:
            hold2[5] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        case S_BLOCK:
            hold2[6] = holdPiece2 + 1;
            hold2[7] = holdPiece2 + 1;
            hold2[9] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            break;
        case RS_BLOCK:
            hold2[5] = holdPiece2 + 1;
            hold2[6] = holdPiece2 + 1;
            hold2[10] = holdPiece2 + 1;
            hold2[11] = holdPiece2 + 1;
            break;
        default:
            break;
    }

    ///////////////////////////////////////

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
                    if (hold[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[hold[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\t<!");
                break;
            case 5:
                cnt += sprintf(buff + cnt, " \\----------/\t<!");
                break;
            case 6:
                cnt += sprintf(buff + cnt, " /- N E X T-\\\t<!");
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 7) * 4; j < (i - 6) * 4; j++) {
                    if (nextPiece[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[nextPiece[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\t<!");
                break;
            case 11:
                cnt += sprintf(buff + cnt, " \\----------/\t<!");
                break;
            default:
                cnt += sprintf(buff + cnt, "             \t<!");
                break;
        }

        for (int j = 0; j < width; j++) {
            if (board1[i][j] != 0) {
                if (board1[i][j] == -1) {
                    cnt += sprintf(buff + cnt, "%s", rubbishColour);
                } else {
                    cnt += sprintf(buff + cnt, "%s", colours[board1[i][j] - 1]);
                }
                cnt += sprintf(buff + cnt, "[]\033[0m");
            } else {
                cnt += sprintf(buff + cnt, " .");
            }
        }

        cnt += sprintf(buff + cnt, "!>");

        switch (i) {
            case 1:
                cnt += sprintf(buff + cnt, "\tLevel:\t%d\t\t||\tLevel:\t%d\t\t<!", level1, level2);
                break;
            case 3:
                cnt += sprintf(buff + cnt, "\tScore:\t%09d\t||\tScore:\t%09d\t<!", score1, score2);
                break;
            case 5:
                cnt += sprintf(buff + cnt, "\tLines:\t%04d\t\t||\tLines:\t%04d\t\t<!", linesCleared1, linesCleared2);
                break;
            case 7:
                cnt += sprintf(buff + cnt, "\tTrash:\t%d\t\t||\tTrash:\t%d\t\t<!", rubbish1, rubbish2);
                break;
            default:
                cnt += sprintf(buff + cnt, "\t      \t         \t||\t      \t         \t<!");
                break;
        }

        for (int j = 0; j < width; j++) {
            if (board2[i][j] != 0) {
                if (board2[i][j] == -1) {
                    cnt += sprintf(buff + cnt, "%s", rubbishColour);
                } else {
                    cnt += sprintf(buff + cnt, "%s", colours[board2[i][j] - 1]);
                }
                cnt += sprintf(buff + cnt, "[]\033[0m");
            } else {
                cnt += sprintf(buff + cnt, " .");
            }
        }

        cnt += sprintf(buff + cnt, "!>\t");

        switch (i) {
            case 0:
                cnt += sprintf(buff + cnt, " /- H O L D-\\\n");
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 1) * 4; j < i * 4; j++) {
                    if (hold2[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[hold2[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\n");
                break;
            case 5:
                cnt += sprintf(buff + cnt, " \\----------/\n");
                break;
            case 6:
                cnt += sprintf(buff + cnt, " /- N E X T-\\\n");
                break;
            case 7:
            case 8:
            case 9:
            case 10:
                cnt += sprintf(buff + cnt, " | ");

                for (int j = (i - 7) * 4; j < (i - 6) * 4; j++) {
                    if (nextPiece2[j] != 0) {
                        cnt += sprintf(buff + cnt, "%s", colours[nextPiece2[j] - 1]);
                        cnt += sprintf(buff + cnt, "[]\033[0m");
                    } else {
                        cnt += sprintf(buff + cnt, "  ");
                    }
                }

                cnt += sprintf(buff + cnt, " |\n");
                break;
            case 11:
                cnt += sprintf(buff + cnt, " \\----------/\n");
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

    cnt += sprintf(buff + cnt, "!>\t      \t         \t||\t      \t         \t<!");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "==");
    }

    cnt += sprintf(buff + cnt, "!>\n\t\t  ");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "\\/");
    }

    cnt += sprintf(buff + cnt, "  \t      \t         \t||\t      \t         \t  ");

    for (int i = 0; i < width; i++) {
        cnt += sprintf(buff + cnt, "\\/");
    }
    cnt += sprintf(buff + cnt, "\n");
    cnt += sprintf(buff + cnt, "\t\t <- ->   MOVE\n");
    cnt += sprintf(buff + cnt, "\t\t DOWN    SOFT DROP\n");
    cnt += sprintf(buff + cnt, "\t\t SPACE   HARD DROP\n");
    cnt += sprintf(buff + cnt, "\t\t C / UP  ROTATE\n");
    cnt += sprintf(buff + cnt, "\t\t X       HOLD");

///////////////////////////////////////////////

    buff[cnt] = '\0';
    //clearConsole();
    setCursorPosition(0, 0);
    printf("%s", buff);
}

GameMode showTitle() {

    GameMode gamemode = SINGLE;
    hideCursor();

title:

    clearConsole();
    printf("_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| \n    _|      _|            _|      _|    _|    _|    _|       \n    _|      _|_|_|        _|      _|_|_|      _|      _|_|   \n    _|      _|            _|      _|    _|    _|          _| \n    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   \n\n");
    if (gamemode == SINGLE) {
        printf("\t --> ");
    } else {
        printf("\t     ");
    }

    printf("1 PLAYER\t");

    if (gamemode == TWOPLAYER) {
        printf("--> ");
    } else {
        printf("    ");
    }

    printf("2 PLAYER\t");

    if (gamemode == ONLINE) {
        printf("--> ");
    } else {
        printf("    ");
    }

    printf("ONLINE\n\n\tPress ENTER to start game.\n\tUse arrow keys to choose.");

    int c;

    while ((c = _getch()) != '\r' && c != 0 && c != 224);

    if (c == 0 || c == 224) {
        c = _getch();

        if (c == 75) {
            gamemode = (gamemode == SINGLE) ? ONLINE : ((gamemode == ONLINE) ? TWOPLAYER : SINGLE);

        } else if (c == 77) {
            gamemode = (gamemode == SINGLE) ? TWOPLAYER : ((gamemode == ONLINE) ? SINGLE : ONLINE);
        }

        goto title;
    }

    showCursor();

    return gamemode;
}

int levelMenu() {

    int lev = -1;

    clearConsole();
    printf("_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| \n    _|      _|            _|      _|    _|    _|    _|       \n    _|      _|_|_|        _|      _|_|_|      _|      _|_|   \n    _|      _|            _|      _|    _|    _|          _| \n    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   ");
    printf("\n\nEnter a number from 0 - 9 inclusive as the starting level.\n-> ");
    while (lev < 0 || lev > 9) {
        scanf("%d", &lev);

        if (lev < 0 || lev > 9) {
            printf("Not a valid number. Try again.\n-> ");
        }
    }

    hideCursor();
    clearConsole();

    return lev;
}

int onlineMenu() {
    int admin = 1;
    hideCursor();

oMenu:
    clearConsole();
    printf("_|_|_|_|_|  _|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_|    _|_|_| \n    _|      _|            _|      _|    _|    _|    _|       \n    _|      _|_|_|        _|      _|_|_|      _|      _|_|   \n    _|      _|            _|      _|    _|    _|          _| \n    _|      _|_|_|_|      _|      _|    _|  _|_|_|  _|_|_|   ");
    printf("\n\nWould you like to connect as server or client.\n Use the arrow keys to decide and press ENTER to choose.\n\n");

    if (admin) {
        printf("\t--> ");
    } else {
        printf("\t    ");
    }
    printf("SERVER");

    if (!admin) {
        printf("\t\t--> ");
    } else {
        printf("\t\t    ");
    }
    printf("CLIENT");

    int c;

    while ((c = _getch()) != '\r' && c != 0 && c != 224);

    if (c == 0 || c == 224) {
        c = _getch();

        if (c == 75 || c == 77) {
            admin = admin ? 0 : 1;

        }

        goto oMenu;
    }

    showCursor();

    return admin;
}

char gameOverScreen(int score, int score2, int gamemode, int winner) {
    showCursor();

    if (gamemode == SINGLE) {
        printf("\n\n\t\033[0;31mGAME OVER!\n\033[0mYour score was \033[0;32m%09d!\n\n\033[0mPress \033[0;32mENTER \033[0mto \033[0;32mplay again\033[0m,\033[0;33m M \033[0m to go back to the \033[0;33m MAIN MENU \033[0mor hit \033[0;31mQ \033[0mto \033[0;31mQUIT\033[0m.", score);
    } else if (gamemode == TWOPLAYER) {
        printf("\n\n\t\033[0;32mPLAYER %d WON!\n\nYour scores were:\n\033[0mPlayer 1: \033[0;32m%09d\n\033[0mPlayer 2: \033[0;32m%09d\n\n\033[0mPress \033[0;32mENTER \033[0mto \033[0;32mplay again,\033[0;33m M \033[0m to go back to the \033[0;33m MAIN MENU \033[0mor hit \033[0;31mQ \033[0mto \033[0;31mQUIT\033[0m.", winner, score, score2);
    } else {
        if (winner == 1) {
            printf("\n\n\t\033[0;32mYOU WON!\n\nYour scores were:\n\033[0mYou: \033[0;32m%09d\n\033[0mOther: \033[0;32m%09d\n\n\033[0mPress \033[0;32mENTER or M \033[0mto go back to the \033[0;33m MAIN MENU \033[0mor hit \033[0;31mQ \033[0mto \033[0;31mQUIT\033[0m.", score, score2);
        } else {
            printf("\n\n\t\033[0;32mYOU LOST...\n\nYour scores were:\n\033[0mYou: \033[0;32m%09d\n\033[0mOther: \033[0;32m%09d\n\n\033[0mPress \033[0;32mENTER or M \033[0mto go back to the \033[0;33m MAIN MENU \033[0mor hit \033[0;31mQ \033[0mto \033[0;31mQUIT\033[0m.", score, score2);
        }
    
    }

    char c;
    while((c = _getch()) != 'q' && (c != '\r' || gamemode == ONLINE) && c != 'm');

    return c;
}