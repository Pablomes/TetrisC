@echo off

gcc -Wall -Wextra -c ./sourceColour/tetris.c -o ./sourceColour/tetris.o
gcc -Wall -Wextra -c ./sourceColour/gamelogic.c -o ./sourceColour/gamelogic.o
gcc -Wall -Wextra -c ./sourceColour/piece.c -o ./sourceColour/piece.o
gcc -Wall -Wextra -c ./sourceColour/renderer.c -o ./sourceColour/renderer.o
gcc -Wall -Wextra -c ./sourceColour/utils.c -o ./sourceColour/utils.o
gcc -Wall -Wextra -c ./sourceColour/signalhandler.c -o ./sourceColour/signalhandler.o

gcc ./sourceColour/tetris.o ./sourceColour/gamelogic.o ./sourceColour/piece.o ./sourceColour/renderer.o ./sourceColour/utils.o ./sourceColour/signalhandler.o -o CTetrisColour.exe

del sourceColour\*.o