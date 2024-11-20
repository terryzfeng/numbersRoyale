#include <iostream>
#include <ctime>
#include <random>
#include "cpu.h"

CPU::CPU() : Player(true) {
  std::srand((unsigned) time(0));
}

unsigned int CPU::move(unsigned int _number) {
  unsigned int move = 0;
  if (moves_.empty()) {
    return 0;
  }
  // Select random move
  do {
    move = (std::rand() % board_size_) + 1;
  } while (!is_valid_move(move));
  // Play the move via player class
  return Player::move(move);
}
