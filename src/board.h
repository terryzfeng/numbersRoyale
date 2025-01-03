/**
 * @file board.h
 * @brief Board state for Numbers Royale
 */
#include <vector>
#include <string>
#include <memory>
#include "player.h"
#include "gui.h"


class Board
{
private:
  std::vector<std::unique_ptr<Player>> players_;
  unsigned int num_humans_;
  unsigned int board_size_;

public:
  // Globals
  static constexpr size_t DEFAULT_BOARD_SIZE = 9;
  static constexpr int CONTINUE_GAME = -1;
  static constexpr int TIE_GAME = 0;

  /**
   * @brief Construct a new Board object
   * 
   * @param board_size size of board
   */
  Board(size_t board_size = DEFAULT_BOARD_SIZE);

  /**
   * @brief Destruct the Board object
   * 
   * Delete all players
   */
  ~Board() { reset(); }

  /**
   * @brief Initialize players and board state
   * 
   * If new_board_size is 0, the board will keep the current size
   * 
   * @param new_board_size to reset to
   */
  void init(size_t new_board_size = 0);

  /**
   * @brief Remove all players from the board
   */
  void reset();

  /**
   * @brief (debug) Print the board state and all player's moves
   */
  void print_board() const;

  /**
   * @brief Print scoreboard for all players
   */
  void print_scores() const;

  /**
   * @brief Print round results
   */
  void print_round_result();

  /**
   * @brief Print the moves of a specific player id
   * 
   * Nothing will be printed if the player id is invalid
   * 
   * @param player_id the id of the player to print moves for
   */
  void print_player_moves(unsigned int player_id) const;

  /**
   * @brief Get the board players
   * 
   * @return Players vector
   */
  const std::vector<std::unique_ptr<Player>>& players() const {
    return players_;
  }

  /**
   * @brief Get the number of players
   * 
   * @return unsigned int number of players
   */
  unsigned int get_num_players() const {
    return players_.size();
  }

  /**
   * @brief Get number of human players
   * 
   * @return unsigned int number of humans
   */
  unsigned int get_num_humans() const {
    return num_humans_;
  }

  /**
   * @brief Add a player to the board
   * 
   * @param is_cpu Whether the player is a CPU player
   */
  void add_player(bool is_cpu = false);

  /**
   * @brief Check if game over and return winner option or winning player id
   * 
   * @return TIE_GAME if a tie, CONTIUE_GAME if game is not over, or winning player id
   */
  int check_game_winner() const;

  /**
   * @brief Return player id with highest move
   * 
   * Will also increment the score of the winning player
   * Note: For 2+ players, winning ties are not counted as a win
   * 
   * @return int player id of winner, 0 if tie
   */
  unsigned int check_round_winner();
};
