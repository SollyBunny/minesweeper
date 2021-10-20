
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
// Key codes
#define BREAK 3

#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100

unsigned int sizex, sizey, size;
unsigned int cursorpos;

struct Tile {
	unsigned int bomb;
	unsigned int next;
};
struct Tile * grid;

const unsigned int tilecolor[] = { 0,

	21, // 1
	28, // 2
	124,// 3
	17, // 4
	52, // 5
	32, // 6
	255,// 7
	240,// 8

};

void print() {

	printf("\x1b[?25l\x1b[2J\x1b[H\x1b[1m\x1b[7m\x1b[48;5;231m");

	unsigned int x = 0;
	for (unsigned int i = 0; ;++i) {

		if (grid[i].bomb) {
			printf("\x1b[48;5;231m\x1b[38;5;196m|>\x1b[48;5;0m"); // gets converted to fputs?
		} else if (grid[i].next == 0) {
			printf("\x1b[2C");
		} else {
			printf("\x1b[48;5;231m\x1b[38;5;%um%u \x1b[48;5;0m", tilecolor[grid[i].next], grid[i].next);
		}

		x++;
		if (x >= sizex) {
			x = 0;
			if (i >= size - 1) break;
			putchar('\n');
		}

	}

}

unsigned int gettile(unsigned int x, unsigned int y, unsigned int i) {

	// should handle <0 since its unsigned#
	//printf("x: %u, y: %u\n", x, y);
	//printf("x: %u y: %u\n", x, y);
	if (x >= sizex) return 0;
	if (y >= sizey) return 0;
	//i = y * sizex + x;
	//printf("passed\n");
	if (i >= size) {
		//printf("Error: ");
		//printf("x: %u y: %u i: %u\n", x, y, i);
		return 0;
	}
	//printf("x: %u y: %u i: %u out: %u\n", x, y, i, grid[i].bomb);
	return grid[i].bomb;

}

int main(void) {

	srand(time(0));

	struct winsize _w; // get term size
	ioctl(0, TIOCGWINSZ, &_w);
	sizey = _w.ws_row - 2; //TODO SET TO 1
	sizex = _w.ws_col / 2;
	size = sizex * sizey;

	// set terminal settins (dont echo characters / queue stdin / ignore ctrl-c )
	//system("/bin/stty -echo -ixon -icanon time 5 min 1 intr undef");
	struct termios term, restore;
	tcgetattr(0, &term);
	tcgetattr(0, &restore); // backup the original terminal state to restore later
	term.c_lflag &= ~(ICANON|ECHO|ISIG);
	tcsetattr(0, TCSANOW, &term);

	cursorpos = 0;

	grid = malloc(size * sizeof(struct Tile));

	for (unsigned int i = 0; i < size; ++i) {
		grid[i].bomb = (rand() % 20 < 1) ? 1 : 0;
		//printf("%u\n", grid[i].bomb);
	}
	//putchar('\n');
	grid[0].bomb = 1;

	unsigned int x = 0;
	unsigned int y = 0;
	for (unsigned int i = 0; i < size; ++i) {
		
		//printf("%u %u\n", i, x);

		if (grid[i].bomb == 1) {
			grid[i].next = 0;
			goto bombnextloopend;
		}

		// hope the compiler optimizes this ;-;
		grid[i].next = 	gettile(x + 1, y + 1, i + 1 + sizex) +
						gettile(x    , y + 1, i + 0 + sizex) +
						gettile(x - 1, y + 1, i - 1 + sizex) +
						gettile(x + 1, y - 1, i + 1 - sizex) +
						gettile(x    , y - 1, i + 0 - sizex) +
						gettile(x - 1, y - 1, i - 1 - sizex) +
						gettile(x + 1, y    , i + 1 + 0    ) +
						gettile(x - 1, y    , i - 1 + 0    );

		bombnextloopend:
			x++;
			if (x >= sizex) {
				x = 0;
				y++;
			}
		
	}

	print();
	// mainloop
	
	char c;
	goto mainloopstart;
	while ((c = getchar()) != BREAK) {

		printf("\x1b[%u;%uH\x1b[24m\x1b[7m\x1b[25m", cursorpos / sizex + 1, (cursorpos % sizex) * 2 + 1);
		if (grid[cursorpos].bomb) {
			printf("\x1b[48;5;231m\x1b[38;5;196m|>"); // gets converted to fputs?
		} else if (grid[cursorpos].next == 0) {
			printf("\x1b[38;5;0m  ");
		} else {
			printf("\x1b[48;5;231m\x1b[38;5;%um%u ", tilecolor[grid[cursorpos].next], grid[cursorpos].next);
		}

		switch (c) {

			case KEYW:
				cursorpos -= sizex;
				if (cursorpos > size) {
					cursorpos += size;
				}
				break;
				
			case KEYS:
				cursorpos += sizex;
				if (cursorpos > size) {
					cursorpos -= size;
				}
				break;
				
			case KEYA:
				if (cursorpos % sizex == 1) {
					cursorpos += sizex;
				}
				cursorpos--;
				break;
				
			case KEYD:
				if (cursorpos % sizex == sizex - 1) {
					cursorpos -= sizex;
				}
				cursorpos++;
				break;
				
			default:
				continue;
				
		}	

		mainloopstart:

			printf("\x1b[%u;%uH\x1b[4m\x1b[7m\x1b[5m", cursorpos / sizex + 1, (cursorpos % sizex) * 2 + 1);
			if (grid[cursorpos].bomb) {
				printf("\x1b[38;5;231m\x1b[48;5;196m|>\x1b[48;5;0m"); // gets converted to fputs?
			} else if (grid[cursorpos].next == 0) {
				printf("\x1b[38;5;231m  ");
			} else {
				printf("\x1b[38;5;231m\x1b[48;5;%um%u \x1b[48;5;0m", tilecolor[grid[cursorpos].next], grid[cursorpos].next);
			}
			
		
	}

	tcsetattr(0, TCSANOW, &restore); // restore terminal state
	printf("\x1b[?25h\x1b[0m\nBYE BYE\n");

	return 0;
}
