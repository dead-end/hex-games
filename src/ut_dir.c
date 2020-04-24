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

#include "hg_dir.h"
#include "ut_utils.h"

/******************************************************************************
 * The function checks the direction after a movement to a given dfirection.
 *****************************************************************************/

#define CENTER 'c'
#define RIGHT  'r'
#define LEFT   'l'

static void test_dir_mv() {
	e_dir target;

	//
	// Center
	//
	target = e_dir_mv(DIR_NN, CENTER);
	ut_check_int(target, DIR_NN, "DIR_NN-C");

	target = e_dir_mv(DIR_NE, CENTER);
	ut_check_int(target, DIR_NE, "DIR_NE-C");

	target = e_dir_mv(DIR_SE, CENTER);
	ut_check_int(target, DIR_SE, "DIR_SE-C");

	target = e_dir_mv(DIR_SS, CENTER);
	ut_check_int(target, DIR_SS, "DIR_SS-C");

	target = e_dir_mv(DIR_SW, CENTER);
	ut_check_int(target, DIR_SW, "DIR_SW-C");

	target = e_dir_mv(DIR_NW, CENTER);
	ut_check_int(target, DIR_NW, "DIR_NW-C");

	//
	// Right
	//
	target = e_dir_mv(DIR_NN, RIGHT);
	ut_check_int(target, DIR_NE, "DIR_NN-R");

	target = e_dir_mv(DIR_NE, RIGHT);
	ut_check_int(target, DIR_SE, "DIR_NE-R");

	target = e_dir_mv(DIR_SE, RIGHT);
	ut_check_int(target, DIR_SS, "DIR_SE-R");

	target = e_dir_mv(DIR_SS, RIGHT);
	ut_check_int(target, DIR_SW, "DIR_SS-R");

	target = e_dir_mv(DIR_SW, RIGHT);
	ut_check_int(target, DIR_NW, "DIR_SW-R");

	target = e_dir_mv(DIR_NW, RIGHT);
	ut_check_int(target, DIR_NN, "DIR_NW-R");

	//
	// Left
	//
	target = e_dir_mv(DIR_NN, LEFT);
	ut_check_int(target, DIR_NW, "DIR_NN-L");

	target = e_dir_mv(DIR_NE, LEFT);
	ut_check_int(target, DIR_NN, "DIR_NE-L");

	target = e_dir_mv(DIR_SE, LEFT);
	ut_check_int(target, DIR_NE, "DIR_SE-L");

	target = e_dir_mv(DIR_SS, LEFT);
	ut_check_int(target, DIR_SE, "DIR_SS-L");

	target = e_dir_mv(DIR_SW, LEFT);
	ut_check_int(target, DIR_SS, "DIR_SW-L");

	target = e_dir_mv(DIR_NW, LEFT);
	ut_check_int(target, DIR_SW, "DIR_NW-L");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_dir_exec() {

	test_dir_mv();
}

