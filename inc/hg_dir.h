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

#ifndef INC_HG_DIR_H_
#define INC_HG_DIR_H_

/******************************************************************************
 * The definition of the 6 directions of a hex field which will be used as an
 * array index.
 *****************************************************************************/

#define DIR_NUM 6

typedef enum dir {

	DIR_UNDEF = -1,

	DIR_NN = 0,
	DIR_NE = 1,
	DIR_SE = 2,
	DIR_SS = 3,
	DIR_SW = 4,
	DIR_NW = 5

} e_dir;

#define DIR_STR_UNDEF "DIR-UNDEF"

#define DIR_STR_NN "DIR-NN"
#define DIR_STR_NE "DIR-NE"
#define DIR_STR_SE "DIR-SE"
#define DIR_STR_SS "DIR-SS"
#define DIR_STR_SW "DIR-SW"
#define DIR_STR_NW "DIR-NW"

#define DIR_MV_LEFT(d)  (((d) + 5) % 6)
#define DIR_MV_RIGHT(d) (((d) + 1) % 6)

/******************************************************************************
 * The definitions of the functions.
 *****************************************************************************/

char* e_dir_str(const e_dir dir);

#endif /* INC_HG_DIR_H_ */
