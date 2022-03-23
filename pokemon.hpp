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
};

std::vector<pokemon> get_all_pokemon() {
  std::vector<pokemon> all_pokemon;
  for (const std::experimental::filesystem::directory_entry &file :
       std::experimental::filesystem::directory_iterator("./pokemon")) {
    std::ifstream pokemon_file(file.path(), std::ifstream::in);
    json data;
    pokemon_file >> data;
    pokemon_file.close();
    std::vector<attack> new_pokemon_attacks;
    for (auto &attackinjson : data["Attacks"]) {
      attack new_attack = {attackinjson["Name"], attackinjson["Damage"]};
      new_pokemon_attacks.push_back(new_attack);
    }
    pokemon new_pokemon(data["Name"], 0, data["Health"], data["Character"],
                        new_pokemon_attacks);
    all_pokemon.push_back(new_pokemon);
  }
  return all_pokemon;
}
