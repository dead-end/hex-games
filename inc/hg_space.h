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

#ifndef INC_HG_SPACE_H_
#define INC_HG_SPACE_H_

#include "hg_common.h"

/******************************************************************************
 *
 *****************************************************************************/

typedef enum e_state {

	STATE_NORMAL = 0, STATE_SELECT = 1

} e_state;

#define space_get_color_pair(bg) cp_color_pair_get(COLOR_WHITE, (bg))

/******************************************************************************
 * The function definitions.
 *****************************************************************************/

void space_init(s_point *dim_hex);

void space_free();

s_hex_point** space_get_hex_field(const s_point *hex);

short space_get_color(const int row, const int col, const e_state state);

#endif /* INC_HG_SPACE_H_ */
