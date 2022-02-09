#pragma once
#include <string>
#include <vector>
#include <sys/ioctl.h>

enum center_opt {
	x,
	y,
	both,
	none
};

struct location {
	int x = 0;
	int y = 0;
	center_opt opt = none;
};

class Terminal {
public:
	unsigned int rows;
	unsigned int cols;

	std::vector<std::vector<char>> *display = new std::vector<std::vector<char>>;

	bool border = true;

	Terminal(unsigned int r = 0, unsigned int c = 0) {
		if (r == 0 || c == 0) {
			struct winsize w;
			ioctl(0, TIOCGWINSZ, &w);
			if (r == 0) {
				r = w.ws_row;
			}
			if (c == 0) {
				c = w.ws_col;
			}
		}
		rows = r;
		cols = c;
		std::cout << "Rows: " << rows << " Cols: " << cols;
	}

	void gotoxy(unsigned int x, unsigned int y) {
		if (x > rows) {
			return;
		}
		else if (y > cols) {
			return;
		}
		std::cout << "%c[%d;%df" << 0x1B << y << x;
	}

	void set_size(unsigned int r, unsigned int c) {
		if (r > 0 && c > 0) {
			rows = r;
			display->clear();
			for (unsigned int current_row = 0; current_row != rows; current_row++) {
				std::vector<char> temp;
				for (unsigned int current_col = 0; current_col != cols; current_col++) {
					if (current_col == 0 || current_col == cols - 1) {
						temp.push_back('|');
					}
					else if (current_row == 0 || current_row == rows - 1) {
						temp.push_back('-');
					}
					else {
						temp.push_back(' '); 
					}
				}
				display->push_back(temp);
			}
		}
	}
	
	void write(std::string text, location loco = { 0, 0, none }, bool extend = false) {
		if ((loco.x > border ? rows - 1 : rows) || (border && (loco.x == 1)) || (loco.x == 0)) {
			return;
		}
		if ((loco.y > border ? cols - 1 : cols) || (border && cols == 1) || (cols == 0)) {
			return;
		}
		if ((border && text.length() > rows - 2) || (!border && text.length() > rows) || text.find('\n') != std::string::npos) {
			if (extend) {
				location new_loco = loco;
				if (new_loco.opt == both) {
					new_loco.opt = x;
				}
				else if (new_loco.opt == y) {
					new_loco.opt = none;
				}
				++new_loco.y;
				write(text.substr(rows - 2), new_loco, true);
			}
		}
		switch (loco.opt) {
			case x:
				loco.x = (cols - text.length())/2;
				break;
			case y:
				loco.y = rows/2;
				break;
			case both:
				loco.x = (cols - text.length())/2;
				loco.y = rows/2;
				break;
			default:
				break;
		}
		for (auto text_pos = text.begin(); text_pos != text.end(); text_pos++) {
			display->at(loco.x + (text_pos - text.begin())).at(loco.y) = text[text_pos - text.begin()];
		}
	}

	void print_screen() {
		for (auto current_row = display->begin(); current_row != display->end(); current_row++) {
			for (auto current_col = current_row->begin(); current_col != current_row->end(); current_col++) {
				if ((current_col - current_row->begin()) == ((current_row->end() - current_row->begin()) - 1)) {
					std::cout << *current_col << '\n';
				}
				else {
					std::cout << *current_col;
				}
			}
			std::cout << std::flush;
		}
		clear_screen();
	}

	void clear_screen() {
		if (!border) {
			for (std::vector<char> &row : *display) {
				for (char &character : row) {
					character = ' ';
				}
			}
		}
		else {
			for (auto current_row = display->begin() + 1; current_row != display->end() - 1; current_row++) {
				for (auto current_col = current_row->begin(); current_col != current_row->end(); current_col++) {
					if (current_col - current_row->begin() == 0 || current_col - current_row->begin() == cols - 1) {
						continue;
					}
					else {
						*current_col = ' ';
					}
				}
			}
		}		
	}
};
