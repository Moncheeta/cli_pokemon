#pragma once
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <vector>

enum alignment {
  center_x,
  center_y,
  center_both,
  left,
  right,
  top,
  bottom,
  top_left,
  top_right,
  top_middle,
  bottom_left,
  bottom_right,
  bottom_middle,
  none
};

enum direction { horizontal, vertical, no_direction };

struct location {
  unsigned int x = 0;
  unsigned int y = 0;
  alignment opt = none;

  location(unsigned int new_x = 0, unsigned int new_y = 0,
           alignment new_opt = none) {
    x = new_x;
    y = new_y;
    opt = new_opt;
  }
};

class Terminal {
public:
  unsigned int rows;
  unsigned int cols;

  std::vector<std::vector<char>> *display = new std::vector<std::vector<char>>;

  bool border = true;

  void gotoxy(unsigned int x, unsigned int y) {
    if (x > rows) {
      return;
    } else if (y > cols) {
      return;
    }
    printf("\033[%d;%dH", y, x);
  }

  void clrscr() { std::cout << "\033[2J\033[1;1H"; }

  void set_size(unsigned int r, unsigned int c) {
    if (r > 0 && c > 0) {
      rows = r;
      cols = c;
      display->clear();
      for (unsigned int current_row = 0; current_row != rows; current_row++) {
        std::vector<char> temp;
        for (unsigned int current_col = 0; current_col != cols; current_col++) {
          if (current_col == 0 || current_col == cols - 1) {
            temp.push_back('|');
          } else if (current_row == 0 || current_row == rows - 1) {
            temp.push_back('-');
          } else {
            temp.push_back(' ');
          }
        }
        display->push_back(temp);
      }
    }
  }

  Terminal(unsigned int c = 0, unsigned int r = 0) {
    if (r == 0 || c == 0) {
      struct winsize w;
      ioctl(0, TIOCGWINSZ, &w);
      if (r == 0) {
        r = w.ws_row - 1;
      }
      if (c == 0) {
        c = w.ws_col;
      }
    }
    set_size(r, c);
  }

