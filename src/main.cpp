#include "numbers_royale.h"

int main() {
  GUI gui;
  NumbersRoyale game(&gui);

  // Enter game loop
  game.main_menu();

  return 0;
}
