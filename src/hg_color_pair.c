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

/*******************************************************************************
 * The definition of the color pair struct contains a foreground and a
 * background color and an id for the color pair.
 ******************************************************************************/

typedef struct {

	short fg;
	short bg;
	short cp;

} s_color_pair;

/*******************************************************************************
 * We define an array for the registered colors pairs.
 ******************************************************************************/

#define CP_MAX 64

static size_t _cp_num = 0;

static s_color_pair _cp_array[CP_MAX];

//
// An offset for the color pair id.
//
#define CP_START 8

/*******************************************************************************
 * The macro logs the given color pair.
 ******************************************************************************/

#define log_color_pair(c) log_debug("color pair: %d fg: %d bg: %d",(c)->cp, (c)->fg, (c)->bg);

/*******************************************************************************
 * The comparison function for the qsort and bsearch functions. It compares two
 * s_color_pair by their foreground and background color.
 ******************************************************************************/

static int col_color_pair_comp(const void *ptr1, const void *ptr2) {

	const s_color_pair *cp1 = (s_color_pair*) ptr1;
	const s_color_pair *cp2 = (s_color_pair*) ptr2;

	//
	// If the foreground colors differ, we do not need to look at the background
	// color.
	//
	if (cp1->fg != cp2->fg) {
		return cp1->fg - cp2->fg;
	}

	//
	// If the foreground colors are equal, we return the difference of the
	// background colors.
	//
	return cp1->bg - cp2->bg;
}

/*******************************************************************************
 * After new entries are added to the array, it has to be sorted. It is not
 * implemented as a macro, so we need not expose the internal structure of the
 * library.
 ******************************************************************************/

void cp_color_pair_sort() {

	log_debug("Sorting array with: %ld elements.", _cp_num);

	qsort(_cp_array, _cp_num, sizeof(s_color_pair), col_color_pair_comp);
}

/*******************************************************************************
 * The function adds a color pair to the array. The array is not sorted. If you
 * want to add more than one color pair, you can do this without sorting the
 * array every time.
 ******************************************************************************/

short cp_color_pair_add(const short fg, const short bg) {

	//
	// Ensure that there is space for an other s_color_pair.
	//
	if (_cp_num == CP_MAX) {
		log_exit_str("Too many pairs!");
	}

	//
	// Set the array data
	//
	s_color_pair *cp_ptr = &_cp_array[_cp_num];

	cp_ptr->fg = fg;
	cp_ptr->bg = bg;
	cp_ptr->cp = _cp_num + CP_START;

	if (init_pair(cp_ptr->cp, cp_ptr->fg, cp_ptr->bg)) {
		log_exit_str("Unable to create color pair!");
	}

	log_color_pair(cp_ptr);

	//
	// Update the number of pairs
	//
	_cp_num++;

	return cp_ptr->cp;
}

/*******************************************************************************
 * The function searches a color pair in our array of registered color pairs. If
 * the color pair was not found, a new color pair is created and added to the
 * array.
 ******************************************************************************/

short cp_color_pair_get(const short fg, const short bg) {

	log_debug("Search fg: %d bg: %d", fg, bg);

	//
	// Create a key for the search
	//
	s_color_pair key;

	key.fg = fg;
	key.bg = bg;

	//
	// Do the searching.
	//
	const s_color_pair *result = bsearch(&key, _cp_array, _cp_num, sizeof(s_color_pair), col_color_pair_comp);

	//
	// If the color pair was found, we can return the id.
	//
	if (result != NULL) {
		log_color_pair(result);
		return result->cp;
	}

	//
	// If the color pair was not found in the array, we add it to the array.
	// Then we need to sort it and return the color pair id for the new entry.
	//
	const short cp = cp_color_pair_add(fg, bg);

	cp_color_pair_sort();

	return cp;
}
