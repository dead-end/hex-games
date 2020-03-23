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

#define C_UNDEF COLOR_UNDEF
#define C_DARK C_UNDEF -1
#define C_LIGHT C_UNDEF -2
#define C_YELLOW C_UNDEF -3

/******************************************************************************
 * The definition of the ship templates.
 *****************************************************************************/

s_hex_field _ship_field_templ[DIR_NUM];

/******************************************************************************
 *
 *****************************************************************************/
// TODO: unused
s_ship_type ship_type_1;

void ship_type_init() {

	ship_type_1.c_dark = col_color_create(300, 300, 700);

	ship_type_1.c_light = col_color_create(400, 400, 700);

	ship_type_1.c_yellow = col_color_create(900, 800, 0);
}

///******************************************************************************
// *
// *****************************************************************************/
//
//// TODO: unused
//static short ship_translate(const s_ship_type *ship_type, short color_temp) {
//
//	switch (color_temp) {
//
//	case C_UNDEF:
//		return C_UNDEF;
//
//	case C_DARK:
//		return ship_type->c_dark;
//
//	case C_LIGHT:
//		return ship_type->c_light;
//
//	case C_YELLOW:
//		return ship_type->c_yellow;
//
//	default:
//		log_exit("Unknown template color: %d", color_temp)
//		;
//	}
//}

///******************************************************************************
// *
// *****************************************************************************/
//
//// TODO: unused
//void ship_get_hex_point(const s_ship_type *ship_type, const e_dir dir, const int row, const int col, s_hex_point *hex_point) {
//
//	s_hex_point ship_templ = _ship_field_templ[dir][row][col];
//
//	hex_point->chr = ship_templ.chr;
//
//	hex_point->fg = ship_translate(ship_type, ship_templ.fg);
//
//	hex_point->bg = ship_translate(ship_type, ship_templ.bg);
//}
//
///******************************************************************************
// *
// *****************************************************************************/
//
//// TODO: unused
//void ship_get_field(const s_ship_type *ship_type, const e_dir dir, s_hex_point **ship_field) {
//
//	for (int row = 0; row < HEX_SIZE; row++) {
//		for (int col = 0; col < HEX_SIZE; col++) {
//
//			s_hex_point ship_templ = _ship_field_templ[dir][row][col];
//
//			ship_field[row][col].chr = ship_templ.chr;
//
//			ship_field[row][col].fg = ship_translate(ship_type, ship_templ.fg);
//
//			ship_field[row][col].bg = ship_translate(ship_type, ship_templ.bg);
//		}
//	}
//}

/******************************************************************************
 *
 *****************************************************************************/

static void ship_field_corners(s_hex_field *ship_field) {
	log_debug_str("Setting corners!");

	hex_point_set_undef(ship_field->point[0][0]);
	hex_point_set_undef(ship_field->point[0][3]);
	hex_point_set_undef(ship_field->point[3][0]);
	hex_point_set_undef(ship_field->point[3][3]);
}

/******************************************************************************
 *
 *****************************************************************************/

