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

#include "hg_common.h"
#include "hg_hex.h"

/******************************************************************************
 * The function allocates a 4 x 4 array of s_hex_point, which represents a hex
 * field.
 *****************************************************************************/

s_hex_point** hex_field_alloc() {
	s_hex_point **hex_field;

	hex_field = xmalloc(sizeof(s_hex_point*) * HEX_SIZE);

	for (int row = 0; row < HEX_SIZE; row++) {
		hex_field[row] = xmalloc(sizeof(hex_field) * HEX_SIZE);
	}

	return hex_field;
}

/******************************************************************************
 * The function frees a hex field.
 *****************************************************************************/

void hex_field_free(s_hex_point **hex_field) {

	for (int row = 0; row < HEX_SIZE; row++) {
		free(hex_field[row]);
	}

	free(hex_field);
}

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

void hex_get_hex_idx(const int win_row, const int win_col, s_point *hex_idx, const s_point *hex_max) {

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

	log_debug("Event - row %d col: %d hex - row: %d col: %d", win_row, win_col, hex_idx->row, hex_idx->col);
}
