/**
 * @file player.h
 * @brief Define a player for Numbers Royale
 * Each player has a moves set and a score.
 * Once a number is played, it is deleted from the moves set.
 */
#pragma once
#include <unordered_set>
#include <array>
#include <string>


class Player {
protected:
  std::unordered_set<unsigned int> moves_;
  unsigned int id_;
  unsigned int score_;
  unsigned int last_move_;
  unsigned int board_size_;

public:
  /**
   * @brief Construct a new Player 
   */
  Player();
  virtual ~Player() = default;

  /**
   * @brief Initialize the player with id and board size
   * 
   * @param id player id
   * @param board_size size of the board
   */
  void init(unsigned int id, unsigned int board_size);

  /**
   * @brief Check if a move is valid
   * 
   * Verify the number is in the player's set of moves
   * 
   * @param number number to try to move
   * @return true if move is valid, false otherwise
   */
  bool is_valid_move(unsigned int number);

  /**
   * @brief Play a move for the player
   * 
   * The number played must be in the player's set of available moves
   * After the move is played, the number is removed from the set
   * 
   * @param number the number to move
   * @return the number moved or 0 if the move is invalid
   */
  virtual unsigned int move(unsigned int number);

  /**
   * @brief Get the player's available moves as a string
   * 
   * Underscore represents moves that have been played
   * Print with 2 spaces between each number
   * 
   * Example:
   * "1  2  3  4  _  6  _  8  9"
   */
  std::string get_moves_string();

  /**
   * @brief Get the player id (1-indexed)
   * 
   * @return the player number
   */
  unsigned int id() const {
    return id_;
  }

  /**
   * @brief Get the player name
   * 
   * @return std::string player name
   */
  virtual std::string name() const {
    return "P" + std::to_string(id_);
  }

  /**
   * @brief Get the player's last played move
   * 
   * @return player's last move int
   */
  unsigned int get_last_move() const {
    return last_move_;
  }

  /**
   * @brief Get the player's moves set
   * 
   * @return the player's moves set
   */
  const std::unordered_set<unsigned int>& get_moves() const {
    return moves_;
  }

  /**
   * @brief Increment the player's score
   */
  void inc_score() {
    score_++;
  }

  /**
   * @brief Get the player's score
   */
  unsigned int get_score() const {
    return score_;
  }

  /**
   * @brief Return whether Player is a CPU
   *
   * CPU class should override and return true
   *
   * @return false Player class is not a CPU
   */
  virtual bool is_cpu() const {
    return false;
  }
};
