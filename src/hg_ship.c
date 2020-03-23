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

#define Q_UDEF NULL

#define Q_LRLR L"\x2588"

#define Q_XRLR L"\x259F"
#define Q_LXLR L"\x2599"

#define Q_LRLX L"\x259B"
#define Q_LRXR L"\x259C"

short blue_dark;
short blue_light;
short yellow;

/******************************************************************************
 * The definition of the ship templates.
 *****************************************************************************/

s_hex_point **_ship_field[DIR_NUM];

/******************************************************************************
 * The function allocates the memory for the ship field.
 *****************************************************************************/

static void ship_field_alloc() {

	for (int i = 0; i < DIR_NUM; i++) {
		_ship_field[i] = hex_field_alloc();
	}
}

/******************************************************************************
 * The function frees the 6 directions of a ship.
 *****************************************************************************/

void ship_field_free() {

	for (int i = 0; i < DIR_NUM; i++) {
		hex_field_free(_ship_field[i]);
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void ship_field_corners(s_hex_point **ship_field) {
	log_debug_str("Setting corners!");

	hex_point_set_undef(ship_field[0][0]);
	hex_point_set_undef(ship_field[0][3]);
	hex_point_set_undef(ship_field[3][0]);
	hex_point_set_undef(ship_field[3][3]);
}

/******************************************************************************
 *
 *****************************************************************************/

void ship_field_init() {
	log_debug_str("Init ships!");

	ship_field_alloc();

	blue_dark = col_color_create(300, 300, 700);
	blue_light = col_color_create(400, 400, 700);

	yellow = col_color_create(900, 800, 0);

	s_hex_point **ship;

	// Direction NN
	ship = _ship_field[DIR_NN];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[1][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship[1][3], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_LRLX, blue_dark, yellow);
	hex_point_set(ship[2][1], Q_LRXR, blue_dark, yellow);
	hex_point_set(ship[2][2], Q_LRLX, blue_light, yellow);
	hex_point_set(ship[2][3], Q_LRXR, blue_light, yellow);

	hex_point_set_undef(ship[3][1]);
	hex_point_set_undef(ship[3][2]);

	// Direction NN
	ship = _ship_field[DIR_NE];
	ship_field_corners(ship);

	hex_point_set_undef(ship[0][1]);
	hex_point_set_undef(ship[0][2]);

	hex_point_set(ship[1][0], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[1][2], Q_LRLX, blue_dark, blue_light);
	hex_point_set(ship[1][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_LRXR, yellow, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLX, blue_dark, blue_light);
	hex_point_set(ship[2][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship[2][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_LRXR, yellow, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_LRLX, blue_light, COLOR_UNDEF);

	// Direction NN
	ship = _ship_field[DIR_SE];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_XRLR, yellow, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_XRLR, yellow, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LXLR, blue_dark, blue_light);
	hex_point_set(ship[1][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship[1][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[2][2], Q_LXLR, blue_dark, blue_light);
	hex_point_set(ship[2][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set_undef(ship[3][1]);
	hex_point_set_undef(ship[3][2]);

	// Direction SS
	ship = _ship_field[DIR_SS];
	ship_field_corners(ship);

	hex_point_set_undef(ship[0][1]);
	hex_point_set_undef(ship[0][2]);

	hex_point_set(ship[1][0], Q_LXLR, blue_dark, yellow);
	hex_point_set(ship[1][1], Q_XRLR, blue_dark, yellow);
	hex_point_set(ship[1][2], Q_LXLR, blue_light, yellow);
	hex_point_set(ship[1][3], Q_XRLR, blue_light, yellow);

	hex_point_set(ship[2][0], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[2][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship[2][3], Q_LRLX, blue_light, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_LRLX, blue_light, COLOR_UNDEF);

	// Direction NN
	ship = _ship_field[DIR_SW];
	ship_field_corners(ship);

	hex_point_set(ship[0][1], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[0][2], Q_LXLR, yellow, COLOR_UNDEF);

	hex_point_set(ship[1][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[1][2], Q_XRLR, blue_light, blue_dark);
	hex_point_set(ship[1][3], Q_LXLR, yellow, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_XRLR, blue_light, blue_dark);
	hex_point_set(ship[2][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship[2][3], Q_LRLX, blue_light, COLOR_UNDEF);

	hex_point_set_undef(ship[3][1]);
	hex_point_set_undef(ship[3][2]);

	// Direction NN
	ship = _ship_field[DIR_NW];
	ship_field_corners(ship);

	hex_point_set_undef(ship[0][1]);
	hex_point_set_undef(ship[0][2]);

	hex_point_set(ship[1][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[1][1], Q_LRXR, blue_light, blue_dark);
	hex_point_set(ship[1][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship[1][3], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship[2][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship[2][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[2][2], Q_LRXR, blue_light, blue_dark);
	hex_point_set(ship[2][3], Q_LRLX, yellow, COLOR_UNDEF);

	hex_point_set(ship[3][1], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship[3][2], Q_LRLX, yellow, COLOR_UNDEF);

	log_debug_str("Ships ready to fly!");
}
