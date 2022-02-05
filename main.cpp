#include <iostream>
#include <string>
#include <vector>
#include "pokemon.h"

// If low you can Pokemon or catch it
// Run is to run
// Fight gives you options

void lower_string(std::string& input) { // this is just for convienice
	for (char &letter : input) {
		letter = tolower(letter);
	}
}

void gotoxy(int x,int y) {
    printf("%c[%d;%df",0x1B,y,x);
}

class Battle {
public:

	unsigned int rows, cols;

	Battle(unsigned int r = 0, unsigned int c = 0) {
		srand((unsigned)time(0));
		if (r == 0||c == 0) {
			FILE *fp;
			char output[5];
			fp = popen("tput lines", "r");
			while (fgets(output, sizeof(output), fp) != NULL) {
				rows = atoi(output);
			}
			pclose(fp);

			fp = popen("tput cols", "r");
			while (fgets(output, sizeof(output), fp) != NULL) {
				cols = atoi(output);
			}
			pclose(fp);
		}
		else {
			rows = r;
			cols = c;
		}
		--rows;
		if (rows%2 != 0) {
			--rows;
		}
		if (cols%2 != 0) {
			--cols;
		}
		setup_screen();
	}

	std::vector<std::vector<char>> *display = new std::vector<std::vector<char>>;

	~Battle() {
		delete display;
		std::cout << "Battle Ended." << std::endl;
	}
	
	char get_input()
	{
		std::string output;
		std::cin >> output;
		lower_string(output);
		if (output == "a"||output=="attack") { return 'a'; }
		else if (output == "r"||output=="run") { return 'r'; }
		else if (output == "p"||output=="pokemon") { return 'p'; }
		else { return 'n'; }
	}

	void setup_screen()
	{
		for (int current_row = 0; current_row != rows; current_row++) {
			std::vector<char> temp;
			for (int current_col = 0; current_col != cols; current_col++) {

				if (current_col == (cols-1)) {
					temp.push_back('|');
				}
				else if (current_col == 0) {
					temp.push_back('|');
				}
				else {
					if (current_row == 0 || current_row == (rows-1)) {
						temp.push_back('-');
					}
					else {
						temp.push_back(' ');
					}
				}
			}
			display->push_back(temp);
		}
	}

	void print_screen()
	{
		std::cout << "\x1b[2J\x1b[H";
		for (int current_row = 0; current_row != rows; current_row++) {
			for (int current_col = 0; current_col != cols; current_col++) {
				if (current_col == (cols-1)) { std::cout << (*display)[current_row][current_col] << std::endl; }
				else { std::cout << (*display)[current_row][current_col]; }
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

	void prep(std::vector<pokemon> poke_list, std::vector<pokemon>& players, pokemon player1 = { "None" }, pokemon opponent = { "None" })
	{
		const unsigned int start1_col = (cols-std::string_view("Welcome to CLI Pokemon!").length())/2;
		const unsigned int start1_row = (rows/2) - 1;
		const unsigned int start2_col = (cols-std::string_view("Press enter to begin!").length())/2;
		const unsigned int start2_row = rows/2;
		unsigned int current_char = start1_col;
		for (char letter : std::string_view("Welcome to CLI Pokemon!")) {
			(*display)[start1_row][current_char] = letter;
			++current_char;
		}
		current_char = start2_col;
		for (char letter : std::string_view("Press enter to begin!")) {
			(*display)[start2_row][current_char] = letter;
			++current_char;
		}
		print_screen();
		std::cin.get();
		std::cout << "\x1b[2J\x1b[H";
		bool con = true, found = false;
		if (player1.name != "None") { con = false; }
		unsigned int current_row = 0, current_y = 1;
		std::string pokemon_name;
		std::string name;
		std::string output;
		while (con == true)
		{
			std::cout << "Select a pokemon: " << std::endl;
			current_row = 0;
			for (pokemon poke : poke_list) {
				std::cout << poke.name << std::endl;
				++current_row;
			}
			gotoxy(19, current_y);
			std::cin >> output;
			lower_string(output);
			for (pokemon poke : poke_list) {
				name = poke.name;
				lower_string(name);
				if (output == name) {
					pokemon_name = poke.name;
					found = true;
				}
			}
			
			if (!found) {
				std::cout << "\x1b[2J\x1b[H";
			}
			else {
				char yezorno;
				for (pokemon poke : poke_list) {
					std::cout << "\n";
				}
				std::cout << "You choose " << pokemon_name << ". Are you sure? y/n" << std::endl;
				std::cin >> yezorno;
				if (tolower(yezorno) == 'y') {
					con = false;
				}
				else {
					std::cout << "\x1b[2J\x1b[H";;
				}
			}
		}
		if (opponent.name == "None") {
			unsigned int type;
			type = rand()%poke_list.size();
			opponent = poke_list[type];
		}
		if (found = true) {
			player1.name = pokemon_name;
		}
		players.push_back(player1);
		players.push_back(opponent);
	}

	char game_loop(std::vector<pokemon>& players) {
		pokemon *player = &players[0], *opponent = &players[1];
		unsigned int pokemon_spawn, pokemon_spawn_chance = 0.2*100;
		while (true)
		{
			update_screen(*player, *opponent);
			char action = get_input();
			if (action != 'n') {
				if (action == 'p') {
					if (opponent->health <= 10) {
						pokemon_spawn = rand()%pokemon_spawn_chance;
						if (pokemon_spawn == 0) {
							return 'p';
						}
					}
				}
				else if (action == 'a') {

				}
			}
		}
	}

private:
	void put_pokemon_on_display(const unsigned int start_col, std::string name, std::string level, std::string health)
	{
		unsigned int current_char = start_col;
		for (char letter : name) {
			(*display)[1][current_char] = letter;
			++current_char;
		}
		++current_char;
		for (char letter : std::string_view("(Level:")) {
			(*display)[1][current_char] = letter;
			++current_char;
		}
		unsigned int current_number = 0;
		for (char number : level) {
			(*display)[1][current_char] = level[current_number];
			++current_number;
			++current_char;
		}
		(*display)[1][current_char] = ')';
		current_char = start_col;
		for (char letter : std::string_view("HP:")) {
			(*display)[2][current_char] = letter;
			++current_char;
		}
		for (char letter : health) {
			(*display)[2][current_char] = letter;
			++current_char;
		}
	}

	void clear_screen()
	{
		unsigned int current_row = 0, current_col;
		for (std::vector<char> &row : *display) {
			if (current_row == 0) {}
			else if (current_row == (rows - 1)) {}
			else {
				current_col = 0;
				for (char &col : row) {
					if (current_col == 0||current_col == (cols-1)) {}
					else {
						col = ' ';
					}
					++current_col;
				}
			}
			++current_row;
		}
	}
};

int main() {
	Battle* Pokemon = new Battle;
	std::vector<pokemon>* poke_list = new std::vector<pokemon>;
	std::vector<attack> poke1_attacks;
	attack main_attack = { "Lightning", 20, "Strikes an enemy with lightning" };
	poke1_attacks.push_back(main_attack);
	std::vector<attack> poke2_attacks;
	main_attack = { "Fire", 25, "Burns your enemy" };
	poke2_attacks.push_back(main_attack);
	pokemon poke1 = {"Char", 1, 100, poke1_attacks}, poke2 = {"Picka", 3, 80, poke2_attacks};
	poke_list->push_back(poke1);
	poke_list->push_back(poke2);
	std::vector<pokemon> players;
	Pokemon->prep(*poke_list, players);
	Pokemon->game_loop(players);
	delete Pokemon;
	return 0;
}