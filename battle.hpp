#pragma once

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "pokemon.hpp"
#include "lib/display.hpp"
#include "lib/util.hpp"

class Battle {
private:
  Terminal *term = nullptr;

public:
  Battle(Terminal &term);
  ~Battle();

  std::pair<pokemon, pokemon>
  set_players(const std::vector<pokemon> &list_of_pokemon);

  char get_input();
  unsigned int get_attack(pokemon &player);

  void display_pokemon(pokemon &poke1, pokemon &poke2);

  void update_screen(pokemon &poke1, pokemon &poke2);

  unsigned int game_loop(std::pair<pokemon, pokemon> &);
};

std::pair<unsigned int, std::pair<pokemon, pokemon>>
start_battle(std::vector<pokemon> &, Terminal &);
