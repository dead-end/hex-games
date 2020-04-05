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

#ifndef INC_HG_MARKER_MOVE_H_
#define INC_HG_MARKER_MOVE_H_

#include "hg_hex.h"
#include "hg_dir.h"

/******************************************************************************
 * The move marker has an optional wchar_t character, which is an arrow to one
 * of the 6 directions of the hex field.
 *****************************************************************************/

typedef struct {

	e_dir dir;

} s_marker_move;

/******************************************************************************
 * The function definitions.
 *****************************************************************************/

void s_marker_move_init();

s_marker_move* s_marker_move_get(const e_dir dir);

void s_marker_move_reset();

void s_marker_move_to_field(const s_marker_move *marker, const int color_idx, s_hex_field *hex_field, const bool highlight);

#endif /* INC_HG_MARKER_MOVE_H_ */
