#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "lib/json.hpp"

using namespace nlohmann;

struct pokemon_died : public std::exception {
  const char *what() const throw() { return "Pokemon Died"; }
};

struct attack {
  std::string name;
  unsigned int damage;

  attack(std::string new_name, unsigned int new_damage) {
    name = new_name;
    damage = new_damage;
  }
};

struct pokemon {
  std::string name;
  unsigned int level;
  int health;

  std::string character = "";

  std::vector<attack> attacks;

  std::vector<pokemon> collection;

  pokemon(std::string new_name = "Not Set", unsigned int new_level = 0,
          int new_health = 0,
          std::string new_character = "There Is No\nCharacter",
          std::vector<attack> new_attacks =
              std::vector<attack>(1, attack("None", 0)),
          std::vector<pokemon> new_collection = {}) {
    name = new_name;
    level = new_level;
    health = new_health;
    character = new_character;
    attacks = new_attacks;
    collection = new_collection;
  }

  void impose_attack(pokemon &enemy, attack chosen_attack) {
    enemy.health = enemy.health - chosen_attack.damage;
    if (enemy.health <= 0) {
      throw pokemon_died();
    }
  }

  void choose_action(pokemon &opponent) {
    attack &chosen_attack = attacks[rand() % attacks.size()];
    impose_attack(opponent, chosen_attack);
  }
};
