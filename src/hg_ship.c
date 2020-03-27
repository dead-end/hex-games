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

#include "hg_hex.h"
#include "hg_ship.h"

/******************************************************************************
 * The definition the characters that are used for the ships.
 *****************************************************************************/

//
// Undefined character
//
#define Q_UDEF NULL

//
// The full block
//
#define Q_LRLR L"\x2588"

//
// A block that consists of 4 rectangles. One of them is missing, which is
// denoted as X.
//
// LR
// LR
//
#define Q_XRLR L"\x259F"
#define Q_LXLR L"\x2599"
#define Q_LRLX L"\x259B"
#define Q_LRXR L"\x259C"

/******************************************************************************
 * The definition of the template colors. They have to be replaced with the
 * colors of the ship type. So the value is negative, to ensure an error if the
 * mapping failed.
 *****************************************************************************/

#define C_UNDEF COLOR_UNDEF

#define C_DARK -2

#define C_LIGHT -3

#define C_YELLOW -4

/******************************************************************************
 * The definition of the ship templates.
 *****************************************************************************/

s_hex_field _ship_field_templ[DIR_NUM];

/******************************************************************************
 * The function initializes the ship types. Currently we have only one ship
 * type and initializing means allocating colors.
 *****************************************************************************/

s_ship_type _ship_type[1];

static void ship_type_init() {

	_ship_type[SHIP_TYPE_NORMAL].c_dark = col_color_create(300, 300, 700);
	_ship_type[SHIP_TYPE_NORMAL].c_light = col_color_create(400, 400, 700);
	_ship_type[SHIP_TYPE_NORMAL].c_yellow = col_color_create(900, 800, 0);
}

/******************************************************************************
 * The function returns the s_ship_type by its id (which is the enum ship
 * type).
 *****************************************************************************/

s_ship_type* ship_type_get(e_ship_type ship_type) {

	return &_ship_type[ship_type];
}

/******************************************************************************
 * The function maps the color from the template to color defined for the ship
 * type.
 *****************************************************************************/

static short ship_translate(const short color_temp, const s_ship_type *ship_type) {

	switch (color_temp) {

	case C_UNDEF:
		return C_UNDEF;

	case C_DARK:
		return ship_type->c_dark;

	case C_LIGHT:
		return ship_type->c_light;

	case C_YELLOW:
		return ship_type->c_yellow;

	default:
		log_exit("Unknown template color: %d", color_temp)
		;
	}
}

/******************************************************************************
 * The function copies the s_hex_field from the template to the given
 * s_hex_field. The colors from the template are mapped to the colors from the
 * s_ship_type.
 *****************************************************************************/

void ship_get_hex_field(const s_ship_type *ship_type, const e_dir dir, s_hex_field *ship_field) {
	s_hex_point *hp_templ, *hp_ship;

	//
	// Select the template for the given direction.
	//
	s_hex_field *hf_templ = &_ship_field_templ[dir];

	for (int row = 0; row < HEX_SIZE; row++) {
		for (int col = 0; col < HEX_SIZE; col++) {

			//
			// Store the s_hex_point's
			//
			hp_templ = &hf_templ->point[row][col];
			hp_ship = &ship_field->point[row][col];

			//
			// Set the s_hex_point for the ship, with the mapped colors from
			// the template.
			//
			hp_ship->chr = hp_templ->chr;
			hp_ship->fg = ship_translate(hp_templ->fg, ship_type);
			hp_ship->bg = ship_translate(hp_templ->bg, ship_type);
		}
	}
}

/******************************************************************************
 * The function initializes the ship templates.
 *****************************************************************************/

