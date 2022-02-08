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

struct attack {
	std::string name;
	unsigned int damage;
	std::string desc;
};

struct pokemon {
	std::string name = "None";
	unsigned int level = 1;
	int health = 100;

	std::vector<attack> attacks;
};

class Battle {
public:

	Terminal *term = new Terminal;

	char get_input()
	{
		term->gotoxy(term->border ? 1 : 0, term->rows - 1);
		std::string output;
		std::cin >> output;
		lower_string(output);
		if (output == "a"||output=="attack") { return 'a'; }
		else if (output == "r"||output=="run") { return 'r'; }
		else if (output == "p"||output=="pokemon") { return 'p'; }
		else { return 'n'; }
	}

	void display_pokemon(pokemon &poke1, pokemon &poke2)
	{
		term->write(poke1.name + " (Level: " + std::to_string(poke1.level) + ')', { 1, 1 });
		term->write("HP: " + std::to_string(poke1.health), { 1, 2 });
	}

	void update_screen(pokemon poke1, pokemon poke2)
	{
		display_pokemon(poke1, poke2);
		term->print_screen();
	}

	void prep(std::vector<pokemon> &list_of_pokemon, pokemon &player, pokemon &opponent)
	{
		std::cout << std::endl;
		term->write("Welcome to CLI Pokemon!\nPress enter to begin!", { 0, 0, both });
		term->print_screen();
		std::cin.get();
		std::cout << "\x1b[2J\x1b[H";
		bool con = true, found = false;
		if (player.name != "None") { con = false; }
		unsigned int current_row = 0, current_y = 1;
		std::string input, output;
		while (con == true)
		{
			term->write("Select a pokemon: ");
			current_row = term->border ? 1 : 0;
			for (pokemon poke : list_of_pokemon) {
				try {
					term->write(poke.name, { term->border ? 1 : 0, (int)current_row });
					++current_row;
				}
				catch (...) {
					break;
				};
			}
			term->gotoxy(19, current_y);
			std::cin >> input;
			lower_string(input);
			for (pokemon poke : list_of_pokemon) {
				output = poke.name;
				lower_string(output);
				if (input == output) {
					player.name = poke.name;
					found = true;
				}
			}
			
			if (!found) {
				std::cout << "\x1b[2J\x1b[H";
			}
			else {
				char confirmation = 'n';
				term->gotoxy(term->border ? 1 : 0, term->border ? list_of_pokemon.size() + 1 : list_of_pokemon.size());
				term->write("You choose " + player.name + ". Are you sure? y/n ");
				std::cin >> confirmation;
				if (tolower(confirmation) == 'y') {
					con = false;
				}
				else {
					std::cout << "\x1b[2J\x1b[H";
					continue;
				}
			}
		}
		if (opponent.name == "None") {
			unsigned int type;
			type = rand()%list_of_pokemon.size();
			opponent = list_of_pokemon[type];
		}
	}

	char game_loop(pokemon &player, pokemon &opponent) {
		unsigned int pokemon_spawn, pokemon_spawn_chance = 0.2*100;
		while (true)
		{
			update_screen(player, opponent);
			char action = get_input();
			if (action != 'n') {
				if (action == 'p') {
					if (opponent.health <= 10) {
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
};

std::vector<pokemon> *list_of_pokemon = new std::vector<pokemon>;
list_of_pokemon->push_back( "Char", 1, 100, { "Fire", 25, "Burns your enemy" });
list_of_pokemon->push_back( "Picka", 3, 80, { "Lightning", 20, "Strikes an enemy with lightning" });

int main() {
	Battle* Pokemon = new Battle;
	pokemon player, opponent;
	Pokemon->prep(*list_of_pokemon, player, opponent);
	Pokemon->game_loop(player, opponent);
	delete Pokemon;
	return 0;
}
