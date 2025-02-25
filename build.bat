@echo off

gcc -Wall -Wextra -c ./source/tetris.c -o ./source/tetris.o
gcc -Wall -Wextra -c ./source/gamelogic.c -o ./source/gamelogic.o
gcc -Wall -Wextra -c ./source/piece.c -o ./source/piece.o
gcc -Wall -Wextra -c ./source/renderer.c -o ./source/renderer.o
gcc -Wall -Wextra -c ./source/utils.c -o ./source/utils.o
gcc -Wall -Wextra -c ./source/signalhandler.c -o ./source/signalhandler.o

gcc ./source/tetris.o ./source/gamelogic.o ./source/piece.o ./source/renderer.o ./source/utils.o ./source/signalhandler.o -o CTetris.exe

del source\*.o