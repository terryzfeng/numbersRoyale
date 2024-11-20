/**
 * @file board.h
 * @brief Board state for Numbers Royale
 */
#include <vector>
#include <string>
#include "player.h"
#include "gui.h"


class Board
{
private:
  std::vector<Player*> players_;
  GUI* gui_;
  unsigned int num_humans_;
  unsigned int board_size_;

public:
  // Globals
  static const int DEFAULT_BOARD_SIZE = 9;

  // Comparator functions
  // Sort by descending score
  static bool score_cmp(Player* a, Player* b) {
    return a->get_score() > b->get_score();
  }
  // Sort by last move, descending
  static bool move_cmp(Player* a, Player* b) {
    return a->get_last_move() > b->get_last_move();
  }

  /**
   * @brief Construct a new Board object
   * 
   * @param gui GUI object to handle display
   * @param board_size size of board
   */
  Board(GUI *gui = nullptr, int board_size = DEFAULT_BOARD_SIZE);

  /**
   * @brief Destruct the Board object
   * 
   * Delete all players
   */
  ~Board() { reset(); }

  /**
   * @brief Initialize players and board state
   * 
   * If board_size is 0, the board will keep the current size
   * 
   * @param board_size to reset to
   */
  void init(unsigned int board_size = 0);

  /**
   * @brief Remove all players from the board
   */
  void reset();

  /**
   * @brief (debug) Print the board state and all player's moves
   */
  void print_board();

  /**
   * @brief Print scoreboard for all players
   */
  void print_scores();

  /**
   * @brief Print round results
   */
  void print_round_result();

  /**
   * @brief Print the moves of a specific player id
   * 
   * @param player_id the id of the player to print moves for
   */
  void print_player_moves(unsigned int player_id);

  /**
   * @brief Get the board players
   * 
   * @return Players vector
   */
  std::vector<Player*>& players() {
    return players_;
  }

  /**
   * @brief Get the number of players
   * 
   * @return unsigned int number of players
   */
  unsigned int num_players() const {
    return players_.size();
  }

  /**
   * @brief Get number of human players
   * 
   * @return unsigned int number of humans
   */
  unsigned int num_humans() const {
    return num_humans_;
  }

  /**
   * @brief Add a player to the board
   * 
   * @param is_cpu Whether the player is a CPU player
   */
  void add_player(bool is_cpu = false);

  /**
   * @brief Check if game over and return winner id
   * 
   * @return int id of the winner or 0 if a tie, -1 if game is not over
   */
  int check_game_winner();

  /**
   * @brief Return player id with highest move
   * 
   * Will also increment the score of the winning player
   * 
   * @return int player id of winner, 0 if tie
   */
  int check_round_winner();
};
