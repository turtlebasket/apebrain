#include <ncurses.h>
#include <json/json.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

#include "tdutil.h"

using namespace std;

// TODO: JSON-based save system
vector<item> load_save(string filename) {
	vector<item> list;
	ifstream infile;
	infile.open(filename);

	string content, tick_state;
	bool ticked=false;

	while(getline(infile, content) && getline(infile, tick_state)){
		if (tick_state=="t") ticked=true;
		else if (tick_state=="f") ticked=false;
		else exit(1);
		list.push_back(item(content, ticked));
	}

	infile.close();
	return list;
}

void write_save(vector<item> list, string filename) {

	// rearrange ticked
	for (int i = list.size()-1; i>=0; i--) {
		if (list[i].ticked) {
			item temp = list[i];
			list.push_back(temp);
			list.erase(list.begin() + i);
		}
	}

	ofstream outfile;
	outfile.open(filename);

	for (int i=0; i<(int)list.size(); i++) {
		outfile << (list[i].content+"\n").c_str();
		outfile << ((string)(list[i].ticked?"t":"f")).c_str();
		if (i!=(int)list.size()-1)
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

		for (int i = 0; i < (int)todo.size(); i++) {
			item task = todo.at(i);
			if (i == (int)selected_index) {
				attrset(A_STANDOUT);
				mvprintw(i+2, 0, "%d.\t%s %s% ", i+1, task.tick_state().c_str(), task.content.c_str());
				attrset(A_NORMAL);
			}
			else
				mvprintw(i+2, 0, "%d.\t%s %s% ", i+1, task.tick_state().c_str(), task.content.c_str());

		}

		char in = getch();

		// interpret input

		if (in == (char)KEY_UP || in=='k') {
			if (selected_index > 0)
				selected_index--;
		}

		else if (in == (char)KEY_DOWN || in == 'j') {
			if (selected_index < (int)todo.size()-1)
				selected_index++;
		}

		else if (in == 'K') {
			if (selected_index > 0) {
				item temp = todo[selected_index-1];
				todo[selected_index-1] = todo[selected_index];
				todo[selected_index] = temp;
				selected_index--;
                clear();
			}
		}

		else if (in == 'J') {
			if (selected_index < (int)todo.size()-1) {
				item temp = todo[selected_index+1];
				todo[selected_index+1] = todo[selected_index];
				todo[selected_index] = temp;
				selected_index++;
                clear();
			}
		}

		else if ((int)in-'0' >=1 && (int)in-'0' <=9)
			selected_index = (int)in-'0'-1;

		else if (in == ' ' || in == '\n') {
			todo[selected_index].tick();
		}

        else if (in == 'd') {
            if (selected_index == (int)(todo.size()-1)) {
                selected_index--;
                todo.erase(todo.begin()+selected_index+1);
            }
            else
                todo.erase(todo.begin()+selected_index);
            clear();
        }

        else if (in == 'x') {
			for (int i=(int)todo.size()-1; i>=0; i--) {
				if (todo[i].ticked)
					todo.erase(todo.begin()+i);
			}
			selected_index=0;
			clear();
			wrefresh(win);
		}

		else if (in == 'a') {

			clear();
			WINDOW* get_add = newwin(row, col, 0, 0);
			curs_set(1);
			string add_name = "";
			bool input_escaped = false;
            char c;
            mvprintw(0, 0, "%s", "Add task: ");

			while ((c=getch())!='\n') {

				if ((int)c == 27) { // escape character
					input_escaped = true;
					break;
				}

				else if ((int)c == 7 || c == '\b') {       //backspace character
                    if (add_name.size() > 0)               // is there something to delete?
                        add_name.erase(add_name.size()-1);
                    // otherwise, do nothing
				}

                // TODO: Ctrl+W keybinding support
                else if ((int)c == 23) {
                    if (add_name.size() > 0) {
                        for (int i = add_name.length(); i >= (int)add_name.rfind(' '); i--)
                            add_name.erase(i);
                    }
                }

				else {
					add_name += c;
					// add_name += to_string((int)c);
				}

                mvprintw(0, 10, "%s", add_name.c_str());
                clrtoeol();

			}

			delwin(get_add);
            if (!input_escaped)
                todo.insert(todo.begin(), item(add_name));

			curs_set(0);
			clear();
			wrefresh(win);
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
