/**
 * @file cpu.h
 * @brief CPU logic for Numbers Royale player
 * The CPU player selects a random number from the available set. 
 */
#include "player.h"

class CPU : public Player {
public:
  /**
   * @brief Initialize the CPU player logic
   */
  CPU();

  /**
   * @brief Get the CPU player name
   * 
   * @return std::string CPU name
   */
  std::string name () const override {
    return "CPU";
  }

  /**
   * @brief Select a move for the CPU player
   * 
   * Override the move function from the Player class.
   * Will ignore the move number and select a random number
   * from the moves set.
   * 
   * @param _number (ignored) the number to move
   */
  unsigned int move(unsigned int _number) override;
};
