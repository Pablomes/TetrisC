@echo off

gcc -Wall -Wextra -c ./sourceOnline/tetris.c -o ./sourceOnline/tetris.o
gcc -Wall -Wextra -c ./sourceOnline/gamelogic.c -o ./sourceOnline/gamelogic.o
gcc -Wall -Wextra -c ./sourceOnline/piece.c -o ./sourceOnline/piece.o
gcc -Wall -Wextra -c ./sourceOnline/renderer.c -o ./sourceOnline/renderer.o
gcc -Wall -Wextra -c ./sourceOnline/utils.c -o ./sourceOnline/utils.o
gcc -Wall -Wextra -c ./sourceOnline/signalhandler.c -o ./sourceOnline/signalhandler.o
gcc -Wall -Wextra -c ./sourceOnline/online.c -o ./sourceOnline/online.o

gcc ./sourceOnline/tetris.o ./sourceOnline/gamelogic.o ./sourceOnline/piece.o ./sourceOnline/renderer.o ./sourceOnline/utils.o ./sourceOnline/signalhandler.o ./sourceOnline/online.o -o CTetrisOnline.exe -lws2_32

del sourceOnline\*.o