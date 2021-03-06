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

#ifndef INC_HG_SHIP_H_
#define INC_HG_SHIP_H_

#include "hg_hex.h"
#include "hg_dir.h"

/******************************************************************************
 * The enum value is the id of the ship type.
 *****************************************************************************/

typedef enum {

	SHIP_TYPE_NORMAL

} e_ship_type;

/******************************************************************************
 * The definition of a ship type. The different types differ in the colors and
 * the allowed movements, which are represented by paths.
 *****************************************************************************/

typedef struct {

	//
	// The color definitions of the ship type.
	//
	short color[3];

	//
	// The paths that are used for the move marker.
	//
	char **paths;

} s_ship_type;

/******************************************************************************
 * The definition of an instance of a ship.
 *****************************************************************************/

typedef struct {

	//
	// The direction of the ship.
	//
	e_dir dir;

	//
	// The type of the ship
	//
	s_ship_type *ship_type;

} s_ship_inst;

/******************************************************************************
 * Definition of the functions.
 *****************************************************************************/

void ship_field_init();

void ship_field_free();

void ship_get_hex_field(const s_ship_type *ship_type, const e_dir dir, s_hex_field *ship_field);

s_ship_inst* s_ship_inst_create(const e_ship_type ship_type, const e_dir dir);

#endif /* INC_HG_SHIP_H_ */
