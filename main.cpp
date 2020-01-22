#include <ncurses.h>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

struct item {
	string content;
	bool ticked;
	void tick() {
		ticked = !ticked;
	}

	string tick_state() {
		return ticked ? "[x]" : "[ ]";
	}

	item(string cont) {
		content = cont;
		ticked=false;
	}
};


int main() {
	// placeholder values (TESTING)
	vector<item> todo {
		item("Take out the trash"),
		item("Do chemistry problems"),
		item("Eat crayons")
	};

	// ncurses init
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
	char title[] = "apebrain";

	while(1) {
		refresh();
		// clear();
		// wrefresh(win);

		attrset(A_UNDERLINE);
		mvprintw(0, 0, "%s", title); // print title at top

		// print window contents
		attrset(A_NORMAL);

		for (int i = 0; i < todo.size(); i++) {
			item task = todo.at(i);
			if (i == (int)selected_index) {
				attrset(A_STANDOUT);
				mvprintw(i+2, 0, "  %s %s%", task.tick_state().c_str(), task.content.c_str());
				attrset(A_NORMAL);
			}
			else
				mvprintw(i+2, 0, "  %s %s%", task.tick_state().c_str(), task.content.c_str());

		}

		char in = getch();

		// interpret input

		if (in == (char)KEY_UP || in=='k') {
			if (selected_index > 0)
				selected_index--;
		}

		else if (in == (char)KEY_DOWN || in == 'j') {
			if (selected_index < todo.size()-1)
				selected_index++;
		}

		else if (in == ' ') {
			todo[selected_index].tick();
		}

		else if (in == 'q') {
			endwin();
			return 0;
		}

	}

	endwin();
	return 0;
}
