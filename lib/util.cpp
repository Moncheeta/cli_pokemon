#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "../pokemon.hpp"

#include "util.hpp"

void lower_string(std::string &input) { // this is just for convienice
  for (char &letter : input) {
    letter = tolower(letter);
  }
}

std::string cut_off_string(std::string text, unsigned int max_length,
                           std::string cut_off) {
  if (text.length() < max_length) {
    return text;
  }
  return text.substr(max_length - cut_off.length()) + cut_off;
}

char getch() {
  char buf = 0;
  struct termios old;
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

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
