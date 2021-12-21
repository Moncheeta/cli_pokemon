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
		std::cout << "\x1b[2J\x1b[H";
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

	void display_pokemon(pokemon poke1, pokemon poke2) {
		std::string poke1_level = std::to_string(poke1.level);
		std::string poke1_health = std::to_string(poke1.health);
		unsigned int start_col = 2;
		put_pokemon_on_display(start_col, poke1.name, poke1_level, poke1_health);
		std::string poke2_level = std::to_string(poke2.level);
		std::string poke2_health = std::to_string(poke2.health);
		start_col = (((cols-poke2.name.length())-poke2_level.length())-std::string_view("(Level:").length())-4;
		put_pokemon_on_display(start_col, poke2.name, poke2_level, poke2_health);
	}

private:
	void put_pokemon_on_display(const unsigned int start_col, std::string name, std::string level, std::string health) {
		unsigned int current_char = start_col;
		for (char letter : name) {
			display[1][current_char] = letter;
			++current_char;
		}
		++current_char;
		for (char letter : std::string_view("(Level:")) {
			display[1][current_char] = letter;
			++current_char;
		}
		unsigned int current_number = 0;
		for (char number : level) {
			display[1][current_char] = level[current_number];
			++current_number;
			++current_char;
		}
		display[1][current_char] = ')';
		current_char = start_col;
		for (char letter : std::string_view("HP:")) {
			display[2][current_char] = letter;
			++current_char;
		}
		for (char letter : health) {
			display[2][current_char] = letter;
			++current_char;
		}
	}
	void clear_screen() {
		for (std::vector<char> row : display) {
			for (char col : row) {
				col = ' ';
			}
		}
	}
};

int main() {
	game Battle;
	Battle.setup_screen();
	pokemon player1 = {"Char", 1, 100};
	pokemon player2 = {"Picka", 3, 80};
	Battle.display_pokemon(player1, player2);
	Battle.print_screen();
	return 0;
}
