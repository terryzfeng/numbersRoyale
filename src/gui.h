/**
 * @file gui.h
 * @brief Simple CLI GUI for Numbers Royale
 * Prints a border for the menu, items, and headers
 */
#include <iostream>

class GUI {
public:
  static constexpr int BOARD_WIDTH = 35;
  static constexpr int INNER_PAD = 2;
  static constexpr int INNER_WIDTH = BOARD_WIDTH - INNER_PAD;

  /**
   * @brief Clear the console
   */
  static void clear_screen();

  /**
   * @brief Print the top border
   * 
   * "|=================================|"
   */
  static void print_border();

  /**
   * @brief Print an item with left and right edges and padding
   * 
   * default:
   * "| 1. PLAY GAME                   |"
   * 
   * if right_justify is true:
   * "|                   1. PLAY GAME |"
   * 
   * @param item The string to print
   * @param left_justify Whether to left justify the text
   */
  static void print_item(const char* item, bool right_justify = false);

  /**
   * @brief Print a header with the centered title
   * 
   * "|=================================|"
   * "|         NUMBERS ROYALE          |"
   * "|=================================|"
   * 
   * @param title The title to print
   * @param clear Whether to clear the screen before printing
   */
  static void print_header(const char* title, bool clear = true);
};
