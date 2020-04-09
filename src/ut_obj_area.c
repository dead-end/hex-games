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
#include "ut_utils.h"
#include "hg_obj_area.h"

/******************************************************************************
 * The function checks a obj_area_goto call.
 *****************************************************************************/

#define BUFFER_SIZE 256

static void test_hex(const s_point *from, const e_dir dir, const int row, const int col) {
	const s_point exp = { .row = row, .col = col };
	char buf[BUFFER_SIZE];
	s_point to;

	obj_area_goto(from, dir, &to);

	snprintf(buf, BUFFER_SIZE, "From: %d/%d dir: %s to: %d/%d", from->row, from->col, e_dir_str(dir), row, col);

	ut_check_s_point(&to, &exp, buf);
}

/******************************************************************************
 * The function checks the obj_area_goto function.
 *****************************************************************************/

void test_obj_area_goto() {
	s_point from;

	//
	// Point with odd / odd coordinates
	//
	s_point_set(&from, 1, 1);

	test_hex(&from, DIR_NN, 0, 1);
	test_hex(&from, DIR_NE, 1, 2);
	test_hex(&from, DIR_SE, 2, 2);

	test_hex(&from, DIR_SS, 2, 1);
	test_hex(&from, DIR_SW, 2, 0);
	test_hex(&from, DIR_NW, 1, 0);

	//
	// Point with even / even coordinates
	//
	s_point_set(&from, 2, 2);

	test_hex(&from, DIR_NN, 1, 2);
	test_hex(&from, DIR_NE, 1, 3);
	test_hex(&from, DIR_SE, 2, 3);

	test_hex(&from, DIR_SS, 3, 2);
	test_hex(&from, DIR_SW, 2, 1);
	test_hex(&from, DIR_NW, 1, 1);

	//
	// Point with even / odd coordinates
	//
	s_point_set(&from, 2, 3);

	test_hex(&from, DIR_NN, 1, 3);
	test_hex(&from, DIR_NE, 2, 4);
	test_hex(&from, DIR_SE, 3, 4);

	test_hex(&from, DIR_SS, 3, 3);
	test_hex(&from, DIR_SW, 3, 2);
	test_hex(&from, DIR_NW, 2, 2);

	//
	// Point with odd / even coordinates
	//
	s_point_set(&from, 3, 2);

	test_hex(&from, DIR_NN, 2, 2);
	test_hex(&from, DIR_NE, 2, 3);
	test_hex(&from, DIR_SE, 3, 3);

	test_hex(&from, DIR_SS, 4, 2);
	test_hex(&from, DIR_SW, 3, 1);
	test_hex(&from, DIR_NW, 2, 1);
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_obj_area_exec() {

	test_obj_area_goto();
}
