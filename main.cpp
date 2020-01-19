#include <ncurses.h>
#include <string>
#include <cstdlib>

using namespace std;

struct item {
	string content;
};

struct item todo[100];
string item_state(bool);

int main() {

	initscr(); 						// start curses mode (INITIALIZE SCREEN)
	raw();							// get raw character input, not lines
	keypad(stdscr, TRUE);			// get arrow keys, etc
	noecho();						// hide typed input
	curs_set(0);					// hide cursor

	int row=0, col=0; 				// store screen size
	getmaxyx(stdscr, row, col);

	int selected_index = 0; 		// start under application title

	// initialize window

	WINDOW *win = newwin(row, col, 0, 0);
	char title[] = "test title";

	while(1) {
		refresh();
		// clear();
		// wrefresh(win);

		attrset(A_UNDERLINE);
		mvprintw(0, 0, "%s", title); // print title at top

		// print window contents
		attrset(A_NORMAL);

		for (int i = 2; i <= 5; i++) {
			char* item = "item";
			if (i == (int)selected_index+2) {
				attrset(A_STANDOUT);
				mvprintw(i, 0, "[ ] %s%", item);
				attrset(A_NORMAL);
			}
			else
				mvprintw(i, 0, "[ ] %s%", item);

			/*
			mvprintw(i, 0, "[ ] %s", "item");
			if (i == (int)selected_index+2)
				mvchgat(i, 0, 30, A_STANDOUT, 1, NULL);
			*/
		}

		char in = getch();

		// interpret input

		if (in == (char)KEY_UP || in=='k') {
			if (selected_index > 0)
				selected_index--;
		}

		else if (in == (char)KEY_DOWN || in == 'j') {
			if (selected_index <= (5))
				selected_index++;
		}

		else if (in == 'q') {
			endwin();
			return 0;
		}
	}

	endwin();
	return 0;
}

string tick_state(bool ticked) {
	// get item state

	return ticked ? "[x]" : "[ ]";
}
