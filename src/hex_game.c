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

#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>

#include "hg_common.h"
#include "hg_ncurses.h"

#include "hg_color.h"
#include "hg_color_pair.h"

#include "hg_hex.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory. This is
 * important if the program terminates after an error.
 *****************************************************************************/

static void hg_exit_callback() {

	ncur_exit();

	log_debug_str("Exit callback finished!");
}

/***************************************************************************
 * The initial function of the program.
 **************************************************************************/

static void hg_init() {

	setlocale(LC_ALL, "");

	ncur_init();

	//
	// Register exit callback.
	//
	if (on_exit(hg_exit_callback, NULL) != 0) {
		log_exit_str("Unable to register exit function!");
	}
}

void print_hex(const int hex_idx_row, const int hex_idx_col, const chtype ch, const short color_pair) {

	const int start_row = hex_start_row(hex_idx_row, hex_idx_col);
	const int start_col = hex_start_col(hex_idx_row, hex_idx_col);

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			if (hex_is_hex(row, col)) {

				attron(COLOR_PAIR(color_pair));

				mvaddch(start_row + row, start_col + col, ch);
			}
		}
	}
}

/***************************************************************************
 * Main
 **************************************************************************/

int main() {

	hg_init();

	short colors[3];
	colors[0] = col_color_create(600, 600, 600);
	colors[1] = col_color_create(700, 700, 700);
	colors[2] = col_color_create(800, 800, 800);

	cp_color_pair_add(COLOR_WHITE, colors[0]);
	cp_color_pair_add(COLOR_WHITE, colors[1]);
	cp_color_pair_add(COLOR_WHITE, colors[2]);

	cp_color_pair_sort();

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 12; col++) {

			const int color_idx = hex_bg_color_idx(row, col);

			const short color_pair = cp_color_pair_get(COLOR_WHITE, colors[color_idx]);

			print_hex(row, col, L' ', color_pair);
		}
	}

	getch();

	//
	// Cleanup is handled with the exit callback.
	//
	return EXIT_SUCCESS;
}
