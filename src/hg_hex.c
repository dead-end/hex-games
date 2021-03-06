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

#include "hg_hex.h"
#include "hg_color_pair.h"

/******************************************************************************
 * The function computes the hex index from a mouse event, which is the
 * position of the cursor.
 *
 * To compute the index, the columns are split into blocks of 3 columns. There
 * are two cases which are handled separately:
 *
 * - the left column of the block
 * - the center and right column of the block
 *
 * Three hex fields:
 *
 *  ##    ##
 * ####  ####
 * ####OO####
 *  ##OOOO##
 *    OOOO
 *     OO
 *
 * The three hex fields spited into blocks of three columns. The last block is
 * not complete.
 *
 *  ##        ##
 * ###  #    ###  #
 * ###  #OO  ###  #
 *  ##  OOO  O##
 *      OOO  O
 *       OO
 *****************************************************************************/

void hex_get_hex_idx(const int win_row, const int win_col, const s_point *hex_max, s_point *hex_idx) {

	const int col_3_idx = win_col / 3;

	//
	// Handle the left column of the block
	//
	if (win_col % 3 == 0) {
		const int win_row_offset = win_row - 1;

		if (win_row_offset < 0) {
			s_point_set(hex_idx, -1, -1);

		} else {
			hex_idx->col = col_3_idx - ((win_row_offset / 2 + col_3_idx) % 2);

			if (hex_idx->col < 0) {
				s_point_set(hex_idx, -1, -1);

			} else {
				hex_idx->row = win_row_offset / 4;
			}
		}

	}

	//
	// Handle the center and right column of the block.
	//
	else {

		//
		// The rows have toggling offset of 0 and 2.
		//
		const int win_row_offset = win_row - ((col_3_idx % 2 == 1) ? 2 : 0);

		if (win_row_offset < 0) {
			s_point_set(hex_idx, -1, -1);

		} else {
			hex_idx->row = win_row_offset / 4;
			hex_idx->col = col_3_idx;
		}
	}

	//
	// Ensure that the hex index is inside the valid ranges.
	//
	if (hex_idx->row >= hex_max->row || hex_idx->col >= hex_max->col) {
		s_point_set(hex_idx, -1, -1);
	}

	log_debug("Event: %d/%d hex: %d/%d", win_row, win_col, hex_idx->row, hex_idx->col);
}

/******************************************************************************
 * The function sets the values for the 4 corners of the s_hex_field. The
 * corners are ignored.
 *****************************************************************************/

void hex_field_set_corners(s_hex_field *hex_field) {
	log_debug_str("Setting corners!");

	hex_point_set_undef(hex_field->point[0][0]);
	hex_point_set_undef(hex_field->point[0][3]);
	hex_point_set_undef(hex_field->point[3][0]);
	hex_point_set_undef(hex_field->point[3][3]);
}

/******************************************************************************
 * The function prints a hex field, with a foreground and a background hex
 * field.
 *****************************************************************************/

void hex_field_print(WINDOW *win, const s_point *pos_ul, const s_hex_field *hex_field_fg, const s_hex_field *hex_field_bg) {

	//
	// Pointer to a constant value not a constant pointer
	//
	const s_hex_point *hex_point_fg;
	const s_hex_point *hex_point_bg;

	short color_pair;
	short color_bg;
	wchar_t *chr;

	//
	// Loop over the points of the hex field
	//
	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// Ignore the corners of the hex field
			//
			if (hex_field_is_corner(row, col)) {
				continue;
			}

			//
			// Store the background hex point
			//
			hex_point_bg = &hex_field_bg->point[row][col];

			//
			// If the foreground hex field is undefined, we print the hex field
			// of the space. This means, there is no ship or something else in
			// the foreground.
			//
			if (hex_field_fg == NULL || hex_field_fg->point[row][col].chr == W_NULL) {
				chr = hex_point_bg->chr;
				color_pair = cp_color_pair_get(hex_point_bg->fg, hex_point_bg->bg);
			}

			//
			// If the foreground is defined, we have to check the background color.
			//
			else {

				//
				// Store the foreground hex point
				//
				hex_point_fg = &hex_field_fg->point[row][col];

				chr = hex_point_fg->chr;

				//
				// If the foreground hex field has a background color, we use
				// this. Otherwise we used the background color of the space.
				//
				color_bg = hex_point_fg->bg == COLOR_UNDEF ? hex_point_bg->bg : hex_point_fg->bg;
				color_pair = cp_color_pair_get(hex_point_fg->fg, color_bg);
			}

			//
			// Set the color pair and print the character.
			//
			attron(COLOR_PAIR(color_pair));
			mvwaddwstr(win, pos_ul->row + row, pos_ul->col + col, chr);
		}
	}
}

/******************************************************************************
 * The function sets the background color for the hex field.
 *****************************************************************************/

void hex_field_set_bg(s_hex_field *hex_field, const short bg) {

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// Ignore the corners of the hex field
			//
			if (hex_field_is_corner(row, col)) {
				continue;
			}

			//
			// Update the background color
			//
			hex_field->point[row][col].bg = bg;
		}
	}
}
