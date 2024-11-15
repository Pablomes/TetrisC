@echo off

gcc -Wall -Wextra -ggdb3 ./source/tetris.c -o CTetris.exe

::set opts=-FC -GR- -EHa- -W4 -nologo -Zi -D_CRT_SECURE_NO_WARNINGS
::cl %opts% .\source\tetris.c -FeCTetris.exe /link user32.lib -incremental:no

:: *.o for gcc, *.obj for cl
del source\*.o 2> NUL
del .\*.obj 2> NUL 