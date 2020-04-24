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
#include "hg_common.h"

/******************************************************************************
 * Two macros that allows to turn to left / right, depending on the current
 * direction. It is important the the result is positive so adding -1 is not
 * possible.
 *****************************************************************************/

#define DIR_MV_LEFT(d)  (((d) + DIR_NUM - 1) % DIR_NUM)

#define DIR_MV_RIGHT(d) (((d) + 1) % DIR_NUM)

/******************************************************************************
 * Definition of string representations of the different directions.
 *****************************************************************************/

#define DIR_STR_UNDEF "DIR-UNDEF"

#define DIR_STR_NN "DIR-NN"
#define DIR_STR_NE "DIR-NE"
#define DIR_STR_SE "DIR-SE"
#define DIR_STR_SS "DIR-SS"
#define DIR_STR_SW "DIR-SW"
#define DIR_STR_NW "DIR-NW"

/******************************************************************************
 * The function returns the string representation of the direction.
 *****************************************************************************/

char* e_dir_str(const e_dir dir) {

	switch (dir) {

	case DIR_UNDEF:
		return DIR_STR_UNDEF;

	case DIR_NN:
		return DIR_STR_NN;

	case DIR_NE:
		return DIR_STR_NE;

	case DIR_SE:
		return DIR_STR_SE;

	case DIR_SS:
		return DIR_STR_SS;

	case DIR_SW:
		return DIR_STR_SW;

	case DIR_NW:
		return DIR_STR_NW;

	default:
		log_exit("Unknown dir: %d", dir)
		;
	}
}

/******************************************************************************
 * Definition of path characters, representing a movement to the left / right /
 * center direction.
 *****************************************************************************/

#define MV_PATH_LEFT   'l'
#define MV_PATH_CENTER 'c'
#define MV_PATH_RIGHT  'r'

/******************************************************************************
 * The function moves the direction depending on a path character.
 *
 * c: go forward
 * l: move left and go forward
 * r: move right and go forward.
 *****************************************************************************/

e_dir e_dir_mv(const e_dir dir, const char chr) {
	e_dir result;

	//
	// Change the direction (relatively) depending on the path character.
	//
	switch (chr) {

	case MV_PATH_LEFT:
		result = DIR_MV_LEFT(dir);
		break;

	case MV_PATH_RIGHT:
		result = DIR_MV_RIGHT(dir);
		break;

	case MV_PATH_CENTER:
		result = dir;
		break;

	default:
		log_exit("Invalid direction: %d", chr)
		;
	}

	log_debug("char: %c dir:  %d", chr, result);

	return result;
}
