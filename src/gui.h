/**
 * @file gui.h
 * @brief Simple CLI GUI for Numbers Royale
 * Prints a border for the menu, items, and headers
 */
#include <iostream>
#include <string>

class GUI {
public:
  static constexpr size_t BOARD_WIDTH = 35;
  static constexpr size_t INNER_PAD = 2;
  static constexpr size_t INNER_WIDTH = BOARD_WIDTH - INNER_PAD;

  /**
   * @brief Clear the console
   * 
   * Will do nothing if DEBUG flag is defined
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
  static void print_item(std::string item, bool right_justify = false);

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
  static void print_header(std::string title, bool clear = true);
};
