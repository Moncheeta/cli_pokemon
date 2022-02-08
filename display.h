#pragma once
#include <string>
#include <vector>
#include <sys/ioctl.h>

struct point {
	int x = 0;
	int y = 0;
}

class Terminal {
public:
	unsigned int rows;
	unsigned int cols;

	std::vector<std::vector<char>>> *display = new std::vector<std::vector<char>>>;

	bool border = true;

	enum center_opt {
		x;
		y;
		both;
		none;
	}

	struct location {
		int x = 0;
		int y = 0;
		center_opt opt = none;
	}

	Terminal(unsigned int r = NULL, unsigned int c = NULL) {
		if (r == NULL || c == NULL) {
			struct winsize w;
			ioctl(0, TIOCGWINSZ, &w);
			if (r == NULL) {
				rows = w.ws_row;
			}
			if (c == NULL) {
				cols = w.ws_col;
			}
		}

		if (r != NULL || c != NULL) {
			if (r != NULL) {
				rows = r;
			}
			if (c != NULL) {
				cols = c;
			}
		}
	}

	~Terminal() {
		delete *display;
	}

	void gotoxy(int x, int y) {
		if (x < 0) {
			x = rows - x;
		}
		if (y < 0) {
			y = cols - y;
		}
		std::cout << "%c[%d;%df" << 0x1B << y << x;
	}

	void set_size(unsigned int r = NULL, unsigned int c = NULL) {
		if (r != NULL) {
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
	
	void write(std::string text, location point) {
		if (!border) {
			if (point.x > cols || point.y > rows) { throw out_of_range; }
		}
		else {
			if (point.x == 0 || point.x == cols || point.y == 0 || point.y == rows) { throw out_of_range; }
		}
		unsigned int max_length = cols - location.x;
		if (border) {
			max_length = max_length - 2;
		}
		for (unsigned int current_char = location.x; current_char <= text.length(); current_char++) {
			*display[location.x + current_char][location.y];
		}
	}

	void print_screen() {
		for (auto current_row = display->begin(); current_row != display->end(); current_row++) {
			for (auto current_col = current_row->begin(); current_col != current_row->end(); current_col++){
				if (current_col - current_row->begin() == current_row->end() - 1){
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

private:
	void clear_screen() {
		for (auto current_row = display->begin(); current_row != display->end(); current_row++) {
			for (auto current_col = current_row->begin(); current_col != current_row->end(); current_col++){
				if (border) {
					if (current_col - current_row->begin() == 0 || current_col - current_row->begin() == current_row->end() - 1) {
						*display[current_row - display->begin()][current_col - current_row->begin()] = '|';
					}
					else if (current_row - current_row->begin() == 0 || current_row - current_row->begin() == rows - 1) {
						*display[current_row - display->begin()][current_col - current_row->begin()] = '-';
					}
					else {
						*display[current_row - display->begin()][current_col - current_row->begin()] = ' ';
					}
				}
				else {
						*display[current_row - display->begin()][current_col - current_row->begin()] = ' ';
				}
			}
		}
	}
}
