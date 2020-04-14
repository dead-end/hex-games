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

#include <stdbool.h>

#include "hg_viewport.h"

/******************************************************************************
 * The defintion of the viewport.
 *****************************************************************************/

s_viewport _viewport;

/******************************************************************************
 * The function is called with a point and updates the viewport if necessary.
 * It returns true if the viewport is updated. An update requires printing of
 * the game.
 *****************************************************************************/

bool s_viewport_update(const s_point *idx) {
	bool do_update = false;

	log_debug("Viewport: %d/%d", _viewport.pos.row, _viewport.pos.col);

	//
	// Update row
	//
	if (idx->row < 0) {

		if (idx->row + _viewport.pos.row >= 0) {
			_viewport.pos.row = _viewport.pos.row + idx->row;
			do_update = true;

		}
	} else if (idx->row >= _viewport.dim.row) {

		if (idx->row + _viewport.pos.row < _viewport.max.row) {
			_viewport.pos.row = _viewport.pos.row + idx->row - (_viewport.dim.row - 1);
			do_update = true;
		}
	}

	//
	// Update col
	//
	if (idx->col < 0) {

		if (idx->col + _viewport.pos.col >= 0) {
			_viewport.pos.col = _viewport.pos.col + idx->col;
			do_update = true;
		}
	} else if (idx->col >= _viewport.dim.col) {

		if (idx->col + _viewport.pos.col < _viewport.max.col) {
			_viewport.pos.col = _viewport.pos.col + idx->col - (_viewport.dim.col - 1);
			do_update = true;
		}
	}

	if (do_update) {
		log_debug("Viewport updated: %d/%d", _viewport.pos.row, _viewport.pos.col);
	}

	return do_update;
}
