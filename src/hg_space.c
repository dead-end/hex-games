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

static s_point _dim_space;

static s_hex_field **_space;

short colors_normal[3];
short colors_select[3];

/******************************************************************************
 * The function initializes the colors.
 *****************************************************************************/

static void space_init_colors() {
	log_debug_str("Init the space colors and color pairs!");

	//
	// The shadings of the color for the normal state
	//
	colors_normal[0] = col_color_create(50, 50, 50);
	colors_normal[1] = col_color_create(80, 80, 80);
	colors_normal[2] = col_color_create(110, 110, 110);

	//
	// The shadings of the color for the selected state
	//
	colors_select[0] = col_color_create(250, 150, 150);
	colors_select[1] = col_color_create(280, 180, 180);
	colors_select[2] = col_color_create(310, 210, 210);

	cp_color_pair_add(COLOR_WHITE, colors_normal[0]);
	cp_color_pair_add(COLOR_WHITE, colors_normal[1]);
	cp_color_pair_add(COLOR_WHITE, colors_normal[2]);

	cp_color_pair_add(COLOR_WHITE, colors_select[0]);
	cp_color_pair_add(COLOR_WHITE, colors_select[1]);
	cp_color_pair_add(COLOR_WHITE, colors_select[2]);

	cp_color_pair_sort();
}

/******************************************************************************
 * The macro definition to get the shading of the background color.
 *****************************************************************************/

#define space_get_bg_color_idx(r,c) ((r) + 2 * ((c) % 2)) % 3

/******************************************************************************
 * The function returns the background color of the space. This depends on the
 * state of the hex field and the row / col index, which defines the shading of
 * the requested color.
 *****************************************************************************/

short space_get_bg_color(const int row, const int col, const e_state state) {
	short result;

	//
	// Get the index of the shading (0,1,2)
	//
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

s_hex_field** space_alloc(const s_point *dim) {

	log_debug("Creating space with: %d/%d hex fields", dim->row, dim->col);

	s_hex_field **space = xmalloc(sizeof(s_hex_field*) * dim->row);

	for (int row = 0; row < dim->row; row++) {
		space[row] = xmalloc(sizeof(s_hex_field) * dim->col);
	}

	return space;
}

/******************************************************************************
 * The function frees the space field.
 *****************************************************************************/

void space_free() {
	log_debug_str("Freeing the space!");

	for (int row = 0; row < _dim_space.row; row++) {
		free(_space[row]);
	}

	free(_space);
}

/******************************************************************************
 * The function initializes a 4x4 hex field with empty chars or stars:
 *
 *  ##
 * ####
 * ####
 *  ##
 *****************************************************************************/

static void space_hex_field_init(s_hex_field *hex_field) {

#ifdef DEBUG
	int n_stars = 0;
#endif

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// The background color is defined by the state of the hex field.
			//
			hex_field->point[row][col].bg = COLOR_UNDEF;

			//
			// The 4 corners of the array are not necessary for the hex field.
			//
			if (hex_field_is_corner(row, col)) {
				hex_field->point[row][col].chr = W_NULL;
				hex_field->point[row][col].fg = COLOR_UNDEF;
			}

			//
			// We use a random distribution for the stars.
			//
			else if (rand() % RAND_START == 0) {
				hex_field->point[row][col].chr = W_STAR;
				hex_field->point[row][col].fg = COLOR_WHITE;
#ifdef DEBUG
				n_stars++;
#endif
			}

			//
			// The rest of the chars are empty.
			//
			else {
				hex_field->point[row][col].chr = W_EMPTY;
				hex_field->point[row][col].fg = COLOR_WHITE;
			}
		}
	}
	log_debug("stars: %d from: 12", n_stars);
}

/******************************************************************************
 * The function initializes the space field with empty chars or dots (which
 * represent stars).
 *****************************************************************************/

static void space_hex_fields_init(s_hex_field **space, const s_point *dim) {

	for (int row = 0; row < dim->row; row++) {
		for (int col = 0; col < dim->col; col++) {
			space_hex_field_init(&space[row][col]);
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
	s_point_set(&_dim_space, dim_hex->row, dim_hex->col);

	//
	// Allocate the array
	//
	_space = space_alloc(dim_hex);

	//
	// Create stars
	//
	space_hex_fields_init(_space, dim_hex);

	//
	// Initialize the colors
	//
	space_init_colors();
}

/******************************************************************************
 * The function copies the content of the hex field from the space array to the
 * hex field, given by the parameter. The background color is adjusted
 * depending on the state of the space hex field.
 *****************************************************************************/

void space_get_hex_field(const s_point *hex_idx, const e_state state, s_hex_field *space_field) {

#ifdef DEBUG

	//
	// Ensure that the index is inside the allowed ranges.
	//
	if (hex_idx->row < 0 || hex_idx->row >= _dim_space.row || hex_idx->col < 0 || hex_idx->col >= _dim_space.col) {
		log_debug("hex field index out of range: %d/%d", hex_idx->row, hex_idx->col);
	}
#endif

	//
	// The hex field from the space array, which is the template.
	//
	const s_hex_field *space_tmpl = &_space[hex_idx->row][hex_idx->col];

	//
	// The background color, which depends on the state of the space hex field
	// as well as the shading.
	//
	const short color_space_bg = space_get_bg_color(hex_idx->row, hex_idx->col, state);

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			space_field->point[row][col].chr = space_tmpl->point[row][col].chr;
			space_field->point[row][col].fg = space_tmpl->point[row][col].fg;
			space_field->point[row][col].bg = color_space_bg;
		}
	}
}
