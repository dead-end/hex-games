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

/******************************************************************************
 * The function checks whether an int parameter has the expected value or not.
 *****************************************************************************/

void ut_check_int(const int current, const int expected, const char *msg) {

	if (current != expected) {
		log_exit("[%s] current: %d expected: %d", msg, current, expected);
	}

	log_debug("[%s] OK current: %d", msg, current);
}

/******************************************************************************
 * The function checks whether an short parameter has the expected value or not.
 *****************************************************************************/

void ut_check_short(const short current, const short expected, const char *msg) {

	if (current != expected) {
		log_exit("[%s] current: %d expected: %d", msg, current, expected);
	}

	log_debug("[%s] OK current: %d", msg, current);
}

