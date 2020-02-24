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

#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>

#include "hg_common.h"
#include "hg_ncurses.h"

#include "hg_color.h"
#include "hg_color_pair.h"

#include "hg_hex.h"

#include "hg_space.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory. This is
 * important if the program terminates after an error.
 *****************************************************************************/

static void hg_exit_callback() {

	ncur_exit();

	space_free();

	log_debug_str("Exit callback finished!");
}

/******************************************************************************
 * The initial function of the program.
 *****************************************************************************/

static void hg_init() {

	setlocale(LC_ALL, "");

	ncur_init();

	//
	// Register exit callback.
	//
	if (on_exit(hg_exit_callback, NULL) != 0) {
		log_exit_str("Unable to register exit function!");
	}

	//
	// Initializes random number generator. The function does not return a
	// value.
	//
	time_t t;
	srand((unsigned) time(&t));
}

/******************************************************************************
 *
 *****************************************************************************/

void print_hex_field(const s_point *hex, const short color_pair) {

	const int start_row = hex_start_row(hex->row, hex->col);
	const int start_col = hex_start_col(hex->row, hex->col);

	s_hex_point **hex_field = space_get_hex_field(hex);

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// Ignore the corners of the hex field
			//
			// TODO: maybe a macro
			if (hex_field[row][col].chr == W_NULL) {
				continue;
			}

			attron(COLOR_PAIR(color_pair));

			mvaddch(start_row + row, start_col + col, hex_field[row][col].chr);
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void print_hex_fields(const s_point *hex_max) {
	s_point hex_idx;

	for (hex_idx.row = 0; hex_idx.row < hex_max->row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < hex_max->col; hex_idx.col++) {

			const short bg_color = space_get_color(hex_idx.row, hex_idx.col, STATE_NORMAL);

			const short color_pair = cp_color_pair_get(COLOR_WHITE, bg_color);

			print_hex_field(&hex_idx, color_pair);
		}
	}
}

/******************************************************************************
 * Main
 *****************************************************************************/

int main() {

	s_point hex_idx, hex_idx_old, hex_max;

	s_point_set(&hex_idx_old, -1, -1);

	s_point_set(&hex_max, 5, 12);

	hg_init();

	space_init(&hex_max);

	print_hex_fields(&hex_max);

	for (;;) {
		int c = wgetch(stdscr);

		//
		// Exit with 'q'
		//
		if (c == 'q') {
			break;
		}

		if (c == KEY_MOUSE) {
			MEVENT event;

			if (getmouse(&event) != OK) {
				log_exit_str("Unable to get mouse event!");
			}

			hex_get_hex_idx(event.y, event.x, &hex_idx, &hex_max);

			if (!s_point_same(&hex_idx, &hex_idx_old)) {
				short color_pair;

				//
				// Delete old
				//
				if (hex_idx_old.row >= 0 && hex_idx_old.col >= 0) {

					const short bg_color = space_get_color(hex_idx_old.row, hex_idx_old.col, STATE_NORMAL);

					color_pair = cp_color_pair_get(COLOR_WHITE, bg_color);

					print_hex_field(&hex_idx_old, color_pair);
				}

				//
				// Print new
				//
				if (hex_idx.row >= 0 && hex_idx.col >= 0) {

					const short bg_color = space_get_color(hex_idx.row, hex_idx.col, STATE_SELECT);

					color_pair = cp_color_pair_get(COLOR_WHITE, bg_color);

					print_hex_field(&hex_idx, color_pair);
				}

				s_point_copy(&hex_idx_old, &hex_idx);
			}
		}
	}

	//
	// Cleanup is handled with the exit callback.
	//
	return EXIT_SUCCESS;
}
