#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <typeinfo>

#include "lib/json.hpp"

using namespace nlohmann;

struct attack {
  std::string name;
  unsigned int damage;
  std::string desc;

  attack(std::string new_name, unsigned int new_damage,
         std::string new_desc = "None") {
    name = new_name;
    damage = new_damage;
    desc = new_desc;
  }
};

struct pokemon {
  std::string name;
  unsigned int level;
  int health;

  std::string character = "";

  std::vector<attack> attacks;

  pokemon(std::string new_name = "Not Set", unsigned int new_level = 0, int new_health = 0, 
  std::string new_character = "There Is No\nCharacter", std::vector<attack> new_attacks = std::vector<attack>(1, attack("None", 0))) {
    name = new_name;
    level = new_level;
    health = new_health;
    character = new_character;
    attacks = new_attacks;
  }
};

std::vector<pokemon> get_all_pokemon() {
  std::vector<pokemon> all_pokemon;
  for (const std::experimental::filesystem::directory_entry &file : std::experimental::filesystem::directory_iterator("./pokemon")) {
    json data;
    std::ifstream pokemon_file(file.path(), std::ifstream::in);
    pokemon_file >> data;
    std::vector<attack> new_pokemon_attacks;
    for (auto &attackinjson : data["Attacks"]) {
      attack new_attack = { attackinjson["Name"], attackinjson["Damage"], attackinjson["Desc"] };
      new_pokemon_attacks.push_back(new_attack);
    }
    pokemon new_pokemon(data["Name"], 0, data["Health"], data["Character"], new_pokemon_attacks);
    all_pokemon.push_back(new_pokemon);
    pokemon_file.close();
  }
  return all_pokemon;
}