  void write(std::string text, location loco = {0, 0, none},
             bool extend = false) {
    auto n = text.find_first_of('\n');
    if (n != std::string::npos) {
      extend = true;
    }
    switch (loco.opt) {
    case center_x:
      if (n != std::string::npos) {
        loco.x = ((cols - text.substr(0, n).length()) / 2);
        break;
      } else {
        loco.x = (cols - text.length()) / 2;
        break;
      }
    case center_y:
      loco.y = rows / 2;
      break;
    case center_both:
      if (n != std::string::npos) {
        loco.x = ((cols - text.substr(0, n).length()) / 2);
      } else {
        loco.x = (cols - text.length()) / 2;
      }
      loco.y = rows / 2;
      break;
    case left:
      loco.x = 0;
      break;
    case right:
      if (n != std::string::npos) {
        loco.x = border ? cols - text.substr(0, n).length() - 1
                        : cols - text.substr(0, n).length();
      } else {
        loco.x = border ? cols - text.length() - 1 : cols - text.length();
      }
      break;
    case top:
      loco.y = 0;
      break;
    case bottom:
      loco.y = border ? rows - 1 : rows;
      break;
    case top_left:
      loco.x = 0;
      loco.y = 0;
      break;
    case top_right:
      if (n != std::string::npos) {
        loco.x = border ? cols - text.substr(0, n).length() - 1
                        : cols - text.substr(0, n).length();
      } else {
        loco.x = border ? cols - text.length() - 1 : cols - text.length();
      }
      loco.y = 0;
      break;
    case top_middle:
      if (n != std::string::npos) {
        loco.x = (cols - text.substr(0, n).length()) / 2;
      } else {
        loco.x = (cols - text.length()) / 2;
      }
      loco.y = 0;
      break;
    case bottom_left:
      loco.x = 0;
      loco.y = border ? rows - 1 : rows;
      break;
    case bottom_right:
      if (n != std::string::npos) {
        loco.x = border ? cols - text.substr(0, n).length() - 1
                        : cols - text.substr(0, n).length();
      } else {
        loco.x = border ? cols - text.length() - 1 : cols - text.length();
      }
      loco.y = border ? rows - 1 : rows;
      break;
    case bottom_middle:
      if (n != std::string::npos) {
        loco.x = (cols - text.substr(0, n).length()) / 2;
      } else {
        loco.x = (cols - text.length()) / 2;
      }
      loco.y = border ? rows - 1 : rows;
      break;
    default:
      break;
    }
    if ((n != std::string::npos) ||
        (border && (loco.x + text.length() > cols - 1)) ||
        (!border && (loco.x + text.length() > cols))) {
      if (extend) {
        location new_loco = loco;
        switch (new_loco.opt) {
        case center_both:
          new_loco.opt = center_x;
          break;
        case center_y:
          new_loco.opt = none;
          break;
        case top:
          new_loco.opt = none;
          break;
        case top_left:
          new_loco.opt = left;
          break;
        case top_right:
          new_loco.opt = right;
          break;
        case top_middle:
          new_loco.opt = center_x;
          break;
        case bottom:
        case bottom_left:
        case bottom_right:
        case bottom_middle:
          for (auto it = text.rbegin();
               it - text.rbegin() != (text.end() - text.begin()) - 3; it++) {
            *it = '.';
          }
          break;
        default:
          break;
        }
        if ((new_loco.opt != bottom) && (new_loco.opt != bottom_left) &&
            (new_loco.opt != bottom_right) && (new_loco.opt != bottom_middle)) {
          if (new_loco.y == 0) {
            ++new_loco.y;
          }
          ++new_loco.y;
          if (n != std::string::npos) {
            write(text.substr(n + 1), new_loco, true);
            text = text.substr(0, n);
          } else {
            write(text.substr(cols - 2), new_loco, true);
            text = text.substr(0, rows - 2);
          }
        }
      }
    }
    if (loco.x == 0 && border) {
      ++loco.x;
    }
    if (loco.y == 0 && border) {
      ++loco.y;
    }
    if ((loco.y > rows) || ((loco.y == rows) && border)) {
      return;
    }
    if ((loco.x > cols) || ((loco.x == cols) && border)) {
      return;
    }
    for (auto text_pos = text.begin(); text_pos != text.end(); text_pos++) {
      (*display)[loco.y][loco.x + (text_pos - text.begin())] = *text_pos;
    }
  }
  void draw_line(location loco = {0, 0, none}, unsigned int length = 0,
                 direction dir = no_direction) {
    if (length == 0) {
      return; // no point
    }
    if (dir == horizontal) {
      for (unsigned int current_x = 0; loco.x + current_x != loco.x + length;
           current_x++) {
        if (loco.x + current_x == 0 || loco.x + current_x >= cols) {
          continue;
        }
        (*display)[loco.y][loco.x + current_x] = '-';
      }
    } else if (dir == vertical) {
      for (unsigned int current_y = 0; loco.y + current_y != loco.y + length;
           current_y++) {
        if (loco.y + current_y == 0 || loco.y + current_y >= rows) {
          continue;
        }
        (*display)[loco.y + current_y][loco.x] = '|';
      }
    }
  }

  void draw_quad(location loco = {0, 0, none}, unsigned int length_x = 0,
                 unsigned int length_y = 0) {
    if (length_x == 0 || length_y == 0) {
      return;
    }
    draw_line(location(loco.x, loco.y, none), length_x, horizontal);
    draw_line(location(loco.x, loco.y, none), length_y, vertical);
    draw_line(location(loco.x, loco.y + length_y, none), length_x, horizontal);
    draw_line(location(loco.x + length_x, loco.y, none), length_y, vertical);
  }

  void print(bool clear = true) {
    std::cout << std::flush;
    for (auto current_row = display->begin(); current_row != display->end();
         current_row++) {
      for (auto current_col = current_row->begin();
           current_col != current_row->end(); current_col++) {
        if ((current_col - current_row->begin()) ==
            ((current_row->end() - current_row->begin()) - 1)) {
          std::cout << *current_col << '\n';
        } else {
          std::cout << *current_col;
        }
      }
    }
    if (clear) {
      clear_screen();
    }
  }

  void clear_screen() {
    if (!border) {
      for (std::vector<char> &row : *display) {
        for (char &character : row) {
          character = ' ';
        }
      }
    } else {
      for (auto current_row = display->begin(); current_row != display->end();
           current_row++) {
        if (current_row - display->begin() == 0 ||
            current_row - display->begin() == rows - 1) {
          continue;
        }
        for (auto current_col = current_row->begin();
             current_col != current_row->end(); current_col++) {
          if (current_col - current_row->begin() == 0 ||
              current_col - current_row->begin() == cols - 1) {
            continue;
          } else {
            *current_col = ' ';
          }
        }
      }
    }
  }
};
