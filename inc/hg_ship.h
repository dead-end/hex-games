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

#ifndef INC_HG_SHIP_H_
#define INC_HG_SHIP_H_

/******************************************************************************
 * The definition of the 6 directions of a hex field.
 *****************************************************************************/

#define DIR_NUM 6

typedef enum dir {

	DIR_NN, DIR_NE, DIR_SE, DIR_SS, DIR_SW, DIR_NW

} e_dir;

/******************************************************************************
 * The struct defines a ship type. Currently the different ship types have
 * different colors.
 *****************************************************************************/

typedef struct s_ship_type {

	short c_dark;

	short c_light;

	short c_yellow;

} s_ship_type;

/******************************************************************************
 * The enum value is the id of the ship type.
 *****************************************************************************/

typedef enum e_ship_type {

	SHIP_TYPE_NORMAL

} e_ship_type;

/******************************************************************************
 * Definition of the functions.
 *****************************************************************************/

void ship_field_init();

void ship_field_free();

void ship_get_field(const s_ship_type *ship_type, const e_dir dir, s_hex_field *ship_field);

s_ship_type* ship_type_get(e_ship_type ship_type);

#endif /* INC_HG_SHIP_H_ */
