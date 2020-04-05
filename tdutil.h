#include <string>

struct item {
	std::string content;
	bool ticked;

	item(std::string cont) {
		content=cont;
		ticked=false;
	}

	item(std::string cont, bool tick) {
		content=cont;
		ticked=tick;
	}

	void tick() {
		ticked = !ticked;
	}

	std::string tick_state() {
		return ticked ? "[x]" : "[ ]";
	}
};
