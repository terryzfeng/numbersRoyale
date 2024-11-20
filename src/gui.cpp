#include "gui.h"

void GUI::clear_screen() {
  // Mac/Linux
#ifndef DEBUG
  system("clear");
#endif
}

void GUI::print_border() {
  char border[BOARD_WIDTH + 1] = "|";
  for (int i = 1; i < BOARD_WIDTH - 1; ++i) {
    border[i] = '=';
  }
  border[BOARD_WIDTH - 1] = '|';
  printf("%s\n", border);
}

void GUI::print_item (const char* item, bool right_justify) {
  if (right_justify) {
    printf("| %*s |\n", INNER_WIDTH - INNER_PAD, item);
  } else {
    printf("| %-*s |\n", INNER_WIDTH - INNER_PAD, item);
  }
}

void GUI::print_header(const char* title, bool clear) {
  if (clear) {
    clear_screen();
  }
  print_border();
  int rightPad = (INNER_WIDTH - strlen(title)) / 2;
  int leftPad = INNER_WIDTH - rightPad - strlen(title);
  printf("|%*s%s%*s|\n", leftPad, "", title, rightPad, "");
  print_border();
}
