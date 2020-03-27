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

#include "hg_common.h"

#include <ncurses.h>

/******************************************************************************
 * The function initializes the ncurses mouse support.
 *****************************************************************************/

static void ncur_init_mouse() {

	if (has_mouse()) {
		log_exit_str("Terminal does not support a mouse!");
	}

	//
	// Register mouse events (which do not have a proper error handling)
	//
	mousemask(BUTTON1_RELEASED | BUTTON1_CLICKED | BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

	printf("\033[?1003h\n");

	mouseinterval(0);
}

/******************************************************************************
 * The function finishes the ncurses mouse support.
 *****************************************************************************/

static void ncur_finish_mouse() {

	//
	// Disable mouse movement events, as l = low
	//
	printf("\033[?1003l\n");

}

/******************************************************************************
 * The function initializes the main features of ncurses .
 *****************************************************************************/

void ncur_init() {

	//
	// Initialize screen.
	//
	if (initscr() == NULL) {
		log_exit_str("Unable to initialize the screen.");
	}

	if (start_color() == ERR) {
		fprintf(stderr, "Unable to start color!\n");
		exit(EXIT_FAILURE);
	}

	//
	// Disable line buffering.
	//
	if (raw() == ERR) {
		log_exit_str("Unable to set raw mode.");
	}

	//
	// Disable echoing.
	//
	if (noecho() == ERR) {
		log_exit_str("Unable to switch off echoing.");
	}

	//
	// Enable keypad for the terminal.
	//
	if (keypad(stdscr, TRUE) == ERR) {
		log_exit_str("Unable to enable the keypad of the terminal.");
	}

	//
	// Switch off the cursor.
	//
	if (curs_set(0) == ERR) {
		log_exit_str("Unable to set cursor visibility.");
	}

	//
	// Disable ESC delay. (The man page says: "These functions all return TRUE
	// or FALSE, except as noted." which seems not to be correct.
	//
	set_escdelay(0);

	ncur_init_mouse();
}

/******************************************************************************
 * The function ends the ncurses components.
 *****************************************************************************/

void ncur_exit() {

	//
	// End the windows.
	//
	endwin();

	ncur_finish_mouse();
}
