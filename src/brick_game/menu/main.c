#include "main.h"

/**
 * @mainpage Управление
 *
 * @section Меню
 *
 * "K_LEFT" - листать меню влево; \n
 * "K_RIGHT" - листать меню вправо; \n
 * "Enter" - запустить выбранную игру; \n
 * "Q" - вывести банер о подтверждении закрытии программы.
 *
 * @section Tetris
 *
 * "Enter" - начать игру \n
 * "K_LEFT" - сдвинуть тетрамино влево \n
 * "K_RIGHT" - сдвинуть тетрамино вправо \n
 * "K_DOWN" - ускорить падение тетрамино \n
 * "Space" - повернуть тетрамино по часовой стрелке \n
 * "P" - поставить игру на паузу \n
 * "Q" - закончить текущую игровую сессию / выйти в меню
 */

int main(void) {
  int error_code = OK;

  error_code = startMenu();

  return error_code;
}

int startMenu() {
  int error_code = OK;

  initNcurses();

  MenuPages_t menu = initMenu();
  UserAction_t menu_action = -2;
  char **console_field = initConsoleField();

  fillConsoleField(console_field);

  int end_flag = false;
  while (!error_code && !end_flag) {
    menu_action = userInput(menu_action);
    menuActionProcessing(menu_action, &end_flag, &menu, console_field);
    currentMenuPage(console_field, menu);
  }
  endwin();

  freeMenu(menu);
  freeConsoleField(console_field);

  return error_code;
}

void menuActionProcessing(UserAction_t menu_action, int *end_flag,
                          MenuPages_t *menu, char **console_field) {
  switch (menu_action) {
    case Start:
      startGame(menu, console_field);
      break;
    case Terminate:
      banner(end_flag, "close program");
      break;
    case Left:
      if (menu->page > Tetris) {
        menu->page -= 1;
      }
      break;
    case Right:
      if (menu->page < EnumCount - 1) {
        menu->page += 1;
      }
      break;
    default:
      break;
  }
}

MenuPages_t initMenu() {
  MenuPages_t menu = {};
  menu.names_pages = (char **)malloc(EnumCount * sizeof(char *));
  menu.names_pages[0] = strDup("Tetris");
  menu.names_pages[1] = strDup("Coming soon");

  menu.page = Tetris;

  return menu;
}

void freeMenu(MenuPages_t menu) {
  for (int i = 0; i < EnumCount; i++) {
    free(menu.names_pages[i]);
  }
  free(menu.names_pages);
}

void currentMenuPage(char **console_field, MenuPages_t menu) {
  if (menu.page >= Tetris && menu.page < EnumCount) {
    fillScreenMenuPage(console_field, menu);
    printConsoleField(console_field, menu.names_pages[menu.page]);
  }
}

void fillScreenMenuPage(char **console_field, MenuPages_t menu) {
  fillConsoleField(console_field);

  switch (menu.page) {
    case Tetris:
      for (int j = 6; j < 9; j++) {
        console_field[6][j] = FILLED_PIXEL;
      }
      console_field[7][8] = FILLED_PIXEL;

      for (int j = 4; j < 8; j++) {
        console_field[SCREEN_HEIGHT][j] = FILLED_PIXEL;
      }
      char next_word[] = "Next:";
      for (size_t j = 0; j < strlen(next_word); j++)
        console_field[11][13 + j] = next_word[j];
      break;

      // В дальнейшем пополнится

    default:
      break;
  }

  cleanHelpBar(console_field);
  updateHelpBar(console_field, 0, getHighScore(SCORE_FILE, menu.page + 1), 1);
}

void banner(int *flag, const char *action_name) {
  erase();
  printw("Do you want to %s?\n\n", action_name);
  printw("\"Enter\" - Yes        \"Q\" - No");
  refresh();

  int stoper = false;
  while (!stoper) {
    switch (getch()) {
      case '\n':
        *flag = true;
        stoper = true;
        break;
      case 'q':
      case 'Q':
        stoper = true;
        break;
      default:
        break;
    }
  }
}

void startGame(MenuPages_t *menu, char **console_field) {
  switch (menu->page) {
    case Tetris:
      startTetris(console_field, Tetris, menu->names_pages[menu->page]);
      break;

      // В будущем пополнится

    default:
      break;
  }
}
