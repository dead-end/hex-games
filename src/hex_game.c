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

// TODO: delete
void ship_field_free();

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
			if (hex_field_is_corner(row, col)) {
				continue;
			}

			attron(COLOR_PAIR(color_pair));

			mvaddwstr(hex_ul_row + row, hex_ul_col + col, hex_field[row][col].chr);
		}
	}
}

// ----------------------------------------------------------------------------

/******************************************************************************
 *
 *****************************************************************************/

void print_hex_fields(const s_point *hex_dim) {
	log_debug_str("Print hex fields");

	s_point hex_idx;

	for (hex_idx.row = 0; hex_idx.row < hex_dim->row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < hex_dim->col; hex_idx.col++) {

			print_hex_field(&hex_idx, STATE_NORMAL);
		}
	}
}

#define Q_UDEF NULL

#define Q_LRLR L"\x2588"

#define Q_XRLR L"\x259F"
#define Q_LXLR L"\x2599"

#define Q_LRLX L"\x259B"
#define Q_LRXR L"\x259C"

short ship_color_blue1;
short ship_color_blue2;
short ship_color_yellow;

#define DIR_NUM 6

typedef enum dir {
	DIR_NN, DIR_NE, DIR_SE, DIR_SS, DIR_SW, DIR_NW
} e_dir;

s_hex_point **ship_field[6];

#define C_UDEF L'\0'
//#define C_LRLR L'\u2588'
#define C_LRLR 0x2588
#define C_XRLR L'\u259F'
#define C_LXLR L'\u2599'
#define C_LRLX L'\u259B'
#define C_LRXR L'\u259C'

void ship_field_corners(s_hex_point **ship_field) {
	log_debug_str("Setting corners!");

	hex_point_set(ship_field[0][0], C_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship_field[0][3], C_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship_field[3][0], C_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship_field[3][3], C_UDEF, COLOR_UNDEF, COLOR_UNDEF);
}

void ship_field_init() {
	log_debug_str("Init ships!");

	ship_color_blue1 = col_color_create(300, 300, 700);
	ship_color_blue2 = col_color_create(400, 400, 700);

	ship_color_yellow = col_color_create(900, 800, 0);

	for (int i = 0; i < DIR_NUM; i++) {
		ship_field[i] = hex_field_alloc();
	}

	s_hex_point **ship;

	// Direction NN
	ship = ship_field[DIR_NN];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_XRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_LXLR, ship_color_blue2, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_XRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[1][2], Q_LRLR, ship_color_blue2, COLOR_UNDEF);
	hex_point_set(ship[1][3], Q_LXLR, ship_color_blue2, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_LRLX, ship_color_blue1, ship_color_yellow);
	hex_point_set(ship[2][1], Q_LRXR, ship_color_blue1, ship_color_yellow);
	hex_point_set(ship[2][2], Q_LRLX, ship_color_blue2, ship_color_yellow);
	hex_point_set(ship[2][3], Q_LRXR, ship_color_blue2, ship_color_yellow);

	hex_point_set(ship[3][1], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	// Direction NN
	ship = ship_field[DIR_NE];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_XRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[1][2], Q_LRLX, ship_color_blue1, ship_color_blue2);
	hex_point_set(ship[1][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_LRXR, ship_color_yellow, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLX, ship_color_blue1, ship_color_blue2);
	hex_point_set(ship[2][2], Q_LRLR, ship_color_blue2, ship_color_blue1);
	hex_point_set(ship[2][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_LRXR, ship_color_yellow, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_LRLX, ship_color_blue2, COLOR_UNDEF);

	// Direction NN
	ship = ship_field[DIR_SE];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_XRLR, ship_color_yellow, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_LXLR, ship_color_blue2, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_XRLR, ship_color_yellow, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LXLR, ship_color_blue1, ship_color_blue2);
	hex_point_set(ship[1][2], Q_LRLR, ship_color_blue2, COLOR_UNDEF);
	hex_point_set(ship[1][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_LRXR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLR, ship_color_blue1, ship_color_blue2);
	hex_point_set(ship[2][2], Q_LXLR, ship_color_blue1, ship_color_blue2);
	hex_point_set(ship[2][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	// Direction SS
	ship = ship_field[DIR_SS];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_LXLR, ship_color_blue1, ship_color_yellow);
	hex_point_set(ship[1][1], Q_XRLR, ship_color_blue1, ship_color_yellow);
	hex_point_set(ship[1][2], Q_LXLR, ship_color_blue2, ship_color_yellow);
	hex_point_set(ship[1][3], Q_XRLR, ship_color_blue2, ship_color_yellow);

	hex_point_set(ship[2][0], Q_LRXR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[2][2], Q_LRLR, ship_color_blue2, COLOR_UNDEF);
	hex_point_set(ship[2][3], Q_LRLX, ship_color_blue2, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_LRXR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_LRLX, ship_color_blue2, COLOR_UNDEF);

	// Direction NN
	ship = ship_field[DIR_SW];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_XRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_LXLR, ship_color_yellow, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[1][2], Q_XRLR, ship_color_blue2, ship_color_blue1);
	hex_point_set(ship[1][3], Q_LXLR, ship_color_yellow, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_XRLR, ship_color_blue2, ship_color_blue1);
	hex_point_set(ship[2][2], Q_LRLR, ship_color_blue2, COLOR_UNDEF);
	hex_point_set(ship[2][3], Q_LRLX, ship_color_blue2, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	// Direction NN
	ship = ship_field[DIR_NW];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRXR, ship_color_blue2, ship_color_blue1);
	hex_point_set(ship[1][2], Q_LRLR, ship_color_blue2, COLOR_UNDEF);
	hex_point_set(ship[1][3], Q_LXLR, ship_color_blue2, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[2][2], Q_LRXR, ship_color_blue2, ship_color_blue1);
	hex_point_set(ship[2][3], Q_LRLX, ship_color_yellow, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_LRXR, ship_color_blue1, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_LRLX, ship_color_yellow, COLOR_UNDEF);

	log_debug_str("Ships ready to fly!");
}

void ship_field_free() {

	for (int i = 0; i < DIR_NUM; i++) {
		free(ship_field[i]);
	}
}

/******************************************************************************
 * The function prints a hex field. It is called with the state for the
 * underlying hex field of the space.
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
			if (hex_field_fg[row][col].chr == Q_UDEF) {
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

	ship_field_init();

	s_point_set(&hex_idx_tmp, 1, 2);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_NN], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 0, 3);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_NE], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 1, 4);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_SE], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 2, 4);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_SS], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 1, 0);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_SW], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 0, 1);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_NW], STATE_NORMAL);

	s_point_set(&hex_idx_tmp, 2, 5);
	hex_field_print(stdscr, &hex_idx_tmp, ship_field[DIR_SE], STATE_NORMAL);

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
