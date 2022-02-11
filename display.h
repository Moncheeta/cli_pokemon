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
	unsigned int x = 0;
	unsigned int y = 0;
	center_opt opt = none;
};

class Terminal {
public:
	unsigned int rows;
	unsigned int cols;

	std::vector<std::vector<char>> *display = new std::vector<std::vector<char>>;

	bool border = true;

	void gotoxy(unsigned int x, unsigned int y) {
		if (x > rows) {
			return;
		}
		else if (y > cols) {
			return;
		}
		printf("%c[%d;%df", 0x1B, y, x);
	}

	void clrscr() {
		std::cout << std::flush;
	}

	void set_size(unsigned int r, unsigned int c) {
		if (r > 0 && c > 0) {
			rows = r;
			cols = c;
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

	Terminal(unsigned int r = 0, unsigned int c = 0) {
		if (r == 0 || c == 0) {
			struct winsize w;
			ioctl(0, TIOCGWINSZ, &w);
			if (r == 0) {
				r = w.ws_row - 1;
			}
			if (c == 0) {
				c = w.ws_col;
			}
		}
		set_size(r, c);
	}
	
	void write(std::string text, location loco = { 0, 0, none }, bool extend = false) {
		auto n = text.find_first_of('\n');
		if (n != std::string::npos) {
			extend = true;
		}
		switch (loco.opt) {
			case x:
				if (n != std::string::npos) {
					loco.x = ((cols - text.substr(0, n).length())/2);
					break;
				}
				else {
					loco.x = (cols - text.length())/2;
					break;
				}
			case y:
				loco.y = rows/2;
				break;
			case both:
				if (n != std::string::npos) {
					loco.x = ((cols - text.substr(0, n).length())/2);
				}
				else {
					loco.x = (cols - text.length())/2;
				}
				loco.y = rows/2;
				break;
			default:
				break;
		}
		if ((border && text.length() > rows - 2) || (!border && text.length() > rows) || (n != std::string::npos)) {
			if (extend) {
				location new_loco = loco;
				if (new_loco.opt == both) {
					new_loco.opt = x;
				}
				else if (new_loco.opt == y) {
					new_loco.opt = none;
				}
				++new_loco.y;
				if (n != std::string::npos) {
					write(text.substr(n + 1), new_loco, true);
					text = text.substr(0, n);
				}
				else {
					write(text.substr(rows - 2), new_loco, true);
					text = text.substr(0, rows - 2);
				}
			}
		}
		if (loco.x == 0 && border) {
			++loco.x;
		}
		if (loco.y == 0 && border) {
			++loco.y;
		}
		if ((border && (loco.y >= rows)) || (border == false && (loco.y > rows)) || (loco.y < 0)) {
			return;
		}
		if ((border && (loco.x > cols)) || (border == false && (loco.x > cols)) || (loco.x < 0)) {
			return;
		}
		for (auto text_pos = text.begin(); text_pos != text.end(); text_pos++) {
			(*display)[loco.y][loco.x + (text_pos - text.begin())] = *text_pos;
		}
	}

	void print_screen(bool clear = true) {
		std::cout << std::flush;
		for (auto current_row = display->begin(); current_row != display->end(); current_row++) {
			for (auto current_col = current_row->begin(); current_col != current_row->end(); current_col++) {
				if ((current_col - current_row->begin()) == ((current_row->end() - current_row->begin()) - 1)) {
					std::cout << *current_col << '\n';
				}
				else {
					std::cout << *current_col;
				}
			}
		}
		if (clear) {
			clear_screen();
		}
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
			for (auto current_row = display->begin(); current_row != display->end(); current_row++) {
				if (current_row - display->begin() == 0 || current_row - display->begin() == rows - 1) {
					continue;
				}
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
