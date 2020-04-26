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

#include "hg_marker.h"

/******************************************************************************
 * The definition of the marker array.
 *****************************************************************************/

#define MKR_MAX 32

static s_marker _mkr_array[MKR_MAX];

static int _mkr_num_used = 0;

/******************************************************************************
 * The initialization function calls the initialization functions for the
 * concrete markers.
 *****************************************************************************/

void s_marker_init() {
	log_debug_str("Initialize the markers!");

	s_marker_move_init();
}

/******************************************************************************
 * The function gets the next unused marker from the marker array.
 *****************************************************************************/

static s_marker* s_marker_get(const e_marker type) {

	//
	// Ensure that there is an unused marker struct.
	//
	if (_mkr_num_used >= MKR_MAX) {
		log_exit_str("No more marker left!");
	}

	s_marker *marker = &_mkr_array[_mkr_num_used++];

	//
	// Set the marker type
	//
	marker->type = type;

	return marker;
}

/******************************************************************************
 * The function gets the next unused s_maker and the next unused s_marker_move
 * and combines them.
 *****************************************************************************/

s_marker* s_marker_get_move_marker(const e_marker type, const e_dir dir) {

	s_marker *marker = s_marker_get(type);

	marker->marker_move = s_marker_move_get(dir);

	return marker;
}

/******************************************************************************
 *The function resets the array of s_marker instances.
 *****************************************************************************/

void s_marker_release() {
	_mkr_num_used = 0;

	s_marker_move_release();
}

/******************************************************************************
 * The function adds the marker to the hex field. This is done by delegating
 * the call to the specific function.
 *****************************************************************************/

void s_marker_add_to_field(const s_marker *marker, const int color_idx, const bool highlight, s_hex_field *hex_field) {

	//
	// If the field has no marker, there is nothing to do.
	//
	if (marker == NULL) {
		return;
	}

	//
	// Select the marker type and delegate the call.
	//
	switch (marker->type) {

	case MRK_TYPE_MOVE:
		s_marker_move_to_field(marker->marker_move, color_idx, hex_field, highlight);
		break;

	default:
		log_exit("Unknown marker type: %d", marker->type)
		;
	}
}
