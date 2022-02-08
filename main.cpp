#include <string>
#include <vector>
#include <iostream>
#include "display.h"

// If low you can Pokemon or catch it
// Run is to run
// Fight gives you options

void lower_string(std::string& input) { // this is just for convienice
	for (char &letter : input) {
		letter = tolower(letter);
	}
}

struct pokemon {
	std::string name = "None";
	unsigned int level = 1;
	int health = 100;

	std::vector<attack> attacks;

	void add_attack(std::string name, unsigned int damage, std::string desc = "None")
	{	
		attack new_attack = { name, damage, desc };
		attacks.push_back(new_attack);
	}

	void edit_attack(std::string name, unsigned int damage = 0, std::string desc = "None") {
		if (!attacks.empty()) {
			if (damage != 0) {
				for (attack& damage_dealer : attacks) {
					if (damage_dealer.name == name) {
						damage_dealer.damage = damage; 
						break;
					}
				}
			}
			else if (desc != "None") {
				for (attack& damage_dealer : attacks) {
					if (damage_dealer.name == name) {
						damage_dealer.desc = desc;
						break;
					}
				}
			}
		}
	}

	void remove_attack(std::string name) {
		if (!attacks.empty()) {
			for (auto it = attacks.begin(); it != attacks.end(); ++it) {
				if (it->name == name) {
					attacks.erase(it);
					break;
				}
			}
		}
	}
};

class Battle {
public:

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

	void display_pokemon(pokemon poke1, pokemon poke2)
	{
		start_col = (((cols-poke2.name.length())-poke2_level.length())-std::string_view("(Level:").length())-4;
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
