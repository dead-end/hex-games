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

#include "hg_viewport.h"

/******************************************************************************
 * The function check if a point is inside the viewpoint.
 *
 * Example: pos: 2 dim:4
 *
 * 0123456789
 * --1234----
 *****************************************************************************/

bool s_viewport_inside_viewport(const s_viewport *viewport, const s_point *idx) {

	if (idx->row < viewport->pos.row || idx->row >= viewport->pos.row + viewport->dim.row) {
		return false;
	}

	if (idx->col < viewport->pos.col || idx->col >= viewport->pos.col + viewport->dim.col) {
		return false;
	}

	return true;
}

/******************************************************************************
 * The function updates the viewport position.
 *****************************************************************************/

bool s_viewport_update(s_viewport *viewport, const s_point *pos_new) {
	bool do_update = false;

	log_debug("Viewport pos: %d/%d new: %d/%d", viewport->pos.row, viewport->pos.col, pos_new->row, pos_new->col);

	//
	// Update row
	//
	if (pos_new->row < viewport->pos.row) {

		if (pos_new->row >= 0) {
			viewport->pos.row = pos_new->row;
			do_update = true;
		}

	} else if (pos_new->row >= viewport->pos.row + viewport->dim.row) {

		if (viewport->pos.row + viewport->dim.row < viewport->max.row) {
			viewport->pos.row = pos_new->row - viewport->dim.row + 1;
			do_update = true;
		}
	}

	//
	// Update col
	//
	if (pos_new->col < viewport->pos.col) {

		if (pos_new->col >= 0) {
			viewport->pos.col = pos_new->col;
			do_update = true;
		}

	} else if (pos_new->col >= viewport->pos.col + viewport->dim.col) {

		if (viewport->pos.col + viewport->dim.col < viewport->max.col) {
			viewport->pos.col = pos_new->col - viewport->dim.col + 1;
			do_update = true;
		}
	}

	if (do_update) {
		log_debug("Viewport pos updated: %d/%d", viewport->pos.row, viewport->pos.col);
	}

	return do_update;
}

/******************************************************************************
 * The function moves the viewport in to a given direction.
 *****************************************************************************/

bool s_viewport_mv_diff(s_viewport *viewport, const s_point *diff) {

	s_point new_pos = { .row = viewport->pos.row + diff->row, .col = viewport->pos.col + diff->col };

	//
	// Update the row if necessary
	//
	if (new_pos.row < 0) {
		new_pos.row = 0;

	} else if (new_pos.row + viewport->dim.row > viewport->max.row) {
		new_pos.row = viewport->max.row - viewport->dim.row;
	}

	//
	// Update the column if necessary
	//
	if (new_pos.col < 0) {
		new_pos.col = 0;

	} else if (new_pos.col + viewport->dim.col > viewport->max.col) {
		new_pos.col = viewport->max.col - viewport->dim.col;
	}

	if (s_point_same(&viewport->pos, &new_pos)) {
		return false;
	}

	log_debug("pos from: %d/%d to: %d/%d", viewport->pos.row, viewport->pos.col, new_pos.row, new_pos.col);

	viewport->pos.row = new_pos.row;
	viewport->pos.col = new_pos.col;

	return true;
}

/******************************************************************************
 * The function computes the upper left corner with an absolute index and a
 * viewport.
 *****************************************************************************/

void s_viewport_get_ul(const s_viewport *viewport, const s_point *idx_abs, s_point *pos_ul) {

	//
	// Get the relative position inside the viewport.
	//
	const int idx_rel_row = idx_abs->row - viewport->pos.row;
	const int idx_rel_col = idx_abs->col - viewport->pos.col;

	//
	// The row position toggles depending on odd or even (column) hex fields.
	//
	const int offset = viewport->pos.col % 2;

	//
	// Compute the position
	//
	pos_ul->row = (((idx_rel_col) + (offset)) % 2) * 2 + (idx_rel_row) * 4;
	pos_ul->col = idx_rel_col * 3;
}
