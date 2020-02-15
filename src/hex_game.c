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

#include <stdlib.h>
#include <locale.h>
#include <ncursesw/ncurses.h>

#include "hg_common.h"

/******************************************************************************
 * The exit callback function resets the terminal and frees the memory. This is
 * important if the program terminates after an error.
 *****************************************************************************/

static void hg_exit_callback() {

	endwin();

	log_debug_str("Exit callback finished!");
}

/***************************************************************************
 * The initial function of the program.
 **************************************************************************/

static void hg_init() {

	setlocale(LC_ALL, "");

	//
	// Register exit callback.
	//
	if (on_exit(hg_exit_callback, NULL) != 0) {
		log_exit_str("Unable to register exit function!");
	}
}

/***************************************************************************
 * Main
 **************************************************************************/

int main() {

	hg_init();

	if (initscr() == NULL) {
		fprintf(stderr, "Unable to init screen!\n");
		exit(EXIT_FAILURE);
	}

	curs_set(0);

	if (start_color() == ERR) {
		fprintf(stderr, "Unable to start color!\n");
		exit(EXIT_FAILURE);
	}

	getch();

	//
	// Cleanup is handled with the exit callback.
	//
	return EXIT_SUCCESS;
}
