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

s_object **_obj_area = NULL;

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
	_obj_area = NULL;
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

			s_point_set(&object->pos, idx.row, idx.col);

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
				// Get the coordinates of the neighbour in that direction.
				//
				obj_area_goto(&idx, dir, &neighbour);

				//
				// Ensure that the neighbour coordinates are valid.
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
 * the target point to the adjacent field in the given direction.
 *****************************************************************************/

void obj_area_goto(const s_point *from, const e_dir dir, s_point *to) {

	switch (dir) {

	case DIR_NN:
		to->row = from->row - 1;
		to->col = from->col;
		break;

	case DIR_NE:
		to->row = from->row % 2 == 0 ? from->row - 1 + from->col % 2 : from->row - 1 + from->col % 2;
		to->col = from->col + 1;
		break;

	case DIR_SE:
		to->row = from->row % 2 == 0 ? from->row + from->col % 2 : from->row + from->col % 2;
		to->col = from->col + 1;
		break;

	case DIR_SS:
		to->row = from->row + 1;
		to->col = from->col;
		break;

	case DIR_SW:
		to->row = from->row % 2 == 0 ? from->row + from->col % 2 : from->row + from->col % 2;
		to->col = from->col - 1;
		break;

	case DIR_NW:
		to->row = from->row % 2 == 0 ? from->row - 1 + from->col % 2 : from->row - 1 + from->col % 2;
		to->col = from->col - 1;
		break;

	default:
		log_exit("Unknown direction: %d", dir)
		;
	}
}

/******************************************************************************
 * The function checks if a ship can move to this positions in the object area.
 * This means the target must have a valid move marker.
 *
 * It is assumed that move markers are only set to valid locations. So checks
 * whether the target is a valid location for a move marker had taken place
 * upfront.
 *****************************************************************************/

bool obj_area_can_mv_to(const s_object *obj_to) {

	//
	// The target needs a marker.
	//
	if (obj_to->marker == NULL) {
		return false;
	}

	//
	// And the marker has to be a move marker.
	//
	if (obj_to->marker->type != MRK_TYPE_MOVE) {
		return false;
	}

	//
	// And the direction of the move marker must not be undefined, which means
	// highlighting the position of a ship.
	//
	if (obj_to->marker->marker_move->dir == DIR_UNDEF) {
		return false;
	}

	return true;
}

/******************************************************************************
 * The function moves a ship from one position to an other. The result is not
 * printed.
 *****************************************************************************/

void obj_area_mv_ship(s_object *obj_from, s_object *obj_to, const e_dir dir) {

	//
	// Get the source and ensure that this is a ship.
	//
	if (obj_from->obj != OBJ_SHIP) {
		log_exit("Source is not a ship: %d/%d", obj_from->pos.row, obj_from->pos.col);
	}

	//
	// Get the target and ensure that it is empty. (Maybe in future it will be
	// possible to fly through an asteroid field.)
	//
	if (obj_to->obj != OBJ_NONE) {
		log_exit("Target is not empty: %d/%d", obj_to->pos.row, obj_to->pos.col);
	}

	//
	// Copy the ship instance to the target.
	//
	obj_to->obj = obj_from->obj;
	obj_to->ship_inst = obj_from->ship_inst;

	//
	// Update the ship direction.
	//
	obj_to->ship_inst->dir = dir;

	//
	// Remove the ship instance from the source.
	//
	obj_from->obj = OBJ_NONE;
	obj_from->ship_inst = NULL;
}

/******************************************************************************
 * The function sets a move marker to an object. The main part of the function
 * is validation. Setting a marker requires that there is no marker.
 *
 * You can mark a ship position with a move marker that has no direction or you
 * can mark an empty object with a move marker that has a direction.
 *****************************************************************************/

s_object* obj_area_set_mv_marker(s_object *obj, const e_dir dir) {

	//
	// Ensure that we do not overwrite a marker.
	//
	if (obj->marker != NULL) {
		log_exit("Object %d/%d already has a marker!", obj->pos.row, obj->pos.col);
	}

	//
	// Ensure that a ship has not a marker with a direction.
	//
	if (dir != DIR_UNDEF && obj->obj == OBJ_SHIP) {
		log_exit("Object is a ship and dir is: %s", e_dir_str(dir));

	}

	//
	// A marker with no direction is only possible for a ship.
	//
	else if (dir == DIR_UNDEF && obj->obj != OBJ_SHIP) {
		log_exit("Object is a ship and dir is: %s", e_dir_str(dir));
	}

	//
	// If the target is not null, we can set the move marker.
	//
	obj->marker = s_marker_get_move_marker(MRK_TYPE_MOVE, dir);

	//
	// Return the result object from the area.
	//
	return obj;
}

/******************************************************************************
 * The function is used to set move markers for a ship. It is called with the
 * ship object and a path. The path is a string, where each character
 * represents a relative direction:
 *
 * c: go forward
 * l: move left and go forward
 * r: move right and go forward.
 *****************************************************************************/

s_object* obj_area_set_mv_marker_path(s_object *obj_from, const char *mv_path) {

	log_debug("Object: %d/%d move with path: %s", obj_from->pos.row, obj_from->pos.col, mv_path);

	//
	// It is necessary that the object area has a ship at the initial position.
	//
	if (obj_from->obj != OBJ_SHIP) {
		log_exit_str("Object is not a ship!");
	}

	s_object *obj_to = obj_from;
	e_dir dir = obj_from->ship_inst->dir;

	//
	// Move the pointer along the path.
	//
	for (const char *ptr = mv_path; *ptr != '\0'; ptr++) {

		//
		// Update the direction depending on the path character.
		//
		dir = e_dir_mv(dir, *ptr);

		//
		// Go to the neighbor in that direction.
		//
		obj_to = obj_to->neighbour[dir];

		//
		// If the pointer is null, we are outside the object area.
		//
		if (obj_to == NULL) {
			log_debug_str("Object to is null!");
			return NULL;
		}
	}

	//
	// At this point we reached the target position and have to check whether
	// it is occupied.
	//
	if (obj_to->obj != OBJ_NONE) {
		log_debug("Target is occupied: %d/%d", obj_to->pos.row, obj_to->pos.col);
		return NULL;
	}

	//
	// Return the result object from the area.
	//
	return obj_area_set_mv_marker(obj_to, dir);
}
