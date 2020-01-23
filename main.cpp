#include <ncurses.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

struct item {
	string content;
	bool ticked;

	item(string cont) {
		content=cont;
		ticked=false;
	}

	item(string cont, bool tick) {
		content=cont;
		ticked=tick;
	}

	void tick() {
		ticked = !ticked;
	}

	string tick_state() {
		return ticked ? "[x]" : "[ ]";
	}
};

vector<item> load_save(string filename) {
	vector<item> list;
	ifstream infile;
	infile.open(filename);

	string content, tick_state;
	bool ticked=false;

	while(!infile.eof()){
		getline(infile, content);
		getline(infile, tick_state);
		if (tick_state=="t") ticked=true;
		else if (tick_state=="f") ticked=false;
		else exit(1);
		list.push_back(item(content, ticked));
	}

	return list;
}

void write_save(vector<item> list, string filename) {

	// rearrange ticked
	for (int i = 0; i < list.size(); i++) {
		if (list[i].ticked) {
			item temp = list[i];
			list.push_back(temp);
			list.erase(list.begin() + i);
		}
	}

	ofstream outfile;
	outfile.open(filename);

	for (int i=0; i<list.size(); i++) {
		outfile << (list[i].content+"\n").c_str();
		outfile << ((string)(list[i].ticked?"t":"f")).c_str();
		if (i!=list.size()-1)
			outfile << "\n";
	}

	outfile.close();
}

int main() {
	string SAVE_NAME = "save.txt";
	vector<item> todo = load_save(SAVE_NAME);

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
				mvprintw(i+2, 0, " %s %s%", task.tick_state().c_str(), task.content.c_str());
				attrset(A_NORMAL);
			}
			else
				mvprintw(i+2, 0, " %s %s%", task.tick_state().c_str(), task.content.c_str());

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

		else if (in == 'x') {
			for (int i=0; i<todo.size(); i++) {
				if (todo[i].ticked)
					todo.erase(todo.begin()+i);
					wrefresh(win);
			}
		}

		else if (in == 'q') {
			write_save(todo, SAVE_NAME);	// write save file
			endwin();						// close window
			return 0;
		}
	}

	endwin();
	return 0;
}
