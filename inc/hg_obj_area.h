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

#ifndef INC_HG_OBJ_AREA_H_
#define INC_HG_OBJ_AREA_H_

#include "hg_ship.h"
#include "hg_marker.h"

/******************************************************************************
 * The enum defines the possible object types.
 *****************************************************************************/

typedef enum {

	OBJ_NONE,
	OBJ_SHIP

} e_object;

/******************************************************************************
 * The struct defines that possible obejct types.
 *****************************************************************************/

typedef struct s_object s_object;

struct s_object {

	//
	// The position of the object. This is necessary for printing.
	//
	s_point pos;

	//
	// The type of the object.
	//
	e_object obj;

	//
	// A union with pointers for the different object types.
	//
	union {
		s_ship_inst *ship_inst;
	};

	s_object *neighbour[6];

	s_marker *marker;
};

/******************************************************************************
 * Macros to access the object area.
 *****************************************************************************/

extern s_object **_obj_area;

#define s_object_set_ship_at(r,c,i) _obj_area[r][c].obj = OBJ_SHIP; _obj_area[r][c].ship_inst = (i)

#define obj_area_get(r,c) &_obj_area[r][c]

#define obj_area_add_marker(r,c,m) _obj_area[r][c].marker = (m)

/******************************************************************************
 * The definitions of the functions.
 *****************************************************************************/

void obj_area_init(const s_point *dim_hex);

void obj_area_free();

void obj_area_goto(const s_point *from, const e_dir dir, s_point *to);

bool obj_area_can_mv_to(const s_object *obj_to);

void obj_area_mv_ship(s_object *obj_from, s_object *obj_to, const e_dir dir);

s_object* obj_area_set_mv_marker(s_object *obj_from, char *path);

#endif /* INC_HG_OBJ_AREA_H_ */
