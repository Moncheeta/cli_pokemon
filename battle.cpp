#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "battle.hpp"
#include "pokemon.hpp"

#include "lib/display.hpp"
#include "lib/util.hpp"

Battle::Battle(Terminal &new_term) { term = &new_term; }

Battle::~Battle() { term->clrscr(); }

std::pair<pokemon, pokemon>
Battle::set_players(const std::vector<pokemon> &list_of_pokemon) {
  bool con = true, found = false;
  unsigned int current_row = 0;
  std::string input, output;
  pokemon player;
  pokemon opponent;
  while (con == true) {
    term->write("Select a pokemon: ");
    current_row = 2;
    for (const pokemon &poke : list_of_pokemon) {
      try {
        term->write(poke.name, {1, current_row});
        ++current_row;
      } catch (...) {
        break;
      };
    }
    term->print(false);
    term->gotoxy(20, 2);
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
      term->write("You choose " + player.name + ". Are you sure? y/n",
                  {0, (unsigned int)(3 + list_of_pokemon.size()), none});
      term->print();
      term->gotoxy((unsigned int)(33 + player.name.length()),
                   (unsigned int)(4 + list_of_pokemon.size()));
      std::cin >> confirmation;
      if (tolower(confirmation) == 'y') {
        term->clrscr();
        for (const pokemon &poke : list_of_pokemon) {
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
  unsigned int selected = rand() % list_of_pokemon.size();
  opponent = list_of_pokemon[selected];
  std::pair<pokemon, pokemon> results(player, opponent);
  return results;
}

char Battle::get_input() {
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

unsigned int Battle::get_attack(pokemon &player) {
  unsigned int num_of_attacks = player.attacks.size();
  unsigned int current_sel = 0;
  bool con = true;
  while (con) {
    term->draw_quad({0, term->rows - 6}, term->cols / 2, 3);
    if (num_of_attacks >= 1) {
      if (current_sel == 0) {
        term->write(
            cut_off_string(">" + player.attacks[0].name, term->cols / 2 - 2),
            {1, term->rows - 5});
      } else {
        term->write(
            cut_off_string(" " + player.attacks[0].name, term->cols / 2 - 2),
            {1, term->rows - 5});
      }
    } else {
      if (current_sel == 0) {
        term->write(">", {1, term->rows - 5});
      } else {
        term->write(" ", {1, term->rows - 5});
      }
    }
    term->draw_quad({0, term->rows - 4}, term->cols / 2, 3);
    if (num_of_attacks >= 2) {
      if (current_sel == 1) {
        term->write(
            cut_off_string(">" + player.attacks[1].name, term->cols / 2 - 2),
            {1, term->rows - 2});
      } else {
        term->write(
            cut_off_string(" " + player.attacks[1].name, term->cols / 2 - 2),
            {1, term->rows - 2});
      }
    } else {
      if (current_sel == 1) {
        term->write(">", {1, term->rows - 2});
      } else {
        term->write(" ", {1, term->rows - 2});
      }
    }
    term->draw_quad({term->cols / 2, term->rows - 6}, term->cols / 2, 3);
    if (num_of_attacks >= 3) {
      if (current_sel == 2) {
        term->write(
            cut_off_string(">" + player.attacks[2].name, term->cols / 2 - 2),
            {(term->cols / 2) + 1, term->rows - 5});
      } else {
        term->write(
            cut_off_string(" " + player.attacks[2].name, term->cols / 2 - 2),
            {(term->cols / 2) + 1, term->rows - 5});
      }
    } else {
      if (current_sel == 2) {
        term->write(">", {(term->cols / 2) + 1, term->rows - 5});
      } else {
        term->write(" ", {(term->cols / 2) + 1, term->rows - 5});
      }
    }
    term->draw_quad({term->cols / 2, term->rows - 4}, term->cols / 2, 3);
    if (num_of_attacks >= 4) {
      if (current_sel == 3) {
        term->write(
            cut_off_string(">" + player.attacks[3].name, term->cols / 2 - 2),
            {term->cols / 2 + 1, term->rows - 2});
      } else {
        term->write(
            cut_off_string(" " + player.attacks[3].name, term->cols / 2 - 2),
            {term->cols / 2 + 1, term->rows - 2});
      }
    } else {
      if (current_sel == 3) {
        term->write(">", {(term->cols / 2) + 1, term->rows - 2});
      } else {
        term->write(" ", {(term->cols / 2) + 1, term->rows - 2});
      }
    }
    term->print(false);
    if (getch() == '\n') {
      con = false;
      continue;
    }
    getch();
    switch (getch()) {
    case 'A': // up
      if (current_sel == 1) {
        current_sel = 0;
      } else if (current_sel == 3) {
        current_sel = 2;
      }
      break;
    case 'B': // down
      if (current_sel == 0) {
        current_sel = 1;
      } else if (current_sel == 2) {
        current_sel = 3;
      }
      break;
    case 'D': // left
      if (current_sel == 2) {
        current_sel = 0;
      } else if (current_sel == 3) {
        current_sel = 1;
      }
      break;
    case 'C': // right
      if (current_sel == 0) {
        current_sel = 2;
      } else if (current_sel == 1) {
        current_sel = 3;
      }
      break;
    default:
      break;
    }
  }
  term->clear_screen();
  return current_sel;
}

void Battle::display_pokemon(pokemon &poke1, pokemon &poke2) {
  term->write(poke1.name + " (Level: " + std::to_string(poke1.level) +
              ")\nHP: " + std::to_string(poke1.health) + '\n' +
              poke1.character);
  term->write(poke2.name + " (Level: " + std::to_string(poke2.level) +
                  ")\nHP: " + std::to_string(poke2.health) + '\n' +
                  poke2.character,
              {0, 0, top_right});
}

void Battle::update_screen(pokemon &poke1, pokemon &poke2) {
  display_pokemon(poke1, poke2);
  term->print(false);
}

unsigned int Battle::game_loop(std::pair<pokemon, pokemon> &players) {
  pokemon &player = std::get<0>(players);
  pokemon &opponent = std::get<1>(players);
  while (true) {
    update_screen(player, opponent);
    char action = get_input();
    if (action == 'a') {
      unsigned int selected_attack = get_attack(player);
      try {
        player.impose_attack(opponent, player.attacks[selected_attack]);
      } catch (pokemon_died &) {
        return 0; // return who won 0 is player, 1 is enemy
      }
    } else if (action == 'r') {
      return 2; // signaling a run
    } else if (action == 'p') {
      if (opponent.health <= 10) {
        if ((rand() % 2) == 0) {
          player.collection.push_back(opponent);
          return 3; // signaling no win as opponent has left(or has been
                    // captured)
        }
      }
    }
    if (player.health <= 0) {
      return 1;
    }
    opponent.choose_action(player);
  }
}

std::pair<unsigned int, std::pair<pokemon, pokemon>>
start_battle(std::vector<pokemon> &all_pokemon, Terminal &term) {
  srand(time(NULL));
  Battle *Pokemon = new Battle(term);
  std::pair<pokemon, pokemon> players = Pokemon->set_players(all_pokemon);
  unsigned int winner = Pokemon->game_loop(players);
  std::pair<unsigned int, std::pair<pokemon, pokemon>> results =
      std::make_pair(winner, players);
  delete Pokemon;
  return results;
}
