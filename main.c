#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

char* item_state(bool);

int main() {

	initscr(); 				// start curses mode (INITIALIZE SCREEN)
	raw();					// get raw user input, not lines
	keypad(stdscr, TRUE);	// get arrow keys, etc
	noecho();				// hide typed input
	curs_set(0);			// hide cursor

	char todo[] = {'a', 'b', 'c'};

	int row, col; // store max values
	int selected_index = 0; // start under application title
	getmaxyx(stdscr, row, col);

	// initialize window

	char title[] = "test title";

	// create pointer to WINDOW struct
	WINDOW *win = newwin(row, col, 0, 0);

	while(1) {
		// clear();
		wrefresh(win);

		attrset(A_UNDERLINE);
		mvprintw(0, 0, "%s", title); // print title at top

		// print window contents
		attrset(A_NORMAL);

		for (int i = 2; i <= 5; i++) {
			if (i == (int)selected_index+2) {
				attrset(A_STANDOUT);
				mvprintw(i, 0, "[ ] %s%", "item");
				attrset(A_NORMAL);
			}

			else
				mvprintw(i, 0, "[ ] %s%", "item");
		}

		char in = getch();

		// interpret input
 
		if (in == (char)KEY_UP || in=='k') {
			if (selected_index > 0)
				selected_index--;
		}

		else if (in == (char)KEY_DOWN || in == 'j') {
			if (selected_index < (5))
				selected_index++;
		}

		else if (in == 'q') {
			endwin();
			return 0;
		}

		refresh();
	}

	endwin();
	return 0;
}

char* item_state(bool ticked) {
	switch(ticked){
		case 0 :
			return "[ ]";
		case 1:
			return "[x]";
	}
}
