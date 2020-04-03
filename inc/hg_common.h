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

#ifndef INC_HG_COMMON_H_
#define INC_HG_COMMON_H_

#include <stdlib.h>
#include <stdio.h>

#define VERSION "0.1.0"

/******************************************************************************
 * Definition of the print_debug macro. It is only defined if the DEBUG flag is
 * defined. It prints to stderr not to restrain curses.
 *****************************************************************************/

#ifdef DEBUG

#define log_debug(fmt, ...) fprintf(stderr, "DEBUG %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define log_debug_str(fmt)  fprintf(stderr, "DEBUG %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__)

#define DEBUG_USED

#else

#define log_debug(fmt, ...)
#define log_debug_str(fmt)

#define DEBUG_USED __attribute__((unused))

#endif

/******************************************************************************
 * Definition of the print_error macro, that finishes the program after
 * printing the error message.
 *****************************************************************************/

#define log_exit(fmt, ...) fprintf(stderr, "FATAL %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); exit(EXIT_FAILURE)
#define log_exit_str(fmt)  fprintf(stderr, "FATAL %s:%d:%s() " fmt "\n", __FILE__, __LINE__, __func__); exit(EXIT_FAILURE)

/******************************************************************************
 * The s_point struct represents an element that has a row and a column. This
 * can be a pixel (terminal character), an array dimension, a block size...
 *****************************************************************************/

typedef struct s_point {

	int row;

	int col;

} s_point;

//
// The macro sets the row and column of a s_point struct.
//
#define s_point_set(p,r,c) (p)->row = (r);(p)->col = (c)

#define s_point_copy(t,f) (t)->row = (f)->row; (t)->col = (f)->col

#define s_point_same(t,f) (((t)->row == (f)->row) && ((t)->col == (f)->col))

#define s_point_inside(b,p) (0 <= (p)->row && (p)->row < (b)->row && 0 <= (p)->col && (p)->col < (b)->col)

// TODO:
#define W_EMPTY L" "

#define W_STAR L"."

#define W_NULL NULL

void* xmalloc(const size_t size);

#endif /* INC_HG_COMMON_H_ */
