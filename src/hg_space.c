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

#include <ncurses.h>

#include "hg_common.h"
#include "hg_hex.h"
#include "hg_color.h"
#include "hg_color_pair.h"
#include "hg_space.h"

//
// On average 1 of RAND_START chars is a star.
//
#define RAND_START 24

static s_hex_point ****__space;

static s_point __dim_space;

short colors_normal[3];
short colors_select[3];

/******************************************************************************
 * The function initializes the colors.
 *****************************************************************************/

static void space_init_colors() {

	colors_normal[0] = col_color_create(50, 50, 50);
	colors_normal[1] = col_color_create(80, 80, 80);
	colors_normal[2] = col_color_create(110, 110, 110);

	colors_select[0] = col_color_create(150, 50, 50);
	colors_select[1] = col_color_create(180, 80, 80);
	colors_select[2] = col_color_create(210, 110, 110);

	cp_color_pair_add(COLOR_WHITE, colors_normal[0]);
	cp_color_pair_add(COLOR_WHITE, colors_normal[1]);
	cp_color_pair_add(COLOR_WHITE, colors_normal[2]);

	cp_color_pair_add(COLOR_WHITE, colors_select[0]);
	cp_color_pair_add(COLOR_WHITE, colors_select[1]);
	cp_color_pair_add(COLOR_WHITE, colors_select[2]);

	cp_color_pair_sort();
}

/******************************************************************************
 * The macro definitions.
 *****************************************************************************/

#define space_get_bg_color_idx(r,c) ((r) + 2 * ((c) % 2)) % 3

/******************************************************************************
 *
 *****************************************************************************/

short space_get_color(const int row, const int col, const e_state state) {
	short result;

	const int idx = space_get_bg_color_idx(row, col);

	if (state == STATE_NORMAL) {
		result = colors_normal[idx];

	} else if (state == STATE_SELECT) {
		result = colors_select[idx];

	} else {
		log_exit("Unknown state: %d", state);
	}

	log_debug("state: %d pos: %d/%d idx: %d result: %d", state, row, col, idx, result);

	return result;
}

/******************************************************************************
 * The function allocates the array for the space field.
 *****************************************************************************/

s_hex_point**** space_alloc(const s_point *dim) {

	log_debug("Creating space with: %d/%d hex fields", dim->row, dim->col);

	s_hex_point ****space = xmalloc(sizeof(s_hex_point***) * dim->row);

	for (int row = 0; row < dim->row; row++) {
		space[row] = xmalloc(sizeof(s_hex_point**) * dim->col);

		for (int col = 0; col < dim->col; col++) {
			space[row][col] = hex_field_alloc();
		}
	}

	return space;
}

/******************************************************************************
 * The function frees the space field.
 *****************************************************************************/

void space_free() {
	log_debug_str("Freeing the space!");

	for (int row = 0; row < __dim_space.row; row++) {

		//
		// Free the hex fields
		//
		for (int col = 0; col < __dim_space.col; col++) {
			hex_field_free(__space[row][col]);
		}

		free(__space[row]);
	}

	free(__space);
}

/******************************************************************************
 * The function initializes a 4x4 hex field with empty chars or stars:
 *
 *  ##
 * ####
 * ####
 *  ##
 *****************************************************************************/

static void space_init_hex_field(s_hex_point **hex_block) {

#ifdef DEBUG
	int n_stars = 0;
#endif

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// The 4 corners of the array are not necessary for the hex field.
			//
			if (row % 3 == 0 && col % 3 == 0) {
				hex_block[row][col].chr = W_NULL;
				hex_block[row][col].fg = -1;
			}

			//
			// We use a random distribution for the stars.
			//
			else if (rand() % RAND_START == 0) {
				hex_block[row][col].chr = W_STAR;
				hex_block[row][col].fg = COLOR_WHITE;
#ifdef DEBUG
				n_stars++;
#endif
			}

			//
			// The rest of the chars are empty.
			//
			else {
				hex_block[row][col].chr = W_EMPTY;
				hex_block[row][col].fg = COLOR_WHITE;
			}
		}
	}
	log_debug("stars: %d from: 12", n_stars);
}

/******************************************************************************
 * The function initializes the space field with empty chars or dots (which
 * represent stars).
 *****************************************************************************/

static void space_init_hex_fields(s_hex_point ****space, const s_point *dim) {

	for (int row = 0; row < dim->row; row++) {
		for (int col = 0; col < dim->col; col++) {
			space_init_hex_field(space[row][col]);
		}
	}
}

/******************************************************************************
 * The function initializes the background space.
 *****************************************************************************/

void space_init(s_point *dim_hex) {

	log_debug_str("Init space");

	//
	// Store the dimensions
	//
	s_point_set(&__dim_space, dim_hex->row, dim_hex->col);

	//
	// Allocate the array
	//
	__space = space_alloc(dim_hex);

	//
	// Create stars
	//
	space_init_hex_fields(__space, dim_hex);

	//
	// Initialize the colors
	//
	space_init_colors();
}

/******************************************************************************
 * The function returns a hex field by its index.
 *****************************************************************************/

s_hex_point** space_get_hex_field(const s_point *hex) {
	return __space[hex->row][hex->col];
}