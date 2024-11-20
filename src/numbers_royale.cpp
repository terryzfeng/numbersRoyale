#include "numbers_royale.h"
#include "cpu.h"


// Helper functions
/**
 * @brief Wait for user to press enter
 * 
 * @param message Message to display
 */
void wait_for_enter(const char* message = "Press <enter> to continue... ") {
  printf("%s", message);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Get the validated input object
 * 
 * @param is_valid lambda function to validate input
 * @return int selected input after validation
 */
int get_validated_input(const std::function<bool(int)>& is_valid) {
  int input;
  while (true) {
    std::cin >> input;
    if (std::cin.fail() || !is_valid(input)) {
      std::cin.clear(); // clear the error flag
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
      std::cout << "Invalid input. Please try again: ";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard any extra input
      return input;
    }
  }
}


// Numbers Royale Class
NumbersRoyale::NumbersRoyale(GUI *gui): gui_(gui), board_(gui) {
  gui_->clear_screen();
}

void NumbersRoyale::main_menu() {
  while (true) {
    gui_->print_header("NUMBERS ROYALE");
    gui_->print_item("1. Play Game");
    gui_->print_item("2. How to Play");
    gui_->print_item("3. Quit");
    gui_->print_border();
    printf("Select option: ");
    int option = get_validated_input([](int input) { 
      return input >= MainMenuOption::PLAY_GAME && input <= MainMenuOption::QUIT; 
    });

    switch (option) {
      case MainMenuOption::PLAY_GAME:
        play_mode();
        break;
      case MainMenuOption::HOW_TO_PLAY:
        how_to_play();
        break;
      case MainMenuOption::QUIT:
        return;
      default:
        std::cerr << "Error: Invalid option" << std::endl;
        break;
    }
  }
}

void NumbersRoyale::how_to_play() {
  // TODO: Implement line wrapping for gui.h
  gui_->clear_screen();
  printf("|=================================|\n");
  printf("|           HOW TO PLAY           |\n");
  printf("|=================================|\n");
  printf("| 1. The game is played by two    |\n");
  printf("|    players.                     |\n");
  printf("| 2. Each player will take turns  |\n");
  printf("|    selecting a number from      |\n");
  printf("|    their available numbers.     |\n");
  printf("| 3. The player who selects the   |\n");
  printf("|    larger number wins the round.|\n");
  printf("| 4. After using a number, it     |\n");
  printf("|    cannot be used again.        |\n");
  printf("| 5. The game ends when all the   |\n");
  printf("|    numbers have been selected.  |\n");
  printf("| 6. The player with the most     |\n");
  printf("|    points wins the game.        |\n");
  printf("|=================================|\n");
  wait_for_enter("Press <enter> to return to the main menu... ");
}

void NumbersRoyale::play_mode() {
  gui_->print_header("SELECT MODE");
  gui_->print_item("1. Player vs Player");
  gui_->print_item("2. Player vs CPU");
  gui_->print_item("3. Return to Main Menu");
  gui_->print_border();
  printf("Select option: ");
  int option = get_validated_input([](int input) { 
    return input >= PlayModeOption::PLAYER_VS_PLAYER && 
        input <= PlayModeOption::RETURN_TO_MAIN_MENU;
  });

  // P1
  board_.add_player(false);

  switch (option) {
    case PlayModeOption::PLAYER_VS_PLAYER:
      board_.add_player(false);
      break;
    case PlayModeOption::PLAYER_VS_CPU:
      board_.add_player(true);
      break;
    case PlayModeOption::RETURN_TO_MAIN_MENU:
      return;
    default:
      std::cerr << "Error: Invalid option" << std::endl;
      break;
  }

  select_board_size();
}

void NumbersRoyale::select_board_size() {
  gui_->print_header("SELECT BOARD SIZE");
  gui_->print_item("1. 5");
  gui_->print_item("2. 7");
  gui_->print_item("3. 9");
  gui_->print_item("4. Return to Main Menu");
  gui_->print_border();
  printf("Select option: ");
  int option = get_validated_input([](int input) { 
    return input >= BoardSizeOption::SMALL && 
        input <= BoardSizeOption::RETURN_TO_MAIN_MENU_2;
  });

  if (option != BoardSizeOption::RETURN_TO_MAIN_MENU_2) {
    board_.init(board_option_to_size(option));
    play_game();
  } else {
    board_.reset();
  }
}

void NumbersRoyale::play_game() {
  while (board_.check_game_winner() == -1) {
    for (Player* player : board_.players()) {
      if (player->is_cpu()) {
        // CPU
        unsigned int cpu_move = player->move(0);
        if (!cpu_move) {
          std::cerr << "Error: CPU move failed" << std::endl;
        }
      } else {
        // Player
        if (board_.num_humans() > 1) {
          gui_->print_header("NUMBERS ROYALE");
          printf("Pass the device to %s\n", player->name().c_str());
          wait_for_enter();
        }

#ifdef DEBUG
        board_.print_board();
#else
        board_.print_player_moves(player->id());
#endif
        printf("%s, select a number: ", player->name().c_str());

        int player_move = get_validated_input([player](int input) { 
          return player->is_valid_move(input); 
        });
        player->move(player_move);
      }
    }

    // Round results
    board_.print_round_result();
    wait_for_enter();
  }

  // End game
  board_.reset();
}
