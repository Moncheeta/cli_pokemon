#include <iostream>

#include "lib/battle.hpp"
#include "lib/display.hpp"

int main() {
  Terminal *term = new Terminal;
  term->clrscr();
  term->write("Welcome to CLI Pokemon!\nPress enter to begin!",
              {0, 0, center_both});
  term->print();
  std::cin.get();
  term->clrscr();
  std::pair<unsigned int, std::pair<pokemon, pokemon>> results = start_battle(term);
  switch (std::get<0>(results)) {
  case 0:
    std::cout << "You won!";
    break;
  case 1:
    std::cout << "You lost. Better luck next time!";
    break;
  case 2:
    std::cout << "You have successfully ran away!";
    break;
  case 3:
    std::cout
        << "You have successfully captured "
        << std::get<1>(std::get<1>(results)).collection.back().name
        << "!"; // newly captured pokemon are appended to the back of the vector
    break;
  }
  return 0;
}
