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
#include "ut_utils.h"

/******************************************************************************
 * The function checks the computation of the hex index from a mouse event, by
 * testing an individual event.
 *****************************************************************************/

#define BUF_SIZE 32

static void check_hex_get_hex_idx(const int w_row, const int w_col, const int e_row, const int e_col, const s_point *hex_max) {
	char buffer[BUF_SIZE];
	s_point hex_idx;

	//
	// Get the hex index
	//
	hex_get_hex_idx(w_row, w_col, hex_max, &hex_idx);

	//
	// Check the row
	//
	snprintf(buffer, BUF_SIZE, "row - win: %d/%d exp: %d/%d", w_row, w_col, e_row, e_col);
	ut_check_int(hex_idx.row, e_row, buffer);

	//
	// Check the column
	//
	snprintf(buffer, BUF_SIZE, "col - win: %d/%d exp: %d/%d", w_row, w_col, e_row, e_col);
	ut_check_int(hex_idx.col, e_col, buffer);
}

/******************************************************************************
 * The function checks the computation of the hex index from a mouse event, by
 * testing lots of events.
 *****************************************************************************/

static void test_hex_get_hex_idx() {

	const s_point hex_max = { .row = 3, .col = 3 };

	//
	// Win row 0
	//
	check_hex_get_hex_idx(0, 0, -1, -1, &hex_max);

	check_hex_get_hex_idx(0, 1, 0, 0, &hex_max);
	check_hex_get_hex_idx(0, 2, 0, 0, &hex_max);

	check_hex_get_hex_idx(0, 3, -1, -1, &hex_max);
	check_hex_get_hex_idx(0, 4, -1, -1, &hex_max);
	check_hex_get_hex_idx(0, 5, -1, -1, &hex_max);
	check_hex_get_hex_idx(0, 6, -1, -1, &hex_max);

	check_hex_get_hex_idx(0, 7, 0, 2, &hex_max);
	check_hex_get_hex_idx(0, 8, 0, 2, &hex_max);

	//
	// Win row 1
	//
	check_hex_get_hex_idx(1, 0, 0, 0, &hex_max);
	check_hex_get_hex_idx(1, 1, 0, 0, &hex_max);
	check_hex_get_hex_idx(1, 2, 0, 0, &hex_max);
	check_hex_get_hex_idx(1, 3, 0, 0, &hex_max);

	check_hex_get_hex_idx(1, 4, -1, -1, &hex_max);
	check_hex_get_hex_idx(1, 5, -1, -1, &hex_max);

	check_hex_get_hex_idx(1, 6, 0, 2, &hex_max);
	check_hex_get_hex_idx(1, 7, 0, 2, &hex_max);

	//
	// Win row 2
	//
	check_hex_get_hex_idx(2, 0, 0, 0, &hex_max);
	check_hex_get_hex_idx(2, 1, 0, 0, &hex_max);
	check_hex_get_hex_idx(2, 2, 0, 0, &hex_max);
	check_hex_get_hex_idx(2, 3, 0, 0, &hex_max);

	check_hex_get_hex_idx(2, 4, 0, 1, &hex_max);
	check_hex_get_hex_idx(2, 5, 0, 1, &hex_max);

	check_hex_get_hex_idx(2, 6, 0, 2, &hex_max);
	check_hex_get_hex_idx(2, 7, 0, 2, &hex_max);

	//
	// Win row 7
	//
	check_hex_get_hex_idx(7, 0, -1, -1, &hex_max);

	check_hex_get_hex_idx(7, 1, 1, 0, &hex_max);
	check_hex_get_hex_idx(7, 2, 1, 0, &hex_max);

	check_hex_get_hex_idx(7, 3, 1, 1, &hex_max);
	check_hex_get_hex_idx(7, 4, 1, 1, &hex_max);
	check_hex_get_hex_idx(7, 5, 1, 1, &hex_max);
	check_hex_get_hex_idx(7, 6, 1, 1, &hex_max);

	check_hex_get_hex_idx(7, 7, 1, 2, &hex_max);

	//
	// Win col 9
	//
	check_hex_get_hex_idx(0, 9, -1, -1, &hex_max);

	check_hex_get_hex_idx(1, 9, 0, 2, &hex_max);
	check_hex_get_hex_idx(2, 9, 0, 2, &hex_max);

	check_hex_get_hex_idx(3, 9, -1, -1, &hex_max);
	check_hex_get_hex_idx(4, 9, -1, -1, &hex_max);

	check_hex_get_hex_idx(5, 9, 1, 2, &hex_max);
	check_hex_get_hex_idx(6, 9, 1, 2, &hex_max);

	//
	// Win col 10
	//
	check_hex_get_hex_idx(0, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(1, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(2, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(3, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(4, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(5, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(6, 10, -1, -1, &hex_max);
	check_hex_get_hex_idx(7, 10, -1, -1, &hex_max);

	//
	// Win row 12
	//
	check_hex_get_hex_idx(12, 0, -1, -1, &hex_max);
	check_hex_get_hex_idx(12, 1, -1, -1, &hex_max);
	check_hex_get_hex_idx(12, 2, -1, -1, &hex_max);

	check_hex_get_hex_idx(12, 3, 2, 1, &hex_max);
	check_hex_get_hex_idx(12, 4, 2, 1, &hex_max);
	check_hex_get_hex_idx(12, 5, 2, 1, &hex_max);
	check_hex_get_hex_idx(12, 6, 2, 1, &hex_max);

	check_hex_get_hex_idx(12, 7, -1, -1, &hex_max);

	//
	// Win row 13
	//
	check_hex_get_hex_idx(13, 0, -1, -1, &hex_max);
	check_hex_get_hex_idx(13, 1, -1, -1, &hex_max);
	check_hex_get_hex_idx(13, 2, -1, -1, &hex_max);
	check_hex_get_hex_idx(13, 3, -1, -1, &hex_max);

	check_hex_get_hex_idx(13, 4, 2, 1, &hex_max);
	check_hex_get_hex_idx(13, 5, 2, 1, &hex_max);

	check_hex_get_hex_idx(13, 6, -1, -1, &hex_max);
	check_hex_get_hex_idx(13, 7, -1, -1, &hex_max);

	//
	// Win row 14
	//
	check_hex_get_hex_idx(14, 0, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 1, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 2, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 3, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 4, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 5, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 6, -1, -1, &hex_max);
	check_hex_get_hex_idx(14, 7, -1, -1, &hex_max);
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_hex_exec() {

	test_hex_get_hex_idx();
}
