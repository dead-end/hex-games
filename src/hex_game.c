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

void print_hex_field(const s_point *hex_idx, const e_state state) {

	const short bg_color = space_get_color(hex_idx->row, hex_idx->col, state);

	const short color_pair = cp_color_pair_get(COLOR_WHITE, bg_color);

	const int hex_ul_row = hex_field_ul_row(hex_idx->row, hex_idx->col);
	const int hex_ul_col = hex_field_ul_col(hex_idx->row, hex_idx->col);

	s_hex_point **hex_field = space_get_hex_field(hex_idx);

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// Ignore the corners of the hex field
			//
			if (hex_field_point_is_corner(hex_field[row][col])) {
				continue;
			}

			attron(COLOR_PAIR(color_pair));

			mvaddch(hex_ul_row + row, hex_ul_col + col, hex_field[row][col].chr);
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

void print_hex_fields(const s_point *hex_dim) {
	s_point hex_idx;

	for (hex_idx.row = 0; hex_idx.row < hex_dim->row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < hex_dim->col; hex_idx.col++) {

			print_hex_field(&hex_idx, STATE_NORMAL);
		}
	}
}

#define Q_LRLR L"\x2588"

#define Q_XRLR L"\x259F"
#define Q_LXLR L"\x2599"

#define Q_LRLX L"\x259B"
#define Q_LRXR L"\x259C"

short ship_color_blue1;
short ship_color_blue2;
short ship_color_yellow;

void ship_init() {

	ship_color_blue1 = col_color_create(300, 300, 700);
	ship_color_blue2 = col_color_create(380, 380, 700);

	ship_color_yellow = col_color_create(900, 800, 0);
}

/******************************************************************************
 *
 *****************************************************************************/

void ship_print(const s_point *hex_idx, const int dir) {

	const short bg_color = space_get_color(hex_idx->row, hex_idx->col, STATE_NORMAL);

	const short cp_bgd_blue1 = cp_color_pair_get(ship_color_blue1, bg_color);
	const short cp_bgd_blue2 = cp_color_pair_get(ship_color_blue2, bg_color);

	const short cp_blue1_blue2 = cp_color_pair_get(ship_color_blue1, ship_color_blue2);

	const short cp_blu_yell1 = cp_color_pair_get(ship_color_blue1, ship_color_yellow);
	const short cp_blu_yell2 = cp_color_pair_get(ship_color_blue2, ship_color_yellow);
	const short cp_bgd_yell = cp_color_pair_get(ship_color_yellow, bg_color);

	const int ul_row = hex_field_ul_row(hex_idx->row, hex_idx->col);
	const int ul_col = hex_field_ul_col(hex_idx->row, hex_idx->col);

	//
	// Ship
	//

	if (dir == 0) {

		attron(COLOR_PAIR(cp_bgd_blue1));
		mvaddwstr(ul_row + 0, ul_col + 1, Q_XRLR);
		mvaddwstr(ul_row + 1, ul_col + 0, Q_XRLR);
		mvaddwstr(ul_row + 1, ul_col + 1, Q_LRLR);

		attron(COLOR_PAIR(cp_bgd_blue2));
		mvaddwstr(ul_row + 0, ul_col + 2, Q_LXLR);
		mvaddwstr(ul_row + 1, ul_col + 2, Q_LRLR);
		mvaddwstr(ul_row + 1, ul_col + 3, Q_LXLR);

		attron(COLOR_PAIR(cp_blu_yell1));
		mvaddwstr(ul_row + 2, ul_col + 0, Q_LRLX);
		mvaddwstr(ul_row + 2, ul_col + 1, Q_LRXR);

		attron(COLOR_PAIR(cp_blu_yell2));
		mvaddwstr(ul_row + 2, ul_col + 2, Q_LRLX);
		mvaddwstr(ul_row + 2, ul_col + 3, Q_LRXR);

	} else if (dir == 1) {
		attron(COLOR_PAIR(cp_bgd_blue1));
		mvaddwstr(ul_row + 1, ul_col + 0, Q_XRLR);
		mvaddwstr(ul_row + 1, ul_col + 1, Q_LRLR);

		attron(COLOR_PAIR(cp_bgd_yell));
		mvaddwstr(ul_row + 2, ul_col + 0, Q_LRXR);
		mvaddwstr(ul_row + 3, ul_col + 1, Q_LRXR);

		attron(COLOR_PAIR(cp_blue1_blue2));
		mvaddwstr(ul_row + 1, ul_col + 2, Q_LRLX);
		mvaddwstr(ul_row + 2, ul_col + 1, Q_LRLX);

		attron(COLOR_PAIR(cp_bgd_blue2));
		mvaddwstr(ul_row + 2, ul_col + 2, Q_LRLR);
		mvaddwstr(ul_row + 3, ul_col + 2, Q_LRLX);

	} else {
		log_exit("Unknown dir: %d", dir);
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

	s_point hex_idx_tmp;

	ship_init();

	s_point_set(&hex_idx_tmp, 1, 1);
	ship_print(&hex_idx_tmp, 0);

	s_point_set(&hex_idx_tmp, 3, 3);
	ship_print(&hex_idx_tmp, 1);

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
					print_hex_field(&hex_idx_old, STATE_NORMAL);
				}

				//
				// Print new
				//
				if (hex_idx.row >= 0 && hex_idx.col >= 0) {
					print_hex_field(&hex_idx, STATE_SELECT);
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
