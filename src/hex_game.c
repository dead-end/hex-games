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

#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>

#include "hg_common.h"
#include "hg_ncurses.h"

#include "hg_color.h"
#include "hg_color_pair.h"

#include "hg_space.h"

#include "hg_ship.h"
#include "hg_obj_area.h"
#include "hg_marker.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory. This is
 * important if the program terminates after an error.
 *****************************************************************************/

static void hg_exit_callback() {

	ncur_exit();

	space_free();

	obj_area_free();

	log_debug_str("Exit callback finished!");
}

/******************************************************************************
 * The initial function of the program.
 *****************************************************************************/

static void hg_init() {

	setlocale(LC_ALL, "");

	ncur_init();

	//
	// Register exit callback.
	//
	if (on_exit(hg_exit_callback, NULL) != 0) {
		log_exit_str("Unable to register exit function!");
	}

	//
	// Initializes random number generator. The function does not return a
	// value.
	//
	time_t t;
	srand((unsigned) time(&t));
}

/******************************************************************************
 *
 *****************************************************************************/

static void print_object(const s_point *hex_idx, const bool highlight) {
	s_hex_field hf_tmp_bg, hf_tmp_fg;

	const s_object *obj = obj_area_get(hex_idx->row, hex_idx->col);

	//
	// Get the index of the shading (0, 1, 2)
	//
	const int color_idx = hex_field_color_idx(hex_idx->row, hex_idx->col);

	switch (obj->obj) {

	case OBJ_NONE:

		log_debug("space: %d/%d", hex_idx->row, hex_idx->col);

		space_get_hex_field(hex_idx, color_idx, highlight, &hf_tmp_bg);

		s_marker_add_to_field(obj->marker, color_idx, highlight, &hf_tmp_bg);

		hex_field_print(stdscr, hex_idx, NULL, &hf_tmp_bg);
		break;

	case OBJ_SHIP:

		log_debug("ship: %d/%d", hex_idx->row, hex_idx->col);

		space_get_hex_field(hex_idx, color_idx, highlight, &hf_tmp_bg);

		s_marker_add_to_field(obj->marker, color_idx, highlight, &hf_tmp_bg);

		ship_get_hex_field(obj->ship_inst->ship_type, obj->ship_inst->dir, &hf_tmp_fg);

		hex_field_print(stdscr, hex_idx, &hf_tmp_fg, &hf_tmp_bg);
		break;

	default:
		log_exit("Unknown object type: %d", obj->obj)
		;
	}
}

/******************************************************************************
 * The function prints the space hex fields of the game.
 *****************************************************************************/

static void print_objects(const s_point *hex_dim) {
	log_debug_str("Print objects");

	s_point hex_idx;

	for (hex_idx.row = 0; hex_idx.row < hex_dim->row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < hex_dim->col; hex_idx.col++) {

			print_object(&hex_idx, false);
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void set_ship(const int row, const int col, s_ship_inst *ship_inst, e_dir dir, s_ship_type *ship_type) {

	s_ship_inst_set(ship_inst, dir, ship_type);

	s_object_set_ship_at(row, col, ship_inst);
}

/******************************************************************************
 * Main
 *****************************************************************************/

int main() {
	s_ship_inst ship_inst[1];

	s_point hex_idx;

	const s_point hex_max = { .row = 5, .col = 12 };

	s_point hex_idx_old = { .row = -1, .col = -1 };

	hg_init();

	space_init(&hex_max);

	obj_area_init(&hex_max);

	ship_field_init();

	s_marker_init();

	s_ship_type *ship_type_normal = ship_type_get(SHIP_TYPE_NORMAL);

	s_point ship_point = { .row = 2, .col = 2 };
	set_ship(ship_point.row, ship_point.col, &ship_inst[0], DIR_NN, ship_type_normal);

	s_marker *marker;

	marker = s_marker_allocate(MRK_TYPE_MOVE);
	s_marker_move_set(&marker->marker_move, DIR_NW);
	obj_area_add_marker(1, 1, marker);

	marker = s_marker_allocate(MRK_TYPE_MOVE);
	s_marker_move_set(&marker->marker_move, DIR_NN);
	obj_area_add_marker(1, 2, marker);

	marker = s_marker_allocate(MRK_TYPE_MOVE);
	s_marker_move_set(&marker->marker_move, DIR_NE);
	obj_area_add_marker(1, 3, marker);

	marker = s_marker_allocate(MRK_TYPE_MOVE);
	s_marker_move_set(&marker->marker_move, DIR_UNDEF);
	obj_area_add_marker(2, 2, marker);

	print_objects(&hex_max);

	for (;;) {
		int c = wgetch(stdscr);

		//
		// Exit with 'q'
		//
		if (c == 'q') {
			break;
		}

		if (c == KEY_MOUSE) {
			MEVENT event;

			if (getmouse(&event) != OK) {
				log_exit_str("Unable to get mouse event!");
			}

			hex_get_hex_idx(event.y, event.x, &hex_idx, &hex_max);

			if (event.bstate & BUTTON1_PRESSED) {

				if (!s_point_inside(&hex_max, &hex_idx)) {
					continue;
				}

				s_object *obj = obj_area_get(hex_idx.row, hex_idx.col);

				if (obj->marker == NULL || obj->marker->type != MRK_TYPE_MOVE || obj->marker->marker_move.dir == DIR_UNDEF) {
					continue;
				}

				if (obj_area_mv_ship(&ship_point, &hex_idx, obj->marker->marker_move.dir)) {
					print_object(&ship_point, false);
					print_object(&hex_idx, false);
					s_point_copy(&ship_point, &hex_idx);
				}
				continue;
			}

			if (!s_point_same(&hex_idx, &hex_idx_old)) {

				//
				// Delete old
				//
				if (hex_idx_old.row >= 0 && hex_idx_old.col >= 0) {
					print_object(&hex_idx_old, false);
				}

				//
				// Print new
				//
				if (hex_idx.row >= 0 && hex_idx.col >= 0) {
					print_object(&hex_idx, true);
				}

				s_point_copy(&hex_idx_old, &hex_idx);
			}
		}
	}

	//
	// Cleanup is handled with the exit callback.
	//
	return EXIT_SUCCESS;
}
