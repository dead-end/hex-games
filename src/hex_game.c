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
#include "hg_viewport.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory. This is
 * important if the program terminates after an error.
 *****************************************************************************/

static void hg_exit_callback() {
	log_debug_str("Exit callback start!");

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

static void print_object(s_viewport *viewport, s_object *obj, const bool highlight) {
	s_hex_field hf_tmp_bg, hf_tmp_fg;

	s_point pos_ul;
	s_viewport_get_ul(viewport, &obj->pos, &pos_ul);

	//
	// Get the index of the shading (0, 1, 2)
	//
	const int color_idx = hex_field_color_idx(obj->pos.row, obj->pos.col);

	log_debug("pos: %d/%d color index: %d", obj->pos.row, obj->pos.col, color_idx);

	switch (obj->obj) {

	case OBJ_NONE:

		log_debug("space: %d/%d", obj->pos.row, obj->pos.col);

		space_get_hex_field(&obj->pos, color_idx, highlight, &hf_tmp_bg);

		s_marker_add_to_field(obj->marker, color_idx, highlight, &hf_tmp_bg);

		hex_field_print(stdscr, &pos_ul, NULL, &hf_tmp_bg);
		break;

	case OBJ_SHIP:

		log_debug("ship: %d/%d", obj->pos.row, obj->pos.col);

		space_get_hex_field(&obj->pos, color_idx, highlight, &hf_tmp_bg);

		s_marker_add_to_field(obj->marker, color_idx, highlight, &hf_tmp_bg);

		ship_get_hex_field(obj->ship_inst->ship_type, obj->ship_inst->dir, &hf_tmp_fg);

		hex_field_print(stdscr, &pos_ul, &hf_tmp_fg, &hf_tmp_bg);
		break;

	default:
		log_exit("Unknown object type: %d", obj->obj)
		;
	}
}

/******************************************************************************
 * The function prints the space hex fields of the game.
 *****************************************************************************/

static void print_objects(s_viewport *viewport) {
	log_debug_str("Print objects");

	s_object *obj;
	s_point idx_rel, idx_abs;

	if (wclear(stdscr) == ERR) {
		log_exit_str("Unable to clear window.");
	}

	for (idx_rel.row = 0; idx_rel.row < viewport->dim.row; idx_rel.row++) {
		for (idx_rel.col = 0; idx_rel.col < viewport->dim.col; idx_rel.col++) {

			s_viewport_get_abs(viewport, &idx_rel, &idx_abs);

			obj = obj_area_get(idx_abs.row, idx_abs.col);

			print_object(viewport, obj, false);
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static void reset_marker(s_viewport *viewport) {
	log_debug_str("Resetting marker!");

	s_point hex_idx;
	s_object *object;

	for (hex_idx.row = 0; hex_idx.row < viewport->max.row; hex_idx.row++) {
		for (hex_idx.col = 0; hex_idx.col < viewport->max.col; hex_idx.col++) {

			object = obj_area_get(hex_idx.row, hex_idx.col);

			if (object->marker != NULL) {
				object->marker = NULL;
				print_object(viewport, object, false);
			}
		}
	}

	s_marker_reset();
}

/******************************************************************************
 *
 *****************************************************************************/

static void set_ship(const int row, const int col, const e_ship_type ship_type, const e_dir dir) {

	s_ship_inst *ship_inst = s_ship_inst_create(ship_type, dir);

	s_object_set_ship_at(row, col, ship_inst);
}

/******************************************************************************
 *
 *****************************************************************************/

static void set_marker(s_viewport *viewport, s_object *obj_from) {
	s_object *obj_to;

	//
	// Ensure that the object is a ship.
	//
	if (obj_from->obj != OBJ_SHIP) {
		log_exit("Object is not a ship: %d/%d", obj_from->pos.row, obj_from->pos.col);
	}

	obj_area_set_mv_marker(obj_from, DIR_UNDEF);
	print_object(viewport, obj_from, true);

	//
	// Set the marker along the paths.
	//
	char **paths = obj_from->ship_inst->ship_type->paths;

	for (int i = 0; paths[i] != NULL; i++) {

		//
		// Try to set the marker for the path. If it is not possible the
		// function returns NULL.
		//
		obj_to = obj_area_set_mv_marker_path(obj_from, paths[i]);
		if (obj_to != NULL) {
			print_object(viewport, obj_to, false);
		}
	}
}

/******************************************************************************
 *
 *****************************************************************************/

static s_object* cursor_mv(s_viewport *viewport, s_object *obj_from, s_point *to) {

	log_debug("Moving cursor from: %d/%d to: %d/%d viewport: %d/%d", obj_from->pos.row, obj_from->pos.col, to->row, to->col, viewport->pos.row, viewport->pos.col);

	if (!s_point_inside(&viewport->max, to)) {
		log_debug("Target outside game: %d/%d", to->row, to->col);
		return obj_from;
	}

	if (s_point_same(&obj_from->pos, to)) {
		log_debug("Source and target are the same: %d/%d", to->row, to->col);
		return obj_from;
	}

	s_object *obj_to = obj_area_get(to->row, to->col);

	if (!s_viewport_inside_viewport(viewport, &obj_to->pos)) {

		log_debug("Not inside viewport pos: %d/%d dim: %d/%d", viewport->pos.row, viewport->pos.col, viewport->dim.row, viewport->dim.col);

		if (s_viewport_update(viewport, &obj_to->pos)) {
			print_objects(viewport);
		}

		if (!s_viewport_inside_viewport(viewport, &obj_to->pos)) {
			log_exit_str("Outside");
		}

		print_object(viewport, obj_to, true);
		return obj_to;
	}

	//
	// Delete old and print the new
	//
	if (s_viewport_inside_viewport(viewport, &obj_from->pos)) {
		print_object(viewport, obj_from, false);
	}
	print_object(viewport, obj_to, true);

	return obj_to;
}

/******************************************************************************
 * Return next obj_old
 *****************************************************************************/

static s_object* ship_move(s_viewport *viewport, s_object *obj_from, s_object *obj_to) {

	if (!s_viewport_inside_viewport(viewport, &obj_to->pos)) {
		return obj_from;
	}

	if (!obj_area_can_mv_to(obj_to)) {
		return obj_from;
	}

	obj_area_mv_ship(obj_from, obj_to, obj_to->marker->marker_move->dir);

	reset_marker(viewport);

	print_object(viewport, obj_from, false);
	print_object(viewport, obj_to, false);

	set_marker(viewport, obj_to);

	return obj_to;
}

/******************************************************************************
 * Main
 *****************************************************************************/

int main() {
	s_object *obj_old;
	s_object *obj_ship;

	s_point hex_idx;

	s_viewport viewport;
	viewport.dim.row = 5;
	viewport.dim.col = 12;
	viewport.max.row = 10;
	viewport.max.col = 24;
	viewport.pos.row = 0;
	viewport.pos.col = 0;

	hg_init();

	space_init(&viewport.max);

	obj_area_init(&viewport.max);

	ship_field_init();

	s_marker_init();

	set_ship(3, 3, SHIP_TYPE_NORMAL, DIR_NE);

	obj_ship = obj_area_get(3, 2);

	set_ship(obj_ship->pos.row, obj_ship->pos.col, SHIP_TYPE_NORMAL, DIR_NN);

	set_marker(&viewport, obj_ship);

	print_objects(&viewport);

	//
	// Setting the initial cursor is a little hack, because we need an old
	// cursor position.
	//
	s_point_set(&hex_idx, 0, 0);
	obj_old = cursor_mv(&viewport, obj_area_get(1, 1), &hex_idx);

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

			//
			// From event x/y to relative hex index inside the viewport
			//
			hex_get_hex_idx(event.y, event.x, &hex_idx, &viewport.max);

			//
			// From the relative hex index to the absolute hex index.
			//
			s_viewport_get_abs(&viewport, &hex_idx, &hex_idx);

			if (event.bstate & BUTTON1_PRESSED) {
				obj_ship = ship_move(&viewport, obj_ship, obj_area_get(hex_idx.row, hex_idx.col));

			} else {
				obj_old = cursor_mv(&viewport, obj_old, &hex_idx);
			}

		} else {

			switch (c) {

			case KEY_UP:
				log_debug_str("arrow up");
				s_point_set(&hex_idx, obj_old->pos.row - 1, obj_old->pos.col)
				;
				obj_old = cursor_mv(&viewport, obj_old, &hex_idx);
				break;

			case KEY_DOWN:
				log_debug_str("arrow down");
				s_point_set(&hex_idx, obj_old->pos.row + 1, obj_old->pos.col)
				;
				obj_old = cursor_mv(&viewport, obj_old, &hex_idx);
				break;

			case KEY_LEFT:
				log_debug_str("arrow left");
				s_point_set(&hex_idx, obj_old->pos.row, obj_old->pos.col - 1)
				;
				obj_old = cursor_mv(&viewport, obj_old, &hex_idx);
				break;

			case KEY_RIGHT:
				log_debug_str("arrow right");
				s_point_set(&hex_idx, obj_old->pos.row, obj_old->pos.col + 1)
				;
				obj_old = cursor_mv(&viewport, obj_old, &hex_idx);
				break;

			case 10:
				log_debug_str("Enter");
				obj_ship = ship_move(&viewport, obj_ship, obj_area_get(hex_idx.row, hex_idx.col));
				break;
			}
		}
	}

	log_debug_str("End (before cleanup)");

	//
	// Cleanup is handled with the exit callback.
	//
	return EXIT_SUCCESS;
}
