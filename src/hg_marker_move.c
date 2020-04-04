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

#include "hg_common.h"
#include "hg_color.h"
#include "hg_color_pair.h"
#include "hg_marker_move.h"

/******************************************************************************
 * The definition of the move marker colors. We have 3 colors for the shading
 * and a normal and highlighted color.
 *****************************************************************************/

#define NUM_SHADES 3

short _colors_normal[NUM_SHADES];

short _colors_highlight[NUM_SHADES];

/******************************************************************************
 * The definition of arrow characters for the move markers.
 *****************************************************************************/

#define MV_NN L"\x2B06"
#define MV_NE L"\x2B08"
#define MV_SE L"\x2B0A"
#define MV_SS L"\x2B07"
#define MV_SW L"\x2B0B"
#define MV_NW L"\x2B09"

wchar_t *_arrow[6];

/******************************************************************************
 * The function initializes the colors.
 *****************************************************************************/

void s_marker_move_init() {
	log_debug_str("Initialize arrows and colors / color pairs!");

	//
	// Initialize the arrow characters.
	//
	_arrow[DIR_NN] = MV_NN;
	_arrow[DIR_NE] = MV_NE;
	_arrow[DIR_SE] = MV_SE;
	_arrow[DIR_SS] = MV_SS;
	_arrow[DIR_SW] = MV_SW;
	_arrow[DIR_NW] = MV_NW;

	//
	// The shadings of the color for the normal state
	//
	_colors_normal[0] = col_color_create(150, 250, 150);
	_colors_normal[1] = col_color_create(180, 280, 180);
	_colors_normal[2] = col_color_create(210, 310, 210);

	cp_color_pair_add(COLOR_YELLOW, _colors_normal[0]);
	cp_color_pair_add(COLOR_YELLOW, _colors_normal[1]);
	cp_color_pair_add(COLOR_YELLOW, _colors_normal[2]);

	//
	// The shadings of the color for the highlighted state
	//
	_colors_highlight[0] = col_color_create(150, 450, 150);
	_colors_highlight[1] = col_color_create(180, 480, 180);
	_colors_highlight[2] = col_color_create(210, 510, 210);

	cp_color_pair_add(COLOR_YELLOW, _colors_highlight[0]);
	cp_color_pair_add(COLOR_YELLOW, _colors_highlight[1]);
	cp_color_pair_add(COLOR_YELLOW, _colors_highlight[2]);

	log_debug_str("Initialization done!");
}

/******************************************************************************
 * The function adds the move marker to the hex field. It changes the
 * background color and adds arrows to the field.
 *****************************************************************************/

void s_marker_move_to_field(const s_marker_move *marker, const int color_idx, s_hex_field *hex_field, const bool highlight) {

	//
	// Get the background color including the background shading and highlighting.
	//
	const short bg = highlight ? _colors_highlight[color_idx] : _colors_normal[color_idx];

	//
	// Set the background for the move marker
	//
	hex_field_set_bg(hex_field, bg);

	//
	// If the move marker has a foreground character
	//
	if (marker->dir != DIR_UNDEF) {
		hex_field->point[1][1].chr = _arrow[marker->dir];
		hex_field->point[1][1].fg = COLOR_YELLOW;

		hex_field->point[2][1].chr = _arrow[marker->dir];
		hex_field->point[2][1].fg = COLOR_YELLOW;
	}
}
