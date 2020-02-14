/*
 * MIT License
 *
 * Copyright (c) 2020 dead-end
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/***************************************************************************
 * gcc -Iinclude -Wall -Werror -Wpedantic -DDEBUG -g -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw -std=c11 -o three_colors_field three_colors_field.c -lncursesw -ltinfo && ./three_colors_field
 **************************************************************************/

#define _XOPEN_SOURCE_EXTENDED

#include <stdlib.h>
#include <locale.h>
#include <ncursesw/ncurses.h>

#define TILE_SIZE 4

const char tile_mask[4][4] = { { -1, 0, 1, -1 }, { 0, 1, 1, 2 }, { 0, 1, 1, 2 }, { -1, 1, 2, -1 } };

#define COLOR_GREY_0 2
#define COLOR_GREY_1 3
#define COLOR_GREY_2 4

const short _cp_grey[] = { 2, 3, 4 };

/***************************************************************************
 * Main
 **************************************************************************/

void print_tile(const int tile_y, const int tile_x, const chtype ch, const short cp_grey[]) {

	const int start_y = (tile_x % 2) * 2 + tile_y * 4;
	const int start_x = tile_x * 3;

	for (int y = 0; y < TILE_SIZE; y++) {
		for (int x = 0; x < TILE_SIZE; x++) {

			if (tile_mask[y][x] >= 0) {

				const short color_pair = cp_grey[(unsigned) tile_mask[y][x]];

				attron(COLOR_PAIR(color_pair));

				mvaddch(start_y + y, start_x + x, ch);
			}
		}
	}
}

/***************************************************************************
 * Main
 **************************************************************************/

int main() {
	setlocale(LC_ALL, "");

	if (initscr() == NULL) {
		fprintf(stderr, "Unable to init screen!\n");
		exit(EXIT_FAILURE);
	}

	curs_set(0);

	if (start_color() == ERR) {
		fprintf(stderr, "Unable to start color!\n");
		exit(EXIT_FAILURE);
	}

	init_color(COLOR_GREY_0, 700, 700, 700);
	init_color(COLOR_GREY_1, 750, 750, 750);
	init_color(COLOR_GREY_2, 800, 800, 800);

	init_pair(_cp_grey[0], COLOR_GREY_2, COLOR_GREY_0);
	init_pair(_cp_grey[1], COLOR_GREY_0, COLOR_GREY_1);
	init_pair(_cp_grey[2], COLOR_GREY_1, COLOR_GREY_2);

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 24; x++) {
			print_tile(y, x, L' ', _cp_grey);
		}
	}

	getch();

	endwin();

	printf("finished\n");

	return EXIT_SUCCESS;
}
