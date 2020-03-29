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

#include "hg_obj_area.h"
#include "hg_common.h"

/******************************************************************************
 * The definition of the object area array and the dimension of the object area
 * array.
 *****************************************************************************/

static s_point _dim_space;

s_object **_obj_area;

/******************************************************************************
 * The function allocates the array for the object area.
 *****************************************************************************/

static s_object** obj_area_alloc(const s_point *dim) {

	log_debug("Creating object area with: %d/%d", dim->row, dim->col);

	s_object **obj_area = xmalloc(sizeof(s_object*) * dim->row);

	for (int row = 0; row < dim->row; row++) {
		obj_area[row] = xmalloc(sizeof(s_object) * dim->col);
	}

	return obj_area;
}

/******************************************************************************
 * The function frees the object area array.
 *****************************************************************************/

void obj_area_free() {
	log_debug_str("Freeing the object area!");

	for (int row = 0; row < _dim_space.row; row++) {
		free(_obj_area[row]);
	}

	free(_obj_area);
}

/******************************************************************************
 * The function initializes the object area with empty objects.
 *****************************************************************************/

static void obj_area_init_empty(s_object **obj_area) {

	for (int row = 0; row < _dim_space.row; row++) {
		for (int col = 0; col < _dim_space.col; col++) {

			obj_area[row][col].obj = OBJ_NONE;
		}
	}
}

/******************************************************************************
 * The function initializes the object area.
 *****************************************************************************/

void obj_area_init(const s_point *dim_hex) {

	log_debug_str("Init object area.");

	//
	// Store the dimensions
	//
	s_point_set(&_dim_space, dim_hex->row, dim_hex->col);

	//
	// Allocate the array
	//
	_obj_area = obj_area_alloc(dim_hex);

	//
	// Initialize the object area with empty objects.
	//
	obj_area_init_empty(_obj_area);
}
