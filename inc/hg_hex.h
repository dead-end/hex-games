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
#include "hg_color.h"

#include "ncurses.h"

#define HEX_SIZE 4

/******************************************************************************
 * The struct for a hex point, which consists of a (foreground) character and a
 * foreground and background color.
 *****************************************************************************/

typedef struct {

	wchar_t *chr;

	short fg;

	short bg;

} s_hex_point;

/******************************************************************************
 * The struct is the base of the space. It is a 4x4 array of s_hex_points.
 *****************************************************************************/

typedef struct {

	s_hex_point point[HEX_SIZE][HEX_SIZE];

} s_hex_field;

/******************************************************************************
 * The macro definitions.
 *****************************************************************************/

#define hex_field_is_corner(r,c) ((r) % 3 == 0 && (c) % 3 == 0)

#define hex_point_set(h,c,f,b) (h).chr = (c); (h).fg = (f); (h).bg = (b)

#define hex_point_set_undef(h) (h).chr = W_NULL; (h).fg = COLOR_UNDEF; (h).bg = COLOR_UNDEF

//
// The macro definition to get the shading of the background color.
//
#define hex_field_color_idx(r,c) ((r) + 2 * ((c) % 2)) % 3

/******************************************************************************
 * The function definitions.
 *****************************************************************************/

void hex_get_hex_idx(const int win_row, const int win_col, const s_point *hex_max, s_point *hex_idx);

void hex_field_set_corners(s_hex_field *hex_field);

void hex_field_print(WINDOW *win, const s_point *pos_ul, const s_hex_field *hex_field_fg, const s_hex_field *hex_field_bg);

void hex_field_set_bg(s_hex_field *hex_field, const short bg);

#endif /* INC_HG_HEX_H_ */