static void ship_field_init_templ() {
	s_hex_field *ship;

	//
	// Direction: Noth / North
	//
	ship = &_ship_field_templ[DIR_NN];
	hex_field_set_corners(ship);

	hex_point_set(ship->point[0][1], Q_XRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[0][2], Q_LXLR, C_LIGHT, C_UNDEF);

	hex_point_set(ship->point[1][0], Q_XRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[1][2], Q_LRLR, C_LIGHT, C_UNDEF);
	hex_point_set(ship->point[1][3], Q_LXLR, C_LIGHT, C_UNDEF);

	hex_point_set(ship->point[2][0], Q_LRLX, C_DARK, C_YELLOW);
	hex_point_set(ship->point[2][1], Q_LRXR, C_DARK, C_YELLOW);
	hex_point_set(ship->point[2][2], Q_LRLX, C_LIGHT, C_YELLOW);
	hex_point_set(ship->point[2][3], Q_LRXR, C_LIGHT, C_YELLOW);

	hex_point_set_undef(ship->point[3][1]);
	hex_point_set_undef(ship->point[3][2]);

	//
	// Direction: North / East
	//
	ship = &_ship_field_templ[DIR_NE];
	hex_field_set_corners(ship);

	hex_point_set_undef(ship->point[0][1]);
	hex_point_set_undef(ship->point[0][2]);

	hex_point_set(ship->point[1][0], Q_XRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[1][2], Q_LRLX, C_DARK, C_LIGHT);
	hex_point_set(ship->point[1][3], Q_UDEF, C_UNDEF, C_UNDEF);

	hex_point_set(ship->point[2][0], Q_LRXR, C_YELLOW, C_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLX, C_DARK, C_LIGHT);
	hex_point_set(ship->point[2][2], Q_LRLR, C_LIGHT, C_UNDEF);
	hex_point_set(ship->point[2][3], Q_UDEF, C_UNDEF, C_UNDEF);

	hex_point_set(ship->point[3][1], Q_LRXR, C_YELLOW, C_UNDEF);
	hex_point_set(ship->point[3][2], Q_LRLX, C_LIGHT, C_UNDEF);

	//
	// Direction: South / East
	//
	ship = &_ship_field_templ[DIR_SE];
	hex_field_set_corners(ship);

	hex_point_set(ship->point[0][1], Q_XRLR, C_YELLOW, C_UNDEF);
	hex_point_set(ship->point[0][2], Q_LXLR, C_LIGHT, C_UNDEF);

	hex_point_set(ship->point[1][0], Q_XRLR, C_YELLOW, C_UNDEF);
	hex_point_set(ship->point[1][1], Q_LXLR, C_DARK, C_LIGHT);
	hex_point_set(ship->point[1][2], Q_LRLR, C_LIGHT, C_UNDEF);
	hex_point_set(ship->point[1][3], Q_UDEF, C_UNDEF, C_UNDEF);

	hex_point_set(ship->point[2][0], Q_LRXR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[2][2], Q_LXLR, C_DARK, C_LIGHT);
	hex_point_set(ship->point[2][3], Q_UDEF, C_UNDEF, C_UNDEF);

	hex_point_set_undef(ship->point[3][1]);
	hex_point_set_undef(ship->point[3][2]);

	//
	// Direction: South / South
	//
	ship = &_ship_field_templ[DIR_SS];
	hex_field_set_corners(ship);

	hex_point_set_undef(ship->point[0][1]);
	hex_point_set_undef(ship->point[0][2]);

	hex_point_set(ship->point[1][0], Q_LXLR, C_DARK, C_YELLOW);
	hex_point_set(ship->point[1][1], Q_XRLR, C_DARK, C_YELLOW);
	hex_point_set(ship->point[1][2], Q_LXLR, C_LIGHT, C_YELLOW);
	hex_point_set(ship->point[1][3], Q_XRLR, C_LIGHT, C_YELLOW);

	hex_point_set(ship->point[2][0], Q_LRXR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[2][2], Q_LRLR, C_LIGHT, C_UNDEF);
	hex_point_set(ship->point[2][3], Q_LRLX, C_LIGHT, C_UNDEF);

	hex_point_set(ship->point[3][1], Q_LRXR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[3][2], Q_LRLX, C_LIGHT, C_UNDEF);

	//
	// Direction: South / West
	//
	ship = &_ship_field_templ[DIR_SW];
	hex_field_set_corners(ship);

	hex_point_set(ship->point[0][1], Q_XRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[0][2], Q_LXLR, C_YELLOW, C_UNDEF);

	hex_point_set(ship->point[1][0], Q_UDEF, C_UNDEF, C_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[1][2], Q_XRLR, C_LIGHT, C_DARK);
	hex_point_set(ship->point[1][3], Q_LXLR, C_YELLOW, C_UNDEF);

	hex_point_set(ship->point[2][0], Q_UDEF, C_UNDEF, C_UNDEF);
	hex_point_set(ship->point[2][1], Q_XRLR, C_LIGHT, C_DARK);
	hex_point_set(ship->point[2][2], Q_LRLR, C_LIGHT, C_UNDEF);
	hex_point_set(ship->point[2][3], Q_LRLX, C_LIGHT, C_UNDEF);

	hex_point_set_undef(ship->point[3][1]);
	hex_point_set_undef(ship->point[3][2]);

	//
	// Direction: North / West
	//
	ship = &_ship_field_templ[DIR_NW];
	hex_field_set_corners(ship);

	hex_point_set_undef(ship->point[0][1]);
	hex_point_set_undef(ship->point[0][2]);

	hex_point_set(ship->point[1][0], Q_UDEF, C_UNDEF, C_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRXR, C_LIGHT, C_DARK);
	hex_point_set(ship->point[1][2], Q_LRLR, C_LIGHT, C_UNDEF);
	hex_point_set(ship->point[1][3], Q_LXLR, C_LIGHT, C_UNDEF);

	hex_point_set(ship->point[2][0], Q_UDEF, C_UNDEF, C_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[2][2], Q_LRXR, C_LIGHT, C_DARK);
	hex_point_set(ship->point[2][3], Q_LRLX, C_YELLOW, C_UNDEF);

	hex_point_set(ship->point[3][1], Q_LRXR, C_DARK, C_UNDEF);
	hex_point_set(ship->point[3][2], Q_LRLX, C_YELLOW, C_UNDEF);
}

/******************************************************************************
 * The function initializes the ship types and templates.
 *****************************************************************************/

void ship_field_init() {
	log_debug_str("Init ships!");

	//
	// Initialize the ship types.
	//
	ship_type_init();

	//
	// Initialize the ship templates
	//
	ship_field_init_templ();

	log_debug_str("Ships ready to fly!");
}
