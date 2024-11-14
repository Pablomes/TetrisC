#include "signalhandler.h"

void handleSignal(int signal) {
    if (signal == SIGINT) {
        clearConsole();
        printf("\n\nFORCED INTERRUPT DETECTED. CLEANING UP.");
        freeEverything(20, 1);
        printf("\n\nDONE. GOODBYE!!!");
        showCursor();
        exit(0);
    }
}

void setupSignalHandler(void) {
    signal(SIGINT, handleSignal);
}