void ship_field_init() {
	log_debug_str("Init ships!");

	//ship_field_alloc();

	blue_dark = col_color_create(300, 300, 700);
	blue_light = col_color_create(400, 400, 700);

	yellow = col_color_create(900, 800, 0);

	s_hex_field *ship;

	// Direction NN
	ship = &_ship_field_templ[DIR_NN];
	ship_field_corners(ship);

	hex_point_set(ship->point[0][1], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[0][2], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship->point[1][0], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[1][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship->point[1][3], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship->point[2][0], Q_LRLX, blue_dark, yellow);
	hex_point_set(ship->point[2][1], Q_LRXR, blue_dark, yellow);
	hex_point_set(ship->point[2][2], Q_LRLX, blue_light, yellow);
	hex_point_set(ship->point[2][3], Q_LRXR, blue_light, yellow);

	hex_point_set_undef(ship->point[3][1]);
	hex_point_set_undef(ship->point[3][2]);

	// Direction NN
	ship = &_ship_field_templ[DIR_NE];
	ship_field_corners(ship);

	hex_point_set_undef(ship->point[0][1]);
	hex_point_set_undef(ship->point[0][2]);

	hex_point_set(ship->point[1][0], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[1][2], Q_LRLX, blue_dark, blue_light);
	hex_point_set(ship->point[1][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship->point[2][0], Q_LRXR, yellow, COLOR_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLX, blue_dark, blue_light);
	hex_point_set(ship->point[2][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship->point[2][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship->point[3][1], Q_LRXR, yellow, COLOR_UNDEF);
	hex_point_set(ship->point[3][2], Q_LRLX, blue_light, COLOR_UNDEF);

	// Direction NN
	ship = &_ship_field_templ[DIR_SE];
	ship_field_corners(ship);

	hex_point_set(ship->point[0][1], Q_XRLR, yellow, COLOR_UNDEF);
	hex_point_set(ship->point[0][2], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship->point[1][0], Q_XRLR, yellow, COLOR_UNDEF);
	hex_point_set(ship->point[1][1], Q_LXLR, blue_dark, blue_light);
	hex_point_set(ship->point[1][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship->point[1][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set(ship->point[2][0], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[2][2], Q_LXLR, blue_dark, blue_light);
	hex_point_set(ship->point[2][3], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);

	hex_point_set_undef(ship->point[3][1]);
	hex_point_set_undef(ship->point[3][2]);

	// Direction SS
	ship = &_ship_field_templ[DIR_SS];
	ship_field_corners(ship);

	hex_point_set_undef(ship->point[0][1]);
	hex_point_set_undef(ship->point[0][2]);

	hex_point_set(ship->point[1][0], Q_LXLR, blue_dark, yellow);
	hex_point_set(ship->point[1][1], Q_XRLR, blue_dark, yellow);
	hex_point_set(ship->point[1][2], Q_LXLR, blue_light, yellow);
	hex_point_set(ship->point[1][3], Q_XRLR, blue_light, yellow);

	hex_point_set(ship->point[2][0], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[2][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship->point[2][3], Q_LRLX, blue_light, COLOR_UNDEF);

	hex_point_set(ship->point[3][1], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[3][2], Q_LRLX, blue_light, COLOR_UNDEF);

	// Direction NN
	ship = &_ship_field_templ[DIR_SW];
	ship_field_corners(ship);

	hex_point_set(ship->point[0][1], Q_XRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[0][2], Q_LXLR, yellow, COLOR_UNDEF);

	hex_point_set(ship->point[1][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[1][2], Q_XRLR, blue_light, blue_dark);
	hex_point_set(ship->point[1][3], Q_LXLR, yellow, COLOR_UNDEF);

	hex_point_set(ship->point[2][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship->point[2][1], Q_XRLR, blue_light, blue_dark);
	hex_point_set(ship->point[2][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship->point[2][3], Q_LRLX, blue_light, COLOR_UNDEF);

	hex_point_set_undef(ship->point[3][1]);
	hex_point_set_undef(ship->point[3][2]);

	// Direction NN
	ship = &_ship_field_templ[DIR_NW];
	ship_field_corners(ship);

	hex_point_set_undef(ship->point[0][1]);
	hex_point_set_undef(ship->point[0][2]);

	hex_point_set(ship->point[1][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship->point[1][1], Q_LRXR, blue_light, blue_dark);
	hex_point_set(ship->point[1][2], Q_LRLR, blue_light, COLOR_UNDEF);
	hex_point_set(ship->point[1][3], Q_LXLR, blue_light, COLOR_UNDEF);

	hex_point_set(ship->point[2][0], Q_UDEF, COLOR_UNDEF, COLOR_UNDEF);
	hex_point_set(ship->point[2][1], Q_LRLR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[2][2], Q_LRXR, blue_light, blue_dark);
	hex_point_set(ship->point[2][3], Q_LRLX, yellow, COLOR_UNDEF);

	hex_point_set(ship->point[3][1], Q_LRXR, blue_dark, COLOR_UNDEF);
	hex_point_set(ship->point[3][2], Q_LRLX, yellow, COLOR_UNDEF);

	log_debug_str("Ships ready to fly!");
}
