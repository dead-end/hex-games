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

/*******************************************************************************
 * gcc -Wall -Wextra -Wpedantic -Werror -std=c11 -o s_color_pairs s_color_pairs.c && ./s_color_pairs
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * The definition of the color pair struct.
 ******************************************************************************/

#define CP_MAX 64

#define CP_START 8

typedef struct {

	short fg;
	short bg;
	short cp;

} s_color_pair;

static s_color_pair cp_array[CP_MAX];

static short cp_num = 0;

/*******************************************************************************
 * The comparison function for the qsort and bsearch functions. It compares two
 * s_color_pair by their forground and background color.
 ******************************************************************************/

static int s_color_pair_comp(const void *p1, const void *p2) {

	const s_color_pair *pi1 = (s_color_pair*) p1;
	const s_color_pair *pi2 = (s_color_pair*) p2;

	//
	// If the forground colors differ, we do not need to look at the background
	// color.
	//
	if (pi1->fg != pi2->fg) {
		return pi1->fg - pi2->fg;
	}

	//
	// If the forground colors are equal, we return the difference of the
	// background colors.
	//
	return pi1->bg - pi2->bg;
}

/*******************************************************************************
 * After new entries are added to the array, it has to be sorted. It is not
 * implemented as a macro, so we need not expose the internal structure of the
 * library.
 ******************************************************************************/

void s_color_pair_sort() {

	printf("Sorting array with: %d elements.\n", cp_num);

	qsort(cp_array, cp_num, sizeof(s_color_pair), s_color_pair_comp);
}

/*******************************************************************************
 * The function adds a color pair to the array. The array is not sorted.
 ******************************************************************************/

short s_color_pair_add(const short fg, const short bg) {

	//
	// Ensure that there is space for an other s_color_pair.
	//
	if (cp_num == CP_MAX) {
		perror("Too many pairs!");
	}

	//
	// Set the array data
	//

	cp_array[cp_num].fg = fg;
	cp_array[cp_num].bg = bg;
	cp_array[cp_num].cp = cp_num + CP_START;

	printf("added fg: %d bg: %d cp: %d\n", cp_array[cp_num].fg, cp_array[cp_num].bg, cp_array[cp_num].cp);
	const short cp = cp_array[cp_num].cp;

	//
	// Update the number of pairs
	//
	cp_num++;

	return cp;
}

/*******************************************************************************
 * The function searches a color pair in our array.
 ******************************************************************************/

short s_color_pair_get(const short fg, const short bg) {

	printf("Search fg: %d bg: %d\n", fg, bg);

	//
	// Create a key for the search
	//
	s_color_pair key;

	key.fg = fg;
	key.bg = bg;

	//
	// Do the searching.
	//
	const s_color_pair *result = bsearch(&key, cp_array, cp_num, sizeof(s_color_pair), s_color_pair_comp);

	//
	// If the color pair was foud, we can return the id.
	//
	if (result != NULL) {
		printf("Found fg: %d bg: %d cp: %d\n", result->fg, result->bg, result->cp);
		return result->cp;
	}

	//
	// If the color pair was not found in the array, we add it to the array.
	// Then we need to sort it and return the color pair id for the new entry.
	//
	const short cp = s_color_pair_add(fg, bg);

	s_color_pair_sort();

	return cp;
}

/*******************************************************************************
 *
 ******************************************************************************/

void check_short(const short exp, const short cur) {

	if (exp != cur) {
		printf("expected: %d current: %d\n", exp, cur);
		exit(1);
	}
}

/*******************************************************************************
 * The main function contains some tests.
 ******************************************************************************/

int main() {
	short result;

	result = s_color_pair_add(0, 2);
	check_short(CP_START + 0, result);

	result = s_color_pair_add(1, 1);
	check_short(CP_START + 1, result);

	result = s_color_pair_add(4, 4);
	check_short(CP_START + 2, result);

	s_color_pair_sort();

	result = s_color_pair_get(1, 1);
	check_short(CP_START + 1, result);

	result = s_color_pair_get(2, 2);
	check_short(CP_START + 3, result);

	result = s_color_pair_get(0, 1);
	check_short(CP_START + 4, result);

	for (int i = 0; i < cp_num; i++) {
		printf("fg: %d bg: %d cp: %d\n", cp_array[i].fg, cp_array[i].bg, cp_array[i].cp);
	}

	exit(EXIT_SUCCESS);
}
