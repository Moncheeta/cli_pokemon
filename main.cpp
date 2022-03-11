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

  unsigned int get_attack(pokemon player) {
    std::string output;
    std::cout << "Which attack do you wan't to use? ";
    for (auto attack_iter = player.attacks.begin(); attack_iter != player.attacks.end(); ++attack_iter) {
      std::cout << attack_iter - player.attacks.begin() << ": " << attack_iter->name << ", ";
    }
    std::cin >> output;
    return stoi(output);
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

  unsigned int game_loop(pokemon &player, pokemon &opponent) {
    while (true) {
      update_screen(player, opponent);
      char action = get_input();
      if (action == 'a') {
        unsigned int selected_attack = get_attack(player);
        try {
          player.impose_attack(opponent, player.attacks[selected_attack]);
        }
        catch (pokemon_died&) {
          return 0;
        }
      }
      else if (action == 'r') {
        return 2; // signaling a run
      }
      else if (action == 'p') {
        if (opponent.health <= 10) {
          if ((rand() % 2) == 0) {
            player.collection.push_back(opponent);
            return 3; // signaling no win as opponent has left(or has been captured)
          }
        }
      }

      if (player.health <= 0) {
        return 1;
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
