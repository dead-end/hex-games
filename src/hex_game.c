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

/******************************************************************************
 * The function prints a ship at a given position.
 *
 * TODO: WINDOW ???
 *****************************************************************************/

void print_ship(const int row, const int col, const s_ship_type *ship_type, const e_dir dir, const e_state state) {
	s_hex_field hf_tmp_bg, hf_tmp_fg;

	const s_point hex_idx = { .row = row, .col = col };

	ship_get_hex_field(ship_type, dir, &hf_tmp_fg);

	space_get_hex_field(&hex_idx, state, &hf_tmp_bg);

	hex_field_print(stdscr, &hex_idx, &hf_tmp_fg, &hf_tmp_bg);
}

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
 * The function prints the space hex fields of the game.
 *****************************************************************************/

void print_hex_fields(const s_point *hex_dim) {
	log_debug_str("Print hex fields");

	s_hex_field space_field_tmp;
	s_point hex_idx;

	for (hex_idx.row = 0; hex_idx.row < hex_dim->row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < hex_dim->col; hex_idx.col++) {

			space_get_hex_field(&hex_idx, STATE_NORMAL, &space_field_tmp);
			hex_field_print(stdscr, &hex_idx, NULL, &space_field_tmp);
		}
	}
}

/******************************************************************************
 * Main
 *****************************************************************************/

int main() {

	s_hex_field hf_tmp_bg;

	s_point hex_idx;

	const s_point hex_max = { .row = 5, .col = 12 };

	s_point hex_idx_old = { .row = -1, .col = -1 };

	hg_init();

	space_init(&hex_max);

	print_hex_fields(&hex_max);

	ship_field_init();

	const s_ship_type *ship_type_normal = ship_type_get(SHIP_TYPE_NORMAL);

	print_ship(0, 1, ship_type_normal, DIR_NN, STATE_NORMAL);
	print_ship(0, 2, ship_type_normal, DIR_NE, STATE_NORMAL);
	print_ship(0, 3, ship_type_normal, DIR_SE, STATE_NORMAL);
	print_ship(1, 3, ship_type_normal, DIR_SS, STATE_NORMAL);
	print_ship(2, 2, ship_type_normal, DIR_SW, STATE_NORMAL);
	print_ship(1, 1, ship_type_normal, DIR_NW, STATE_NORMAL);

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
					space_get_hex_field(&hex_idx_old, STATE_NORMAL, &hf_tmp_bg);
					hex_field_print(stdscr, &hex_idx_old, NULL, &hf_tmp_bg);
				}

				//
				// Print new
				//
				if (hex_idx.row >= 0 && hex_idx.col >= 0) {
					space_get_hex_field(&hex_idx, STATE_SELECT, &hf_tmp_bg);
					hex_field_print(stdscr, &hex_idx, NULL, &hf_tmp_bg);
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
