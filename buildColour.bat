@echo off

gcc -Wall -Wextra -ggdb3 ./source/color/tetris.c -o CTetrisColour.exe

::set opts=-FC -GR- -EHa- -W4 -nologo -Zi -D_CRT_SECURE_NO_WARNINGS
::cl %opts% .\source\color\tetris.c -FeCTetrisColour.exe /link user32.lib -incremental:no

:: *.o for gcc, *.obj for cl
del source\color\*.o 2> NUL
del .\*.obj 2> NUL 