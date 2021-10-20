main.o:
	gcc -s main.c -o minesweeper
	./minesweeper

valgrind:
	gcc main.c -g -Wall -Wextra -o minesweeper
	valgrind ./minesweeper