#include "tetris_machine.h"

int startTetris(char **console_field, int game, char *name) {
  GameInfo_t game_info = {};
  TetraInfo_t tetra_info = {};
  UserAction_t action = -2;

  srand(time(NULL));
  clock_t start_timer = clock();

  TetrisGameState_t game_state = StateStart;
  while (game_state != StateClose) {
    switch (game_state) {
      case StateStart:
        game_state = tetrisStateStart(&game_info, &tetra_info, console_field,
                                      game, name);
        break;
      case StatePrint:
        game_state = tetrisStatePrint(&game_info, &tetra_info, console_field,
                                      &action, name);
        break;
      case StateAction:
        game_state =
            tetrisStateAction(action, &tetra_info.current, game_info.field,
                              console_field, &game_info.pause, &start_timer);
        break;
      case StateShifting:
        game_state = tetrisStateShifting(&start_timer, &game_info, &tetra_info,
                                         console_field);
        break;
      case StateCalculation:
        game_state =
            tetrisStateCalculation(&game_info, &tetra_info, console_field);
        break;
      case StatePause:
        game_state =
            tetrisStatePause(&game_info.pause, console_field, &start_timer);
        break;
      case StateGameOver:
        game_state = tetrisStateGameOver(&game_info, &tetra_info, game);
        break;
      default:
        break;
    }
  }

  return OK;
}

TetrisGameState_t tetrisStateStart(GameInfo_t *game_info,
                                   TetraInfo_t *tetra_info,
                                   char **console_field, int game, char *name) {
  TetrisGameState_t game_state = StateStart;

  cleanScreen(console_field);
  cleanHelpBar(console_field);
  updateHelpBar(console_field, 0, getHighScore(SCORE_FILE, game), 1);
  eraseNextTetramino(console_field);
  printConsoleField(console_field, name);

  switch (getch()) {
    case '\n':
      game_state = StatePrint;
      *game_info = initGameInfo(game);
      *tetra_info = initTetraInfo();
      srand(time(NULL));
      addNextTetraminoInScreen(tetra_info->next, console_field);
      break;
    case 'q':
    case 'Q':
      game_state = StateClose;
      break;
    default:
      break;
  }

  return game_state;
}

TetrisGameState_t tetrisStatePrint(GameInfo_t *game_info,
                                   TetraInfo_t *tetra_info,
                                   char **console_field, UserAction_t *action,
                                   char *name) {
  TetrisGameState_t game_state = StateShifting;

  printConsoleField(console_field, name);
  eraseTetramino(game_info->field, tetra_info->current);

  *action = userInput();
  if ((int)*action != -1) {
    game_state = StateAction;
  }

  return game_state;
}

TetrisGameState_t tetrisStateAction(UserAction_t action, Tetramino_t *tetramino,
                                    int **field, char **console_field,
                                    int *pause, clock_t *start_timer) {
  TetrisGameState_t game_state = StateShifting;

  switch (action) {
    case Pause:
      *pause = true;
      game_state = StatePause;
      printPauseMessage(console_field);
      printConsoleField(console_field, "Tetris");
      break;
    case Terminate:
      game_state = StateGameOver;
      break;
    case Left:
      if (leftMoveChecking(tetramino, field)) tetramino->move_x -= 1;
      break;
    case Right:
      if (rightMoveChecking(tetramino, field)) tetramino->move_x += 1;
      break;
    case Down:
      tetramino->move_y += 1;
      *start_timer = clock();
      break;
    case Action:
      rotateProcess(tetramino, field);
      break;
    default:
      break;
  }

  return game_state;
}

TetrisGameState_t tetrisStatePause(int *pause, char **console_field,
                                   clock_t *start_timer) {
  TetrisGameState_t game_state = StatePause;

  switch (getch()) {
    case 'q':
    case 'Q':
      game_state = StateGameOver;
      break;
    case 'p':
    case 'P':
      game_state = StatePrint;
      *pause = false;
      erasePauseMessage(console_field);
      *start_timer = clock();
      break;
    default:
      break;
  }

  return game_state;
}

TetrisGameState_t tetrisStateShifting(clock_t *start_timer,
                                      GameInfo_t *game_info,
                                      TetraInfo_t *tetra_info,
                                      char **console_field) {
  TetrisGameState_t game_state = StateCalculation;

  tetrisGameTick(start_timer, game_info->speed, &tetra_info->current);

  int fl = false;
  if (fallingAnalyse(game_info->field, tetra_info, &fl)) {
    game_state = StateGameOver;
  }
  
  if (fl) {
    eraseNextTetramino(console_field);
    addNextTetraminoInScreen(tetra_info->next, console_field);
  }

  return game_state;
}

TetrisGameState_t tetrisStateCalculation(GameInfo_t *game_info,
                                         TetraInfo_t *tetra_info,
                                         char **console_field) {
  TetrisGameState_t game_state = StatePrint;

  calculateNextScreen(game_info, *tetra_info);
  *game_info =
      updateCurrentState(*game_info, NEXT_LEVEL_POINTS, TETRIS_MAX_LEVEL);
  refreshConsoleField(console_field, *game_info);

  return game_state;
}

TetrisGameState_t tetrisStateGameOver(GameInfo_t *game_info,
                                      TetraInfo_t *tetra_info, int game) {
  TetrisGameState_t game_state = StateStart;

  updateHighScore(game_info->high_score, game + 1);

  freeTetraInfo(*tetra_info);
  freeGameInfo(*game_info);

  return game_state;
}

void eraseNextTetramino(char **console_field) {
  for (int j = 0; j < TETRAMINO_BLOCKS; j++) {
    console_field[12][13 + j] = EMPTY_PIXEL;
    console_field[13][13 + j] = EMPTY_PIXEL;
  }
}

void addNextTetraminoInScreen(Tetramino_t nextTetramino, char **console_field) {
  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    int x = nextTetramino.x[i] + nextTetramino.move_x;
    int y = nextTetramino.y[i] + nextTetramino.move_y;
    console_field[y + 14][x + 10] = FILLED_PIXEL;
  }
}