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

s_marker _marker[MKR_MAX];

int _num_used = 0;

/******************************************************************************
 * The definition of arrow characters for the move markers.
 *****************************************************************************/

#define MV_NN L"\x2B06"
#define MV_NE L"\x2B08"
#define MV_SE L"\x2B0A"
#define MV_SS L"\x2B07"
#define MV_SW L"\x2B0B"
#define MV_NW L"\x2B09"

/******************************************************************************
 * The function returns a marker instance for a given direction.
 *****************************************************************************/

s_marker* s_marker_get_move(const e_dir dir) {

	//
	// Ensure that there is an unused marker struct.
	//
	if (_num_used >= MKR_MAX) {
		log_exit_str("No more marker left!");
	}

	s_marker *marker = &_marker[_num_used++];

	marker->type = MRK_TYPE_MOVE;

	switch (dir) {

	case DIR_NN:
		marker->chr = MV_NN;
		break;

	case DIR_NE:
		marker->chr = MV_NE;
		break;

	case DIR_SE:
		marker->chr = MV_SE;
		break;

	case DIR_SS:
		marker->chr = MV_SS;
		break;

	case DIR_SW:
		marker->chr = MV_SW;
		break;

	case DIR_NW:
		marker->chr = MV_NW;
		break;

	default:
		log_exit("Unknown dir: %d", dir)
		;
	}

	return marker;
}
