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

#include <stdbool.h>

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
	s_point idx, neighbour;
	s_object *object;

	for (idx.row = 0; idx.row < _dim_space.row; idx.row++) {
		for (idx.col = 0; idx.col < _dim_space.col; idx.col++) {

			object = &obj_area[idx.row][idx.col];

			//
			// The object type is none
			//
			object->obj = OBJ_NONE;

			object->marker = NULL;

			//
			// Iterate over the directions to find the neighbours.
			//
			for (int dir = 0; dir < 6; dir++) {

				//
				// Get the neighbour in that direction.
				//
				obj_area_goto(&idx, dir, &neighbour);

				//
				// Ensure that the neighbour is valid.
				//
				if (s_point_inside(&_dim_space, &neighbour)) {
					object->neighbour[dir] = &obj_area[neighbour.row][neighbour.col];
				} else {
					object->neighbour[dir] = NULL;
				}
			}
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

/******************************************************************************
 * The function is called with a current position and a direction. It updates
 * the "to" point to the adjacent field in the given direction.
 *****************************************************************************/

void obj_area_goto(const s_point *from, const e_dir dir, s_point *to) {

	switch (dir) {

	case DIR_NN:
		to->row = from->row - 1;
		to->col = from->col;
		break;

	case DIR_NE:
		to->row = from->row % 2 == 0 ? from->row - 1 : from->row;
		to->col = from->col + 1;
		break;

	case DIR_SE:
		to->row = from->row % 2 == 0 ? from->row : from->row + 1;
		to->col = from->col + 1;
		break;

	case DIR_SS:
		to->row = from->row + 1;
		to->col = from->col;
		break;

	case DIR_SW:
		to->row = from->row % 2 == 0 ? from->row : from->row + 1;
		to->col = from->col - 1;
		break;

	case DIR_NW:
		to->row = from->row % 2 == 0 ? from->row - 1 : from->row;
		to->col = from->col - 1;
		break;

	default:
		log_exit("Unknown direction: %d", dir)
		;
	}
}

/******************************************************************************
 * The function moves a ship from one position to an other. The result is not
 * printed. There are valid cases where the ship is not moved. In this case the
 * function returns false, otherwise true.
 *****************************************************************************/
// TODO: use neighbours
bool obj_area_mv_ship(const s_point *point_from, s_point *point_to, const e_dir dir) {

	//
	// Ensure that the target is inside the borders.
	//
	if (!s_point_inside(&_dim_space, point_to)) {
		log_debug("Target outside: %d/%d", point_to->row, point_to->col);
		return false;
	}

	//
	// If the source and the target positions are the same, there is nothing to
	// do. Turning around at the same position is not allowed.
	//
	if (s_point_same(point_from, point_to)) {
		log_debug("Same points: %d/%d", point_from->row, point_from->col);
		return false;
	}

	//
	// Get the source and ensure that this is a ship.
	//
	s_object *obj_from = obj_area_get(point_from->row, point_from->col);
	if (obj_from->obj != OBJ_SHIP) {
		log_exit("Source is not a ship: %d/%d", point_from->row, point_from->col);
	}

	//
	// Get the target and ensure that it is empty. (Maybe in future it will be
	// possible to fly through an asteroid field.)
	//
	s_object *obj_to = obj_area_get(point_to->row, point_to->col);
	if (obj_to->obj != OBJ_NONE) {
		log_exit("Target is not empty: %d/%d", point_to->row, point_to->col);
	}

	//
	// Update the object type in the source and target.
	//
	obj_to->obj = obj_from->obj;
	obj_from->obj = OBJ_NONE;

	//
	// Move the ship instance and update the direction.
	//
	obj_to->ship_inst = obj_from->ship_inst;
	obj_to->ship_inst->dir = dir;

	//
	// Return true to indicate that the ship was moved.
	//
	return true;
}
