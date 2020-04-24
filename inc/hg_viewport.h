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

#ifndef INC_HG_VIEWPORT_H_
#define INC_HG_VIEWPORT_H_

#include "hg_common.h"

/******************************************************************************
 * The viewport has a dimension and a position. The total dimension of the game
 * is also stored here.
 *****************************************************************************/

typedef struct {

	//
	// The dimension of the viewport
	//
	s_point dim;

	//
	// The position of the viewport
	//
	s_point pos;

	//
	// The dimension of the game
	//
	s_point max;

} s_viewport;

/******************************************************************************
 * Definition of the macros.
 *****************************************************************************/

#define s_viewport_get() &_viewport

#define s_viewport_get_abs(v,r,t) (t)->row = (v)->pos.row + (r)->row;  (t)->col = (v)->pos.col + (r)->col

/******************************************************************************
 * Definition of the functions.
 *****************************************************************************/

bool s_viewport_inside_viewport(const s_viewport *viewport, const s_point *idx);

bool s_viewport_update(s_viewport *viewport, const s_point *idx);

void s_viewport_get_ul(const s_viewport *viewport, const s_point *idx_abs, s_point *pos_ul);

#endif /* INC_HG_VIEWPORT_H_ */
