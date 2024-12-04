#include "player.h"
#include <iostream>

namespace {
  // Helper function
  /**
   * @brief Trim trailing whitespace from a string
   * 
   * @param s string to trim
   */
  static void rtrim(std::string& s) {
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
  } 
};

Player::Player(): id_(0), score_(0), last_move_(0), board_size_(0) {}

void Player::init(unsigned int id, unsigned int board_size) {
  id_ = id;
  board_size_ = board_size;
  score_ = 0;
  last_move_ = 0;
  moves_.clear();
  for (int i = 1; i <= board_size; ++i) {
    moves_.insert(i);
  }
}

bool Player::is_valid_move(unsigned int number) {
  return (moves_.find(number) != moves_.end());
}

unsigned int Player::move(unsigned int number) {
  if (!is_valid_move(number)) {
    return 0;
  }
  moves_.erase(number);
  last_move_ = number;
  return number;
}

std::string Player::get_moves_string() {
  std::vector<bool> moves(board_size_);
  std::string result = "";
  // Convert to boolean vector
  for (unsigned int number : moves_) {
    moves[number - 1] = true;
  }
  // Build moves string
  for (int i = 0; i < board_size_; ++i) {
    if (moves[i]) {
      result += std::to_string(i + 1);
    } else {
      result += "_";
    }
    result += "  ";
  }
  rtrim(result);
  return result;
}
