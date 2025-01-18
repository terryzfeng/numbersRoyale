#include <iostream>
#include "board.h"
#include "cpu.h"

namespace {
  // Comparator functions
  // Sort by descending score
  static bool score_cmp(const Player* a, const Player* b) {
    return a->get_score() > b->get_score();
  }
  // Sort by last move, descending
  static bool move_cmp(const Player* a, const Player* b) {
    return a->get_last_move() > b->get_last_move();
  }

  /**
   * @brief Sort players by a custom comparator function
   * 
   * @param players players vector
   * @param cmp Player comparator function
   * @return std::vector<const Player*> sorted players view
   */
  static std::vector<const Player*> sort_players(
      const std::vector<std::unique_ptr<Player>>& players,
      std::function<bool(const Player*, const Player*)> cmp) {
    std::vector<const Player*> sorted_players_view(players.size());
    for (const std::unique_ptr<Player>& player : players) {
      sorted_players_view.push_back(player.get()); // Add raw pointers to the view
    }
    std::sort(sorted_players_view.begin(), sorted_players_view.end(), cmp);
    return sorted_players_view;
  }
};

Board::Board(size_t board_size) : players_(), num_humans_(0), board_size_(board_size) {}

void Board::init(size_t new_board_size) {
  // If board_size is 0, keep the current size
  if (new_board_size > 0) {
    board_size_ = new_board_size;
  }
  for (unsigned int i = 0; i < get_num_players(); ++i) {
    // Player ids are 1-indexed
    players_[i]->init(i + 1, board_size_);
  }
}

void Board::reset() {
  players_.clear();
  num_humans_ = 0;
}

void Board::print_board() const {
  GUI::print_header("NUMBERS ROYALE");
  // Print players in reverse, so player 1 is on the bottom
  for (int i = get_num_players() - 1; i >= 0; --i) {
    std::string playerText = players_[i]->name() + ": " +
        std::to_string(players_[i]->get_score()) + " points";
    GUI::print_item(playerText.c_str());
    GUI::print_item("Moves:");
    GUI::print_item(players_[i]->get_moves_string().c_str(), true);
  }
  GUI::print_border();
}

void Board::print_scores() const {
  int winner_id = check_game_winner();
  switch (winner_id) {
    case TIE_GAME:
      GUI::print_header("TIE GAME!");
      break;
    case CONTINUE_GAME:
      GUI::print_header("SCOREBOARD", false);
      break;
    default:
      std::string winner_text = players_[winner_id - 1]->name() + " WINS!";
      GUI::print_header(winner_text.c_str());
      break;
  }
  // Sort players by descending score
  std::vector<const Player*> sorted_player_view = sort_players(players(), score_cmp);
  for (const Player* sorted_player : sorted_player_view) {
    std::string player_score_text = sorted_player->name() + ": " +
        std::to_string(sorted_player->get_score()) + " points";
    GUI::print_item(player_score_text.c_str());
  }
  GUI::print_border();
}

void Board::print_round_result() {
  unsigned int winner_id = check_round_winner(); 

  // If game is over, print final score and exit
  if (check_game_winner() != CONTINUE_GAME) {
    print_scores();
    return;
  }

  if (winner_id == TIE_GAME) {
    GUI::print_header("It's a tie!");
  } else {
    std::string winner_text = players_[winner_id - 1]->name() + " wins the round!";
    GUI::print_header(winner_text.c_str());
  }

  // Sort players by last move, descending
  std::vector<const Player*> sorted_players_view = sort_players(players(), move_cmp);

  for (const Player* sorted_player : sorted_players_view) {
    std::string player_move_text = sorted_player->name() + " move: " +
        std::to_string(sorted_player->get_last_move());
    GUI::print_item(player_move_text.c_str());
  }

  // Print the current scores
  print_scores();
}

void Board::print_player_moves(unsigned int player_id) const {
  if (player_id == 0 || player_id > get_num_players()) {
    return;
  }
  GUI::print_header("NUMBERS ROYALE");
  const std::unique_ptr<Player>& player = players_[player_id - 1];
  std::string player_move_text = player->name() + " moves:";
  GUI::print_item(player_move_text.c_str());
  GUI::print_item(player->get_moves_string().c_str(), true);
  GUI::print_border();
}

void Board::add_player(bool is_cpu) {
  if (is_cpu) {
    players_.push_back(std::make_unique<CPU>());
  } else {
    players_.push_back(std::make_unique<Player>());
    num_humans_++;
  }
}

int Board::check_game_winner() const {
  if (get_num_players() == 0) {
    return TIE_GAME;
  }

  int toWin = board_size_ / get_num_players();
  unsigned int max_score = 0;
  unsigned int winner_id = 0;
  bool tie = false;

  for (const std::unique_ptr<Player>& player : players_) {
    // Plurality check
    if (player->get_score() > toWin) {
      return player->id();
    }
    // Highest score check
    if (player->get_score() > max_score) {
      max_score = player->get_score();
      winner_id = player->id();
      tie = false;
    } else if (player->get_score() == max_score) {
      tie = true;
    }
  }

  // If no more moves
  if (players_[0]->get_moves().empty()) {
    return tie ? TIE_GAME : winner_id;
  }

  // Game is not over
  return CONTINUE_GAME;
}

unsigned int Board::check_round_winner() {
  unsigned int winner_id = 0;
  unsigned int biggest_move = 0;
  bool tie = false;

  for (const std::unique_ptr<Player>& player : players_) {
    unsigned int current_move = player->get_last_move();
    if (current_move > biggest_move) {
      biggest_move = current_move;
      winner_id = player->id();
      tie = false;
    } else if (current_move == biggest_move) {
      tie = true;
    }
  }

  // After all moves are checked, check for a tie
  if (tie) {
    return 0;
  }

  // Increment the score of the round winner
  players_[winner_id - 1]->inc_score();
  return winner_id;
}
