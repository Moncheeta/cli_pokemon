#include <string>
#include <vector>
#include <array>
#include <iostream>

#include "lib/display.hpp"
#include "pokemon.hpp"

void lower_string(std::string &input) { // this is just for convienice
  for (char &letter : input) {
    letter = tolower(letter);
  }
}

class Battle {
public:
  Terminal *term = new Terminal();

  void on_entry() {
    term->clrscr();
    term->write("Welcome to CLI Pokemon!\nPress enter to begin!",
                {0, 0, center_both});
    term->print_screen();
    std::cin.get();
    term->clrscr();
  }

  std::array<pokemon, 2> set_players(std::vector<pokemon> &list_of_pokemon) {
    bool con = true, found = false;
    unsigned int current_row = 0;
    std::string input, output;
    pokemon player;
    pokemon opponent;
    while (con == true) {
      term->write("Select a pokemon: ");
      current_row = term->border ? 2 : 1;
      for (pokemon &poke : list_of_pokemon) {
        try {
          term->write(poke.name,
                      {(unsigned int)(term->border ? 1 : 0), current_row});
          ++current_row;
        } catch (...) {
          break;
        };
      }
      term->print_screen(false);
      term->gotoxy(term->border ? 20 : 19, term->border ? 2 : 1);
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
        term->clrscr();
      } else {
        term->clrscr();
        char confirmation = 'n';
        std::string query = "You choose " + player.name + ". Are you sure? y/n";
        term->write(query,
                    {0,
                     (unsigned int)(term->border ? 3 + list_of_pokemon.size()
                                                 : 2 + list_of_pokemon.size()),
                     none});
        term->print_screen();
        term->gotoxy((unsigned int)(term->border ? 3 + query.length()
                                                 : 2 + query.length()),
                     (unsigned int)(term->border ? 4 + list_of_pokemon.size()
                                                 : 3 + list_of_pokemon.size()));
        std::cin >> confirmation;
        if (tolower(confirmation) == 'y') {
          term->clrscr();
          for (pokemon &poke : list_of_pokemon) {
            if (poke.name == player.name) {
              player = poke;
              con = false;
              break;
            }
          }
        } else {
          term->clrscr();
        }
      }
    }
    if (opponent.name == "Not Set") {
      unsigned int selected = rand() % list_of_pokemon.size();
      opponent = list_of_pokemon[selected];
    }
    std::array<pokemon, 2> players = {player, opponent};
    return players;
  }

  char get_input() {
    term->gotoxy(term->border ? 1 : 0, term->rows - 1);
    std::string output;
    std::cin >> output;
    lower_string(output);
    if (output == "a" || output == "attack") {
      return 'a';
    } else if (output == "r" || output == "run") {
      return 'r';
    } else if (output == "p" || output == "pokemon") {
      return 'p';
    } else {
      return 'n';
    }
  }

  void display_pokemon(pokemon &poke1, pokemon &poke2) {
    term->write(poke1.name + " (Level: " + std::to_string(poke1.level) +
                ")\nHP: " + std::to_string(poke1.health) + '\n' +
                poke1.character);
    term->write(poke2.name + " (Level: " + std::to_string(poke2.level) +
                    ")\nHP: " + std::to_string(poke2.health) + '\n' +
                    poke2.character,
                {0, 0, top_right});
  }

  void update_screen(pokemon poke1, pokemon poke2) {
    display_pokemon(poke1, poke2);
    term->print_screen();
  }

  char game_loop(pokemon &player, pokemon &opponent) {
    unsigned int pokemon_spawn, pokemon_spawn_chance = 20;
    while (true) {
      update_screen(player, opponent);
      char action = get_input();
      if (action != 'n') {
        if (action == 'p') {
          if (opponent.health <= 10) {
            pokemon_spawn = rand() % pokemon_spawn_chance;
            if (pokemon_spawn == 0) {
              return 'p';
            }
          }
        } else if (action == 'a') {
          return 'n';
        }
      }
    }
  }
};

int main() {
  static std::vector<pokemon> all_pokemon = get_all_pokemon();
  Battle *Pokemon = new Battle;
  Pokemon->on_entry();
  std::array<pokemon, 2> players = Pokemon->set_players(all_pokemon);
  Pokemon->game_loop(players[0], players[1]);
  delete Pokemon;
  return 0;
}
