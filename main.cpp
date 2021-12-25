#include <vector>
#include <iostream>
#include <string>

// If low you can Pokemon or catch it
// Run is to run
// Fight gives you options

struct pokemon {
	std::string name = "None";
	unsigned int level = 1;
	int health = 100;
};

class Battle {
public:
	const unsigned int rows = 26, cols = 64;
	std::vector<std::vector<char>> display;
	
	void setup_screen()
	{
		for (int current_row = 0; current_row != rows; current_row++) {
			std::vector<char> temp;
			for (int current_col = 0; current_col != cols; current_col++) {
				temp.push_back(' ');
			}
			display.push_back(temp);
		}
	}
	
	char get_input()
	{
		std::string output;
		std::cin >> output;
		for (int x; x <= output.length(); x++) {
			output[x] = tolower(output[x]);
		}
		if (output == "a") { return 'a'; }
		else if (output == "attack") { return 'a'; }
		else if (output == "r") { return 'r'; }
		else if (output == "run") { return 'r'; }
		else if (output == "p") { return 'p'; }
		else if (output == "pokemon") { return 'p'; }
		else { return 'n'; }
	}

	void print_screen()
	{
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

	void display_pokemon(pokemon poke1, pokemon poke2)
	{
		std::string poke1_level = std::to_string(poke1.level);
		std::string poke1_health = std::to_string(poke1.health);
		unsigned int start_col = 2;
		put_pokemon_on_display(start_col, poke1.name, poke1_level, poke1_health);
		std::string poke2_level = std::to_string(poke2.level);
		std::string poke2_health = std::to_string(poke2.health);
		start_col = (((cols-poke2.name.length())-poke2_level.length())-std::string_view("(Level:").length())-4;
		put_pokemon_on_display(start_col, poke2.name, poke2_level, poke2_health);
	}

	void update_screen(pokemon poke1, pokemon poke2)
	{
		display_pokemon(poke1, poke2);
		print_screen();
	}

	void gotoxy(int x,int y) {
    printf("%c[%d;%df",0x1B,y,x);
	}

	void game_loop(std::vector<pokemon> poke_list)
	{
		std::cout << "Welcome to CLI Pokemon!" << std::endl << "Click enter to begin!" << std::endl;
		std::string output;
		std::cin >> output;
		std::cout << "\x1b[2J\x1b[H";
		bool con = true;
		bool found = false;
		unsigned int current_y = 1;
		while (con) {
			std::cout << "Select a pokemon: ";
			for (pokemon poke : poke_list) {
				std::cout << std::endl << poke.name;
			}
			gotoxy(19, current_y);
			std::cin >> output;
			for (pokemon poke : poke_list) {
				if (output == poke.name) { found = true; }
			}
			if (!found) {
				std::cout << "\x1b[2J\x1b[H";
			}
			else {
				char yezorno;
				for (pokemon poke: poke_list) {
					std::cout << std::endl;
				}
				std::cout << "You choose " << output << ". Are you sure? y/n" << std::endl;
				std::cin >> yezorno;
				if (tolower(yezorno) == 'y') {
					con = false;
				}
				else {
					std::cout << "\x1b[2J\x1b[H";;
				}
			}
		}
		setup_screen();
		pokemon player1 = { output };
		pokemon player2 = {"Picka", 3, 80};
		update_screen(player1, player2);
	}

private:
	void put_pokemon_on_display(const unsigned int start_col, std::string name, std::string level, std::string health)
	{
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
	void clear_screen()
	{
		for (std::vector<char> row : display) {
			for (char col : row) {
				col = ' ';
			}
		}
	}
};

int main() {
	Battle Pokemon;
	std::vector<pokemon> poke_list;
	pokemon player1 = {"Char", 1, 100};
	pokemon player2 = {"Picka", 3, 80};
	poke_list.push_back(player1);
	poke_list.push_back(player2);
	Pokemon.game_loop(poke_list);
	return 0;
}