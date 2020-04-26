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
 * The definition of arrow characters for the move markers.
 *****************************************************************************/

#define MV_NN L"\x2B06"
#define MV_NE L"\x2B08"
#define MV_SE L"\x2B0A"
#define MV_SS L"\x2B07"
#define MV_SW L"\x2B0B"
#define MV_NW L"\x2B09"

static wchar_t *_arrow[6];

/******************************************************************************
 * The function initializes the array with the arrows characters.
 *****************************************************************************/

static void s_marker_move_init_arows() {
	log_debug_str("Initialize arrows!");

	_arrow[DIR_NN] = MV_NN;
	_arrow[DIR_NE] = MV_NE;
	_arrow[DIR_SE] = MV_SE;
	_arrow[DIR_SS] = MV_SS;
	_arrow[DIR_SW] = MV_SW;
	_arrow[DIR_NW] = MV_NW;
}

/******************************************************************************
 * The definition of the move marker array.
 *****************************************************************************/

#define MKR_MAX 32

static s_marker_move _mkr_mv_array[MKR_MAX];

static int _mkr_mv_num_used = 0;

/******************************************************************************
 * The definition of the move marker colors. We have 3 colors for the shading
 * and a normal and highlighted color.
 *****************************************************************************/

#define NUM_SHADES 3

static short _mkr_clr_normal[NUM_SHADES];

static short _mkr_clr_highlight[NUM_SHADES];

static short _mkr_clr_fg;

/******************************************************************************
 * The function initializes the colors and color pairs.
 *****************************************************************************/

static void s_marker_move_init_colors() {
	log_debug_str("Initialize colors / color pairs!");

	//
	// Set the foreground color.
	//
	// TODO: color of engine ??
	_mkr_clr_fg = COLOR_YELLOW;

	//
	// The shadings of the color for the normal state
	//
	_mkr_clr_normal[0] = col_color_create(50, 150, 50);
	_mkr_clr_normal[1] = col_color_create(80, 180, 80);
	_mkr_clr_normal[2] = col_color_create(110, 210, 110);

	cp_color_pair_add(_mkr_clr_fg, _mkr_clr_normal[0]);
	cp_color_pair_add(_mkr_clr_fg, _mkr_clr_normal[1]);
	cp_color_pair_add(_mkr_clr_fg, _mkr_clr_normal[2]);

	//
	// The shadings of the color for the highlighted state
	//
	_mkr_clr_highlight[0] = col_color_create(50, 250, 50);
	_mkr_clr_highlight[1] = col_color_create(80, 280, 80);
	_mkr_clr_highlight[2] = col_color_create(110, 310, 110);

	cp_color_pair_add(_mkr_clr_fg, _mkr_clr_highlight[0]);
	cp_color_pair_add(_mkr_clr_fg, _mkr_clr_highlight[1]);
	cp_color_pair_add(_mkr_clr_fg, _mkr_clr_highlight[2]);
}

/******************************************************************************
 * The function initializes move markers.
 *****************************************************************************/

void s_marker_move_init() {

	s_marker_move_init_arows();

	s_marker_move_init_colors();
}

/******************************************************************************
 * The function returns the next unused s_marker_move instance from the array
 * and initializes it with the given direction.
 *****************************************************************************/

s_marker_move* s_marker_move_get(const e_dir dir) {

	//
	// Ensure that there is an unused marker struct.
	//
	if (_mkr_mv_num_used >= MKR_MAX) {
		log_exit_str("No more marker left!");
	}

	s_marker_move *marker_move = &_mkr_mv_array[_mkr_mv_num_used++];

	marker_move->dir = dir;

	return marker_move;
}

/******************************************************************************
 * The function resets the array of s_marker_move instances. This simple means
 * to set the index of the next unused move marker to 0.
 *****************************************************************************/

void s_marker_move_release() {
	_mkr_mv_num_used = 0;
}

/******************************************************************************
 * The function adds the move marker to the hex field. It changes the
 * background color and adds arrows to the field.
 *****************************************************************************/

void s_marker_move_to_field(const s_marker_move *marker, const int color_idx, s_hex_field *hex_field, const bool highlight) {

	//
	// Get the background color including the background shading and highlighting.
	//
	const short bg = highlight ? _mkr_clr_highlight[color_idx] : _mkr_clr_normal[color_idx];

	//
	// Set the background for the move marker
	//
	hex_field_set_bg(hex_field, bg);

	//
	// If the move marker has a foreground character
	//

	//
	// There are two types of move markers. The first move maker has no arrows.
	// This marker is used to indicate the ship that moves. The other type has
	// two arrows at fixed positions.
	//
	if (marker->dir != DIR_UNDEF) {

		//
		// Add the first arrow with the direction
		//
		hex_field->point[1][1].chr = _arrow[marker->dir];
		hex_field->point[1][1].fg = _mkr_clr_fg;

		//
		// Add the second arrow with the direction
		//
		hex_field->point[2][1].chr = _arrow[marker->dir];
		hex_field->point[2][1].fg = _mkr_clr_fg;
	}
}
