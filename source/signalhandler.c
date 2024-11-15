#include "signalhandler.h"

// Do you really need a separate file for this?

void handleSignal(int signal) {
    if (signal == SIGINT) {
        clearConsole();
        printf("\n\nFORCED INTERRUPT DETECTED. CLEANING UP.");
        // do you know for sure you'll be in 2-player mode?
        // if not, this will free trash values which could be a crash
        freeEverything(1);
        printf("\n\nDONE. GOODBYE!!!");
        showCursor();
        exit(0);
    }
}

void setupSignalHandler(void) {
    signal(SIGINT, handleSignal);
}