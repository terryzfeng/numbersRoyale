/**
 * @file numbers_royale.h
 * @brief Game loop for Numbers Royale
 */
#include <iostream>
#include <functional>
#include "board.h"

class NumbersRoyale {
public:
  Board board_;

  enum class MainMenuOption {
    PLAY_GAME = 1,
    HOW_TO_PLAY,
    QUIT
  };

  enum class PlayModeOption {
    PLAYER_VS_PLAYER = 1,
    PLAYER_VS_CPU,
    RETURN_TO_MAIN_MENU
  };

  enum class BoardSizeOption {
    SMALL = 1,
    MEDIUM,
    LARGE,
    RETURN_TO_MAIN_MENU 
  };

  /**
   * @brief Convert board option to size
   * 
   * @param option int for a BoardSizeOption enum
   * @return unsigned int board size
   */
  static constexpr unsigned int board_option_to_size(unsigned int option) {
    return (option * 2) + 3; // magic number teehee
  }

  NumbersRoyale();

  void main_menu();
  void how_to_play();
  void play_mode();
  void select_board_size();
  void play_game();
};
