
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>

unsigned int sizex, sizey, size;

struct Tile {
	unsigned int bomb;
	unsigned int next;
};
struct Tile * grid;

char digittoint(unsigned int a) {
	switch(a) {
		case 0: return ' ';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		default: return 'e';
	}
}

void print() {

	unsigned int x = 0;
	for (unsigned int i = 0; i < size; ++i) {

		//putchar(grid[i].bomb == 1 ? 'O' : ' ');
		if (grid[i].bomb) {
			putchar('@');
		} else {
			putchar(digittoint(grid[i].next));
		}

		x++;
		if (x >= sizex) {
			x = 0;
			putchar('-'); putchar('\n');
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
	sizey = _w.ws_row - 1;
	sizex = _w.ws_col / 2;
	size = sizex * sizey;

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

	return 0;
}