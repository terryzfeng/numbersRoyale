#include "numbers_royale.h"
#include "cpu.h"


namespace {
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
   * @brief Template function to get user input with input validation
   * 
   * @param is_valid lambda function to validate user input
   * @return selected user input option of type T after validation
   */
  template <typename T>
  T get_validated_input(const std::function<bool(T)>& is_valid) {
    int input;
    while (true) {
      std::cin >> input;
      T option = static_cast<T>(input);
      if (std::cin.fail() || !is_valid(option)) {
        std::cin.clear(); // clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Invalid input. Please try again: ";
      } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard any extra input
        return option;
      }
    }
  }
};


// Numbers Royale Class
NumbersRoyale::NumbersRoyale(): board_() {
  GUI::clear_screen();
}

void NumbersRoyale::main_menu() {
  while (true) {
    GUI::print_header("NUMBERS ROYALE");
    GUI::print_item("1. Play Game");
    GUI::print_item("2. How to Play");
    GUI::print_item("3. Quit");
    GUI::print_border();
    printf("Select option: ");

    const std::function<bool(MainMenuOption)> valid_menu_option = 
        [](MainMenuOption option) -> bool {
          return option >= MainMenuOption::PLAY_GAME && option <= MainMenuOption::QUIT; 
        };
    MainMenuOption option = get_validated_input(valid_menu_option);

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
  GUI::clear_screen();
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
  GUI::print_header("SELECT MODE");
  GUI::print_item("1. Player vs Player");
  GUI::print_item("2. Player vs CPU");
  GUI::print_item("3. Return to Main Menu");
  GUI::print_border();
  printf("Select option: ");

  const std::function<bool(PlayModeOption)> valid_play_option = 
      [](PlayModeOption option) -> bool {
        return option >= PlayModeOption::PLAYER_VS_PLAYER && 
            option <= PlayModeOption::RETURN_TO_MAIN_MENU;
      };
  PlayModeOption option = get_validated_input(valid_play_option);

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
  GUI::print_header("SELECT BOARD SIZE");
  GUI::print_item("1. 5");
  GUI::print_item("2. 7");
  GUI::print_item("3. 9");
  GUI::print_item("4. Return to Main Menu");
  GUI::print_border();
  printf("Select option: ");

  const std::function<bool(BoardSizeOption)> valid_board_option = 
      [](BoardSizeOption option) -> bool {
        return option >= BoardSizeOption::SMALL && 
            option <= BoardSizeOption::RETURN_TO_MAIN_MENU;
      };
  BoardSizeOption option = get_validated_input(valid_board_option);

  if (option != BoardSizeOption::RETURN_TO_MAIN_MENU) {
    board_.init(board_option_to_size(static_cast<unsigned int>(option)));
    play_game();
  } else {
    board_.reset();
  }
}

void NumbersRoyale::play_game() {
  while (board_.check_game_winner() == -1) {
    // iterate through players
    for (const std::unique_ptr<Player>& player : board_.players()) {
      if (player->is_cpu()) {
        // CPU
        unsigned int cpu_move = player->move(0);
        if (!cpu_move) {
          std::cerr << "Error: CPU move failed" << std::endl;
        }
      } else {
        // Player
        if (board_.get_num_humans() > 1) {
          GUI::print_header("NUMBERS ROYALE");
          printf("Pass the device to %s\n", player->name().c_str());
          wait_for_enter();
        }

#ifdef DEBUG
        board_.print_board();
#else
        board_.print_player_moves(player->id());
#endif
        printf("%s, select a number: ", player->name().c_str());

        const std::function<bool(int)> valid_move_option = 
            [&player](int option) -> bool {
              return player->is_valid_move(option);
            };
        int player_move = get_validated_input(valid_move_option);
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
