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

#include "hg_viewport.h"
#include "ut_utils.h"

/******************************************************************************
 * The function check the s_viewport_inside_viewport() function.
 *****************************************************************************/

static void test_s_viewport_inside_viewport() {
	bool result;

	s_viewport viewport;
	s_point idx;

	log_debug_str("Start test");

	s_point_set(&viewport.max, 10, 10);
	s_point_set(&viewport.dim, 5, 5);
	s_point_set(&viewport.pos, 2, 2);

	//
	// idx: 0/0
	//
	s_point_set(&idx, 0, 0);

	result = s_viewport_inside_viewport(&viewport, &idx);
	ut_check_bool(result, false, "idx: 0/0");

	//
	// idx: 2/2
	//
	s_point_set(&idx, 2, 2);

	result = s_viewport_inside_viewport(&viewport, &idx);
	ut_check_bool(result, true, "idx: 2/2");

	//
	// idx: 6/6
	//
	s_point_set(&idx, 6, 6);

	result = s_viewport_inside_viewport(&viewport, &idx);
	ut_check_bool(result, true, "idx: 6/6");

	//
	// idx: 7/7
	//
	s_point_set(&idx, 7, 7);

	result = s_viewport_inside_viewport(&viewport, &idx);
	ut_check_bool(result, false, "idx: 7/7");
}

/******************************************************************************
 * The function check the s_viewport_get_ul() function.
 *****************************************************************************/

static void test_s_viewport_get_ul() {
	s_viewport viewport;
	s_point idx_abs;
	s_point pos_ul;
	s_point exp_ul;

	log_debug_str("Start test");

	s_point_set(&viewport.max, 10, 10);
	s_point_set(&viewport.dim, 5, 5);

	//
	// Check - vp: 0/0 abs: 0/0
	//
	s_point_set(&viewport.pos, 0, 0);
	s_point_set(&idx_abs, 0, 0);
	s_point_set(&exp_ul, 0, 0);

	s_viewport_get_ul(&viewport, &idx_abs, &pos_ul);
	ut_check_s_point(&pos_ul, &exp_ul, "vp: 0/0 abs: 0/0");

	//
	// Check - vp: 1/1 abs: 1/1
	//
	s_point_set(&viewport.pos, 1, 1);
	s_point_set(&idx_abs, 1, 1);
	s_point_set(&exp_ul, 2, 0);

	s_viewport_get_ul(&viewport, &idx_abs, &pos_ul);
	ut_check_s_point(&pos_ul, &exp_ul, "vp: 1/1 abs: 1/1");

	//
	// Check - vp: 1/1 abs: 2/2
	//
	s_point_set(&viewport.pos, 1, 1);
	s_point_set(&idx_abs, 2, 2);
	s_point_set(&exp_ul, 4, 3);

	s_viewport_get_ul(&viewport, &idx_abs, &pos_ul);
	ut_check_s_point(&pos_ul, &exp_ul, "vp: 1/1 abs: 2/2");

	//
	// Check - vp: 2/2 abs: 3/3
	//
	s_point_set(&viewport.pos, 2, 2);
	s_point_set(&idx_abs, 3, 3);
	s_point_set(&exp_ul, 6, 3);

	s_viewport_get_ul(&viewport, &idx_abs, &pos_ul);
	ut_check_s_point(&pos_ul, &exp_ul, "2/2 abs: 3/3");

	//
	// Check - vp: 2/2 abs: 4/4
	//
	s_point_set(&viewport.pos, 2, 2);
	s_point_set(&idx_abs, 4, 4);
	s_point_set(&exp_ul, 8, 6);

	s_viewport_get_ul(&viewport, &idx_abs, &pos_ul);
	ut_check_s_point(&pos_ul, &exp_ul, "vp: 2/2 abs: 4/4");
}

/******************************************************************************
 * The function is a helper function for the s_viewport_mv_diff() test.
 *****************************************************************************/

#define MSG_SIZE 32

static void help_s_viewport_mv_diff(const s_point *diff, const bool exp_change, const s_point *exp_pos, const s_point *pos) {
	char msg[MSG_SIZE];

	//
	// Create a message for the test
	//
	snprintf(msg, MSG_SIZE, "diff %d/%d %s pos: %d/%d", diff->row, diff->col, bool_str(exp_change), exp_pos->row, exp_pos->col);

	//
	// Setup viewport
	//
	s_viewport viewport = { .max.row = 8, .max.col = 8, .dim.row = 4, .dim.col = 4, .pos.row = pos->row, .pos.col = pos->col };

	const bool cur_change = s_viewport_mv_diff(&viewport, &(s_point ) { .row = diff->row, .col = diff->col });

	//
	// Ensure that the results are correct.
	//
	ut_check_bool(cur_change, exp_change, msg);
	ut_check_s_point(&viewport.pos, exp_pos, msg);
}

/******************************************************************************
 * The function checks the s_viewport_mv_diff() function.
 *
 * 01234567
 * XXXX----
 * --XXXX--
 * ----XXXX
 *****************************************************************************/

static void test_s_viewport_mv_diff() {
	s_point pos;

	log_debug_str("start");

	//
	// 01234567
	// --XXXX--
	//
	s_point_set(&pos, 2, 2);

	help_s_viewport_mv_diff(&(s_point ) { .row = 0, .col = 0 }, false, &(s_point ) { .row = 2, .col = 2 }, &pos);

	help_s_viewport_mv_diff(&(s_point ) { .row = -1, .col = -1 }, true, &(s_point ) { .row = 1, .col = 1 }, &pos);
	help_s_viewport_mv_diff(&(s_point ) { .row = -2, .col = -2 }, true, &(s_point ) { .row = 0, .col = 0 }, &pos);
	help_s_viewport_mv_diff(&(s_point ) { .row = -3, .col = -3 }, true, &(s_point ) { .row = 0, .col = 0 }, &pos);

	help_s_viewport_mv_diff(&(s_point ) { .row = 1, .col = 1 }, true, &(s_point ) { .row = 3, .col = 3 }, &pos);
	help_s_viewport_mv_diff(&(s_point ) { .row = 2, .col = 2 }, true, &(s_point ) { .row = 4, .col = 4 }, &pos);
	help_s_viewport_mv_diff(&(s_point ) { .row = 3, .col = 3 }, true, &(s_point ) { .row = 4, .col = 4 }, &pos);

	//
	// 01234567
	// XXXX----
	//
	s_point_set(&pos, 0, 0);
	help_s_viewport_mv_diff(&(s_point ) { .row = -1, .col = -1 }, false, &(s_point ) { .row = 0, .col = 0 }, &pos);

	//
	// 01234567
	// ----XXXX
	//
	s_point_set(&pos, 4, 4);
	help_s_viewport_mv_diff(&(s_point ) { .row = 1, .col = 1 }, false, &(s_point ) { .row = 4, .col = 4 }, &pos);

	log_debug_str("end");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_viewport_exec() {

	test_s_viewport_get_ul();

	test_s_viewport_inside_viewport();

	test_s_viewport_mv_diff();
}
