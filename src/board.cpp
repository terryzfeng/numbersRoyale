#include <iostream>
#include <vector>
#include <string>
#include "board.h"
#include "cpu.h"


Board::Board(GUI *gui, int board_size) : gui_(gui), board_size_(board_size) {
  num_humans_ = 0;
}

void Board::init(unsigned int board_size) {
  if (board_size > 0) {
    board_size_ = board_size;
  }
  for (int i = 0; i < num_players(); ++i) {
    // Player ids are 1-indexed
    players_[i]->init(i + 1, board_size_);
  }
}

void Board::reset() {
  for (Player* player : players_) {
    delete player;
  }
  num_humans_ = 0;
  players_.clear();
}

void Board::print_board() {
  if (gui_ == nullptr) {
    std::cerr << "No GUI object" << std::endl;
    return;
  }

  gui_->print_header("NUMBERS ROYALE");
  // Print players in reverse, so player 1 is on the bottom
  for (int i = num_players() - 1; i >= 0; --i) {
    std::string playerText = players_[i]->name() + ": " +
        std::to_string(players_[i]->get_score()) + " points";
    gui_->print_item(playerText.c_str());
    gui_->print_item("Moves:");
    gui_->print_item(players_[i]->get_moves_string().c_str(), true);
  }
  gui_->print_border();
}

void Board::print_scores() {
  if (gui_ == nullptr) {
    std::cerr << "No GUI object" << std::endl;
    return;
  }

  int winner_id = check_game_winner();
  switch (winner_id) {
    case 0:
      gui_->print_header("TIE GAME!");
      break;
    case -1:
      gui_->print_header("SCOREBOARD", false);
      break;
    default:
      std::string winner_text = players_[winner_id - 1]->name() + " WINS!";
      gui_->print_header(winner_text.c_str());
      break;
  }
  // Sort players by descending score
  std::vector<Player*> sorted_players = players_;
  std::sort(sorted_players.begin(), sorted_players.end(), score_cmp);

  for (int i = 0; i < num_players(); ++i) {
    std::string player_score_text = sorted_players[i]->name() + ": " +
        std::to_string(sorted_players[i]->get_score()) + " points";
    gui_->print_item(player_score_text.c_str());
  }
  gui_->print_border();
}

void Board::print_round_result() {
  if (gui_ == nullptr) {
    std::cerr << "No GUI object" << std::endl;
    return;
  }

  int winner_id = check_round_winner(); 

  // If game is over, print final score and exit
  if (check_game_winner() != -1) {
    print_scores();
    return;
  }

  if (winner_id == 0) {
    gui_->print_header("It's a tie!");
  } else {
    std::string winner_text = players_[winner_id - 1]->name() + " wins the round!";
    gui_->print_header(winner_text.c_str());
  }

  // Sort players by last move, descending
  std::vector<Player*> sorted_players = players_;
  std::sort(sorted_players.begin(), sorted_players.end(), move_cmp);

  for (int i = 0; i < num_players(); ++i) {
    std::string player_move_text = sorted_players[i]->name() + " move: " +
        std::to_string(sorted_players[i]->get_last_move());
    gui_->print_item(player_move_text.c_str());
  }

  // Print the current scores
  print_scores();
}

void Board::print_player_moves(unsigned int player_id) {
  if (gui_ == nullptr) {
    std::cerr << "No GUI object" << std::endl;
    return;
  }

  gui_->print_header("NUMBERS ROYALE");
  Player* player = players_[player_id - 1];
  std::string player_move_text = player->name() + " moves:";
  gui_->print_item(player_move_text.c_str());
  gui_->print_item(player->get_moves_string().c_str(), true);
  gui_->print_border();
}

void Board::add_player(bool is_cpu) {
  if (is_cpu) {
    players_.push_back(new CPU());
  } else {
    players_.push_back(new Player());
    num_humans_++;
  }
}

int Board::check_game_winner() {
  int toWin = board_size_ / num_players();
  unsigned int max_score = 0;
  unsigned int winner_id = 0;
  bool tie = false;

  for (const Player* player : players_) {
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
    return tie ? 0 : winner_id;
  }

  // Game is not over
  return -1;
}

int Board::check_round_winner() {
  int winner_id = 0;
  int biggest_move = 0;
  bool tie = false;

  for (int i = 0; i < num_players(); ++i) {
    int current_move = players_[i]->get_last_move();
    if (current_move > biggest_move) {
      biggest_move = current_move;
      winner_id = players_[i]->id();
      tie = false;
    } else if (current_move == biggest_move) {
      tie = true;
    }
  }

  if (tie) {
    return 0;
  }

  // Increment the score of the round winner
  players_[winner_id - 1]->inc_score();
  return winner_id;
}
