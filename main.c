
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

#define KEYQ 113
#define KEYE 101

unsigned int sizex, sizey, size;
unsigned int cursorpos;

struct Tile {
	unsigned int bomb;
	unsigned int state;
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

	printf("\x1b[48;5;237m\x1b[?25l\x1b[2J\x1b[H\x1b[1m\x1b[7m");

	/*unsigned int x = 0;
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

	}*/

}

unsigned int gettile(unsigned int x, unsigned int y, unsigned int i) {

	// should handle <0 since its unsigned#
	//printf("x: %u, y: %u\n", x, y);
	//printf("x: %u y: %u\n", x, y);
	if (x >= sizex) return 10;
	if (y >= sizey) return 10;
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

void recursefind(unsigned int x, unsigned int y, unsigned int i) {

	// printf("\x1b[38;5;237m");
	printf("\x1b[24m\x1b[7m\x1b[25m\x1b[48;5;5m");

	i -= sizex;
	i--;

	x--;
	y--;
	
	for (unsigned int sy = 0; sy < 3; ++sy) {
		for (unsigned int sx = 0; sx < 3; ++sx) {
			if (x >= sizex) goto recursefindloopend;
			if (y >= sizey) goto recursefindloopend;
			if (i >= size) goto recursefindloopend;
			//if (gettile(x + sx, y + sy, i) == 10) continue;
			if (grid[i].state != 0) goto recursefindloopend; 
			/*printf("\x1b[%u;%uH\x1b[4m\x1b[7m\x1b[5m", (y + sy + 1), (x * 2) + sx - 1);
			
			printf("A");//"%d %d\n", x + sx, y + sy);*/
			
			printf("\x1b[%u;%uH", (y) + 1, ((x) * 2) - 1);
			grid[i].state = 1;
			if (grid[i].bomb == 0) {
				printf("\x1b[38;5;0m  ");
				fflush(stdout);
				//recursefind(x, y, i);
			} else {
				printf("\x1b[48;5;231m\x1b[38;5;%um%u ", tilecolor[grid[i].bomb], grid[i].bomb);
			}

			recursefindloopend:
				i++;
				x++;			
		}
		i += sizex - 3;
		x -= 3;
		y++;
	}

	/* // dw i made this with mutliple cursors
	if (gettile(x + 1, y + 1, i + 1 + sizex) == 0) {
		if (grid[i + 1 + sizex].state == 0) {
			grid[i + 1 + sizex].state = 1;
			recursefind(x + 1, y + 1, i + 1 + sizex);
		}
	}
	
	if (gettile(x    , y + 1, i + 0 + sizex) == 0) {
		if (grid[i + 0 + sizex].state == 0) {
			grid[i + 0 + sizex].state = 1;
			recursefind(x    , y + 1, i + 0 + sizex);
		}
	}
	
	if (gettile(x - 1, y + 1, i - 1 + sizex) == 0) {
		if (grid[i - 1 + sizex].state == 0) {
			grid[i - 1 + sizex].state = 1;
			recursefind(x - 1, y + 1, i - 1 + sizex);
		}
	}
	
	if (gettile(x + 1, y - 1, i + 1 - sizex) == 0) {
		if (grid[i + 1 - sizex].state == 0) {
			grid[i + 1 - sizex].state = 1;
			recursefind(x + 1, y - 1, i + 1 - sizex);
		}
	}
	
	if (gettile(x    , y - 1, i + 0 - sizex) == 0) {
		if (grid[i + 0 - sizex].state == 0) {
			grid[i + 0 - sizex].state = 1;
			recursefind(x    , y - 1, i + 0 - sizex);
		}
	}
	
	if (gettile(x - 1, y - 1, i - 1 - sizex) == 0) {
		if (grid[i - 1 - sizex].state == 0) {
			grid[i - 1 - sizex].state = 1;
			recursefind(x - 1, y - 1, i - 1 - sizex);
		}
	}
	
	if (gettile(x + 1, y    , i + 1 + 0    ) == 0) {
		if (grid[i + 1 + 0    ].state == 0) {
			grid[i + 1 + 0    ].state = 1;
			recursefind(x + 1, y    , i + 1 + 0    );
		}
	}
	
	if (gettile(x - 1, y    , i - 1 + 0    ) == 0) {
		if (grid[i - 1 + 0    ].state == 0) {
			grid[i - 1 + 0    ].state = 1;
			recursefind(x - 1, y    , i - 1 + 0    );
		}
	}
	*/
	
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
		grid[i].bomb = (rand() % 20 < 1) ? 9 : 0;
		grid[i].state = 0;
		//printf("%u\n", grid[i].bomb);
	}
	//putchar('\n');

	unsigned int x = 0;
	unsigned int y = 0;
	for (unsigned int i = 0; i < size; ++i) {
		
		//printf("%u %u\n", i, x);

		if (grid[i].bomb == 9) {
			goto bombnextloopend;
		}

		// hope the compiler optimizes this ;-;
		grid[i].bomb = 	(gettile(x + 1, y + 1, i + 1 + sizex) == 9) +
						(gettile(x    , y + 1, i + 0 + sizex) == 9) +
						(gettile(x - 1, y + 1, i - 1 + sizex) == 9) +
						(gettile(x + 1, y - 1, i + 1 - sizex) == 9) +
						(gettile(x    , y - 1, i + 0 - sizex) == 9) +
						(gettile(x - 1, y - 1, i - 1 - sizex) == 9) +
						(gettile(x + 1, y    , i + 1 + 0    ) == 9) +
						(gettile(x - 1, y    , i - 1 + 0    ) == 9);

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
		switch (grid[cursorpos].state) {
			case 0:
				printf("\x1b[38;5;237m  ");
				break;
			case 1:
				if (grid[cursorpos].bomb == 0) {
					printf("\x1b[38;5;0m  ");
				} else {
					printf("\x1b[48;5;231m\x1b[38;5;%um%u ", tilecolor[grid[cursorpos].bomb], grid[cursorpos].bomb);
				}
				break;
			case 2:
				printf("\x1b[48;5;231m\x1b[38;5;196m|>"); // gets converted to fputs?
				break;
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
				if (cursorpos % sizex == 0) {
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

			case KEYQ:
				if (grid[cursorpos].state != 0) break;
				if (grid[cursorpos].bomb == 9) goto dead;
				grid[cursorpos].state = 1;
				if (grid[cursorpos].bomb == 0) {
					recursefind(cursorpos % sizex, cursorpos / sizex, cursorpos);
				}
				break;

			case KEYE:
				if (grid[cursorpos].state == 2) {
					grid[cursorpos].state = 0;
					break;
				}
				if (grid[cursorpos].state != 0) break;
				grid[cursorpos].state = 2;
				break;
				
		}	

		mainloopstart:

		
			printf("\x1b[%u;%uH\x1b[4m\x1b[7m\x1b[5m", cursorpos / sizex + 1, (cursorpos % sizex) * 2 + 1);
			switch (grid[cursorpos].state) {
				case 0:
					printf("\x1b[38;5;231m  ");
					break;
				case 1:
					if (grid[cursorpos].bomb == 0) {
						printf("\x1b[38;5;231m  ");
					} else {
						printf("\x1b[38;5;231m\x1b[48;5;%um%u \x1b[48;5;0m", tilecolor[grid[cursorpos].bomb], grid[cursorpos].bomb);
					}
					break;
				case 2:
					printf("\x1b[38;5;231m\x1b[48;5;196m|>\x1b[48;5;0m");
					break;
			}
		
	}

	end:

		tcsetattr(0, TCSANOW, &restore); // restore terminal state
		printf("\x1b[?25h\x1b[0m\n");

		return 0;

	dead:
		
		printf("You died");
		goto end;
}
