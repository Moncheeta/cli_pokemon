#include <vector>
#include <iostream>
#include <string>

struct pokemon {
	std::string name;
	unsigned int level;
	int health;
};

class game {
public:
	std::vector<std::vector<char>> display;
	const unsigned int rows = 26, cols = 64;
	void setup_screen() {
		for (int current_row = 0; current_row != rows; current_row++) {
			std::vector<char> temp;
			for (int current_col = 0; current_col != cols; current_col++) {
				temp.push_back(' ');
			}
			display.push_back(temp);
		}
	}

	void print_screen() {
		for (int current_row = 0; current_row != rows; current_row++) {
			for (int current_col = 0; current_col != cols; current_col++) {
				if (current_col == (cols-1)) {
					std::cout << '|' << std::endl;
				}
				else if (current_col == 0) {
					std::cout << '|';
				}
				else {
					if (current_row == 0 || current_row == (rows-1)) {
						std::cout << '-';
					}
					else {
						std::cout << display[current_row][current_col];
					}
				}
			}
		}
		clear_screen();
	}

	void clear_screen() {
		for (std::vector<char> row : display) {
			for (char col : row) {
				col = ' ';
			}
		}
	}

	void display_pokemon(pokemon &poke1) {
		std::string poke1_name = poke1.name;
		std::string poke1_level = std::to_string(poke1.level);
		for (auto letter : "(Level:") {
			std::cout << "'" << letter << "'" << std::endl;
		}
		std::string poke1_health= std::to_string(poke1.health);
		unsigned int current_char = 2;
		for (char letter : poke1_name) {
			display[1][current_char] = letter;
			++current_char;
		}
		++current_char;
		for (char letter : "(Level:") {
			display[1][current_char] = letter;
			++current_char;
		}
		unsigned int current_number = 0;
		for (char number : poke1_level) {
			display[1][current_char] = poke1_level[current_number];
			++current_number;
			++current_char;
		}
		display[1][current_char] = ')';
		current_char = 2;
		for (char letter : "HP:") {
			display[2][current_char] = letter;
			++current_char;
		}
		for (char letter : poke1_health) {
			display[2][current_char] = letter;
			++current_char;
		}	
	}
};

int main() {
	game Battle;
	Battle.setup_screen();
	pokemon player1 = {"Char", 1, 100};
	Battle.display_pokemon(player1);
	Battle.print_screen();
	return 0;
}
