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

#include "hg_color_pair.h"
#include "ut_utils.h"
#include "hg_common.h"

#include "ncurses.h"

/******************************************************************************
 * The function checks the color pairs.
 *****************************************************************************/

static void test_color_pair_add_get() {
	short cp_add, cp_get;

	//
	// Test add and get
	//
	cp_add = cp_color_pair_add(1, 1);
	cp_get = cp_color_pair_get(1, 1);
	ut_check_short(cp_add, cp_get, "Test: 1, 1");
	const short cp_11 = cp_get;

	cp_add = cp_color_pair_add(2, 1);
	cp_get = cp_color_pair_get(2, 1);
	ut_check_short(cp_add, cp_get, "Test: 2, 1");
	const short cp_21 = cp_get;

	cp_add = cp_color_pair_add(1, 2);
	cp_get = cp_color_pair_get(1, 2);
	ut_check_short(cp_add, cp_get, "Test: 1, 2");
	const short cp_12 = cp_get;

	//
	// Get => Add
	//
	cp_add = cp_color_pair_get(3, 3);
	cp_get = cp_color_pair_get(3, 3);
	ut_check_short(cp_add, cp_get, "get get");

	//
	// Repeat
	//
	cp_get = cp_color_pair_get(1, 1);
	ut_check_short(cp_11, cp_get, "Test again: 1, 1");

	cp_get = cp_color_pair_get(2, 1);
	ut_check_short(cp_21, cp_get, "Test again: 2, 1");

	cp_get = cp_color_pair_get(1, 2);
	ut_check_short(cp_12, cp_get, "Test again: 1, 2");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_color_pair_exec() {

	//
	// Initialize screen.
	//
	if (initscr() == NULL) {
		log_exit_str("Unable to initialize the screen.");
	}

	if (start_color() == ERR) {
		log_exit_str("Unable to start color!\n");
	}

	test_color_pair_add_get();

	endwin();
}
