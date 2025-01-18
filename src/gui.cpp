#include "gui.h"

void GUI::clear_screen() {
#ifndef DEBUG
  system("clear"); // Mac & Linux
#endif
}

void GUI::print_border() {
  std::string border = "";
  border += "|";
  border += std::string(INNER_WIDTH, '=');
  border += "|";
  std::cout << border << std::endl;
}

void GUI::print_item (std::string item, bool right_justify) {
  if (right_justify) {
    printf("| %*s |\n", static_cast<int>(INNER_WIDTH - INNER_PAD), item.c_str());
  } else {
    printf("| %-*s |\n", static_cast<int>(INNER_WIDTH - INNER_PAD), item.c_str());
  }
}

void GUI::print_header(std::string title, bool clear) {
  if (clear) {
    clear_screen();
  }
  print_border();
  int rightPad = (INNER_WIDTH - title.size()) / 2;
  int leftPad = INNER_WIDTH - rightPad - title.size();
  printf("|%*s%s%*s|\n", leftPad, "", title.c_str(), rightPad, "");
  print_border();
}
