#pragma once

#include <string>
#include <termios.h>
#include <unistd.h>

void lower_string(std::string &input) { // this is just for convienice
  for (char &letter : input) {
    letter = tolower(letter);
  }
}

std::string cut_off_string(std::string text, unsigned int max_length,
                           std::string cut_off = "...") {
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
