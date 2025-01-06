#include "interface.h"

void fillConsoleField(char **console_field) {
  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    for (int j = 0; j < CONSOLE_WIDTH; j++) {
      if (i == 0 || i == CONSOLE_HEIGHT - 1) {
        console_field[i][j] = '-';
      } else if (j == 0 || j == 11 || j == CONSOLE_WIDTH - 1) {
        console_field[i][j] = '|';
      } else {
        console_field[i][j] = ' ';
      }
    }
  }

  char high_word[] = "High";
  char score_word[] = "Score:";
  char level_word[] = "Level:";
  for (size_t j = 0; j < strlen(score_word); j++) {
    console_field[2][13 + j] = score_word[j];
    console_field[7][13 + j] = score_word[j];
  }
  for (size_t j = 0; j < strlen(high_word); j++)
    console_field[6][13 + j] = high_word[j];
  for (size_t j = 0; j < strlen(level_word); j++)
    console_field[16][13 + j] = level_word[j];
}

int refreshConsoleField(char **console_field, GameInfo_t game_info) {
  int error_code = OK;

  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      if (game_info.field[i][j] == 1) {
        console_field[i + 1][j + 1] = FILLED_PIXEL;
      } else if (game_info.field[i][j] == 0) {
        console_field[i + 1][j + 1] = EMPTY_PIXEL;
      } else {
        error_code = ERR;
      }
    }
  }

  cleanHelpBar(console_field);
  updateHelpBar(console_field, game_info.score, game_info.high_score,
                game_info.level);

  return error_code;
}

void printConsoleField(char **console_field, char *name_state) {
  erase();

  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    for (int j = 0; j < CONSOLE_WIDTH; j++) {
      printw("%c", console_field[i][j]);
    }
    printw("\n");
  }

  printw("%s\n", name_state);

  refresh();
}

UserAction_t userInput() {
  UserAction_t action = -1;

  switch (getch()) {
    case '\n':
      action = Start;
      break;

    case 'p':
    case 'P':
      action = Pause;
      break;

    case 'q':
    case 'Q':
      action = Terminate;
      break;

    case KEY_LEFT:
      action = Left;
      break;

    case KEY_RIGHT:
      action = Right;
      break;

    case KEY_UP:
      action = Up;
      break;

    case KEY_DOWN:
      action = Down;
      break;

    case ' ':
      action = Action;
      break;

    default:
      action = -2;
      break;
  }
  return action;
}

GameInfo_t updateCurrentState(GameInfo_t game_info, int next_level_points,
                              int max_level) {
  GameInfo_t new_game_info = {};

  freeScreenField(game_info.field);
  new_game_info.field = game_info.next;
  new_game_info.next = initScreenField();
  new_game_info.score = game_info.score;

  new_game_info.level = game_info.level;
  new_game_info.speed = game_info.speed;
  while (game_info.score / next_level_points > new_game_info.level - 1 &&
         new_game_info.level < max_level) {
    new_game_info.level += 1;
    new_game_info.speed = START_SPEED - (START_SPEED - END_SPEED) *
                                            (new_game_info.level - 1) /
                                            (max_level - 1);
  }

  if (game_info.score > game_info.high_score) {
    new_game_info.high_score = game_info.score;
  } else {
    new_game_info.high_score = game_info.high_score;
  }

  new_game_info.pause = game_info.pause;

  return new_game_info;
}

char **initConsoleField() {
  char **console_field = (char **)malloc(CONSOLE_HEIGHT * sizeof(char *));
  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    console_field[i] = (char *)malloc(CONSOLE_WIDTH * sizeof(char));
  }

  return console_field;
}

void freeConsoleField(char **console_field) {
  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    free(console_field[i]);
  }
  free(console_field);
}

void initNcurses() {
  initscr();
  keypad(stdscr, true);
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  cbreak();
}

char *strDup(const char *str) {
  char *d = malloc(strlen(str) + 1);
  if (d == NULL) return NULL;
  memcpy(d, str, strlen(str) + 1);
  return d;
}

int intLen(int num) {
  int counter = 0;

  if (num == 0) {
    counter = 1;
  } else {
    while (num > 0) {
      num /= 10;
      counter++;
    }
  }

  return counter;
}

int getDigit(int num, int digit) {
  int i;
  int div = 1;

  for (i = 0; i < digit - 1; i++) {
    div *= 10;
  }

  return (num / div) % 10;
}

void updateHelpBar(char **console_field, int score, int high_score, int level) {
  int score_digits = intLen(score);
  int high_score_digits = intLen(high_score);
  int level_digits = intLen(level);

  for (int j = 0; j < score_digits; j++) {
    console_field[3][j + 13] = 48 + getDigit(score, score_digits - j);
  }
  for (int j = 0; j < high_score_digits; j++) {
    console_field[8][j + 13] = 48 + getDigit(high_score, high_score_digits - j);
  }
  for (int j = 0; j < level_digits; j++) {
    console_field[17][j + 13] = 48 + getDigit(level, level_digits - j);
  }
}

void cleanHelpBar(char **console_field) {
  for (int j = 0; j < 6; j++) {
    console_field[3][j + 13] = EMPTY_PIXEL;
    console_field[8][j + 13] = EMPTY_PIXEL;
    console_field[17][j + 13] = EMPTY_PIXEL;
  }
}

void printPauseMessage(char **console_field) {
  char pause_word[] = "Pause";
  for (size_t j = 0; j < strlen(pause_word); j++)
    console_field[20][13 + j] = pause_word[j];
}

void erasePauseMessage(char **console_field) {
  char pause_word[] = "Pause";
  for (size_t j = 0; j < strlen(pause_word); j++)
    console_field[20][13 + j] = EMPTY_PIXEL;
}

void cleanScreen(char **console_field) {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      console_field[i + 1][j + 1] = EMPTY_PIXEL;
    }
  }
}
