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

#include "hg_ship.h"

// ----------------------------------------------------------------------------

/******************************************************************************
 * The function prints a hex field. It is called with the state for the
 * underlying hex field of the space. If the hex field is null, then the space
 * hex field is printed.
 *****************************************************************************/

void hex_field_print(WINDOW *win, const s_point *hex_idx, s_hex_point **hex_field_fg, const e_state state) {

	short color_pair;
	short color_bg;
	wchar_t *chr;

	//
	// Get the background color of the space. This depends on the state of the
	// hex field.
	//
	const short color_space_bg = space_get_color(hex_idx->row, hex_idx->col, state);

	//
	// Get the color pair for the space with the given background color. It is
	// used if there is nothing in the forground.
	//
	const short color_pair_space = space_get_color_pair(color_space_bg);

	//
	// Get the upper left corner of the field.
	//
	const int hex_ul_row = hex_field_ul_row(hex_idx->row, hex_idx->col);
	const int hex_ul_col = hex_field_ul_col(hex_idx->row, hex_idx->col);

	//
	// Get the hex field of the space.
	//
	s_hex_point **hex_field_space = space_get_hex_field(hex_idx);

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// Ignore the corners of the hex field
			//
			if (hex_field_is_corner(row, col)) {
				continue;
			}

			//
			// If the foreground hex field is undefined, we print the hex field
			// of the space.
			//
			if (hex_field_fg == NULL || hex_field_fg[row][col].chr == W_NULL) {
				chr = hex_field_space[row][col].chr;
				color_pair = color_pair_space;
			}

			//
			//
			//
			else {
				chr = hex_field_fg[row][col].chr;

				//
				// If the foreground hex field has a background color, we use
				// this. Otherwise we used the background color of the sapce.
				//
				color_bg = hex_field_fg[row][col].bg == COLOR_UNDEF ? color_space_bg : hex_field_fg[row][col].bg;
				color_pair = cp_color_pair_get(hex_field_fg[row][col].fg, color_bg);
			}

			//
			// Set the color pair and print the character.
			//
			attron(COLOR_PAIR(color_pair));
			mvwaddwstr(win, hex_ul_row + row, hex_ul_col + col, chr);
		}
	}
}

// ----------------------------------------------------------------------------

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory. This is
 * important if the program terminates after an error.
 *****************************************************************************/

static void hg_exit_callback() {

	ncur_exit();

	ship_field_free();

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

// ----------------------------------------------------------------------------

/******************************************************************************
 * The function prints the space hex fields of the game.
 *****************************************************************************/

void print_hex_fields(const s_point *hex_dim) {
	log_debug_str("Print hex fields");

	s_point hex_idx;

	for (hex_idx.row = 0; hex_idx.row < hex_dim->row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < hex_dim->col; hex_idx.col++) {

			hex_field_print(stdscr, &hex_idx, NULL, STATE_NORMAL);
		}
	}
}

/******************************************************************************
 * Main
 *****************************************************************************/

extern s_hex_point **_ship_field_templ[DIR_NUM];

extern s_ship_type ship_type_1;

int main() {

	s_hex_point **hex_field = hex_field_alloc();

	s_point hex_idx, hex_idx_old, hex_max;

	s_point_set(&hex_idx_old, -1, -1);

	s_point_set(&hex_max, 5, 12);

	hg_init();

	space_init(&hex_max);

	print_hex_fields(&hex_max);

	s_point hex_idx_tmp;

	ship_field_init();

	s_point_set(&hex_idx_tmp, 0, 1);

	//ship_get_field(&ship_type_1, DIR_NN, hex_field);
	hex_field_print(stdscr, &hex_idx_tmp, _ship_field_templ[DIR_NN], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 0, 2);
	hex_field_print(stdscr, &hex_idx_tmp, _ship_field_templ[DIR_NE], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 0, 3);
	hex_field_print(stdscr, &hex_idx_tmp, _ship_field_templ[DIR_SE], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 1, 3);
	hex_field_print(stdscr, &hex_idx_tmp, _ship_field_templ[DIR_SS], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 2, 2);
	hex_field_print(stdscr, &hex_idx_tmp, _ship_field_templ[DIR_SW], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 1, 1);
	hex_field_print(stdscr, &hex_idx_tmp, _ship_field_templ[DIR_NW], STATE_NORMAL);

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

				//
				// Delete old
				//
				if (hex_idx_old.row >= 0 && hex_idx_old.col >= 0) {
					hex_field_print(stdscr, &hex_idx_old, NULL, STATE_NORMAL);
				}

				//
				// Print new
				//
				if (hex_idx.row >= 0 && hex_idx.col >= 0) {
					hex_field_print(stdscr, &hex_idx, NULL, STATE_SELECT);
				}

				s_point_copy(&hex_idx_old, &hex_idx);
			}
		}
	}

	hex_field_free(hex_field);

	//
	// Cleanup is handled with the exit callback.
	//
	return EXIT_SUCCESS;
}
