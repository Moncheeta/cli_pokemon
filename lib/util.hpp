#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

#include "../pokemon.hpp"

void lower_string(std::string &input); // this is just for convienice

std::string cut_off_string(std::string text, unsigned int max_length,
                           std::string cut_off = "...");

char getch();

std::vector<pokemon> get_all_pokemon();
