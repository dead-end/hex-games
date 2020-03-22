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

#include <ncurses.h>

#ifdef DEBUG
#include <search.h>
#endif

/*******************************************************************************
 * The definition of the color struct, which consists of a red, green and blue
 * value and a color id.
 ******************************************************************************/

typedef struct {

	short red;
	short green;
	short blue;

	short color;

} s_color;

/*******************************************************************************
 * We define an array for the registered colors.
 ******************************************************************************/

#define _COLOR_MAX 64

static size_t _color_num = 0;

static s_color _color_array[_COLOR_MAX];

//
// An offset for the color id.
//
#define _COLOR_START 8

/*******************************************************************************
 * The macro logs the given color.
 ******************************************************************************/

#define log_color(c) log_debug("color: %d r: %d g: %d b: %d",(c)->color, (c)->red, (c)->green, (c)->blue);

/*******************************************************************************
 * The function compares two color structs by the red, green and blue value. The
 * function is only used in DEBUG mode, to ensure that a color combination is
 * allocated only once.
 ******************************************************************************/

#ifdef DEBUG

static int col_color_comp(const void *ptr1, const void *ptr2) {

	const s_color *color1 = (s_color*) ptr1;
	const s_color *color2 = (s_color*) ptr2;

	int result;

	//
	// First compare the red value, then the green and the blue value.
	//
	if (color1->red != color2->red) {
		result = color1->red - color2->red;

	} else if (color1->green != color2->green) {
		result = color1->green - color2->green;

	} else {
		result = color1->blue - color2->blue;
	}

	log_color(color1);

	log_color(color2);

	log_debug("result: %d", result);

	return result;
}

#endif

/*******************************************************************************
 * The function creates a color by its red, green and blue value. Each value has
 * to be between 0 and 1000. The function returns the color id. The color is
 * registered in an array. Currently there is no use for the registered colors.
 ******************************************************************************/

short col_color_create(const short r, const short g, const short b) {

	//
	// Ensure that there is space for an other s_color_pair.
	//
	if (_color_num == _COLOR_MAX) {
		log_exit_str("Too many colors!");
	}

	//
	// Set the array data
	//
	s_color *col_ptr = &_color_array[_color_num];

	col_ptr->red = r;
	col_ptr->green = g;
	col_ptr->blue = b;
	col_ptr->color = _color_num + _COLOR_START;

#ifdef DEBUG

	//
	// In DEBUG mode we ensure that the same color is not registered. A linear
	// search should be ok in DEBUG mode.
	//
	if (lfind(col_ptr, _color_array, &_color_num, sizeof(s_color), col_color_comp) != NULL) {
		log_exit("Color already defined: %d r: %d g: %d b: %d", col_ptr->color, col_ptr->red, col_ptr->green, col_ptr->blue);
	}

#endif

	//
	// Initialize the color.
	//
	if (init_color(col_ptr->color, col_ptr->red, col_ptr->green, col_ptr->blue)) {
		log_exit_str("Unable to create color!");
	}

	log_color(col_ptr);

	//
	// Update the number of colors
	//
	_color_num++;

	return col_ptr->color;
}
