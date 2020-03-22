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

#ifndef INC_HG_HEX_H_
#define INC_HG_HEX_H_

#include "hg_common.h"

#define HEX_SIZE 4

/******************************************************************************
 * The struct for a hex point, which consists of a (foreground) character and a
 * foreground and background color.
 *****************************************************************************/

typedef struct s_hex_point {

	wchar_t *chr;

	short fg;

	short bg;

} s_hex_point;

/******************************************************************************
 * The macro definitions.
 *****************************************************************************/

#define hex_field_ul_row(r,c) (((c) % 2) * 2 + (r) * 4)

#define hex_field_ul_col(r,c) ((c) * 3)

#define hex_field_is_corner(r,c) ((r) % 3 == 0 && (c) % 3 == 0)

#define hex_point_set(h,c,f,b) (h).chr = (c); (h).fg = (f); (h).bg = (b)

/******************************************************************************
 * The function definitions.
 *****************************************************************************/

s_hex_point** hex_field_alloc();

void hex_field_free(s_hex_point **hex_field);

void hex_get_hex_idx(const int win_row, const int win_col, s_point *hex_idx, const s_point *hex_max);

#endif /* INC_HG_HEX_H_ */
