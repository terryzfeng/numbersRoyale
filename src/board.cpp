#include <iostream>
#include "board.h"
#include "cpu.h"


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
  std::vector<Player*> sorted_player_view;
  for (const std::unique_ptr<Player>& player : players()) {
      sorted_player_view.push_back(player.get()); // Add raw pointers to the view
  }

  std::sort(sorted_player_view.begin(), sorted_player_view.end(), score_cmp);

  for (int i = 0; i < get_num_players(); ++i) {
    std::string player_score_text = sorted_player_view[i]->name() + ": " +
        std::to_string(sorted_player_view[i]->get_score()) + " points";
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
  std::vector<Player*> sorted_players_view;
  for (const std::unique_ptr<Player>& player : players_) {
    sorted_players_view.push_back(player.get());
  }
  std::sort(sorted_players_view.begin(), sorted_players_view.end(), move_cmp);

  for (int i = 0; i < get_num_players(); ++i) {
    std::string player_move_text = sorted_players_view[i]->name() + " move: " +
        std::to_string(sorted_players_view[i]->get_last_move());
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

  for (int i = 0; i < get_num_players(); ++i) {
    unsigned int current_move = players_[i]->get_last_move();
    if (current_move > biggest_move) {
      biggest_move = current_move;
      winner_id = players_[i]->id();
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
