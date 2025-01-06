#include "tetris.h"

int eraseTetramino(int **screen_field, Tetramino_t tetramino) {
  int error = 0;

  int stoper = false;
  for (int i = 0; i < TETRAMINO_BLOCKS && !stoper; i++) {
    int x = tetramino.x[i] + tetramino.move_x;
    int y = tetramino.y[i] + tetramino.move_y;
    if (x > SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT) {
      stoper = true;
      error = 1;
    } else if (y >= 0) {
      screen_field[y][x] = 0;
    }
  }

  return error;
}

void tetrisGameTick(clock_t *start_timer, int speed, Tetramino_t *tetramino) {
  clock_t end_timer = clock();

  if (((int)(end_timer - *start_timer)) >= speed) {
    int fl = true;
    for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
      int y = tetramino->y[i] + tetramino->move_y;
      if (y + 1 > SCREEN_HEIGHT) fl = false;
    }
    if (fl) tetramino->move_y += 1;

    *start_timer = clock();
  }
}

int fallingAnalyse(int **screen_field, TetraInfo_t *tetra_info, int *fl) {
  int end_condition = false;

  Tetramino_t *cur_tetramino = &tetra_info->current;

  int stoper = false;
  for (int i = 0; i < TETRAMINO_BLOCKS && !stoper; i++) {
    int x = cur_tetramino->x[i] + cur_tetramino->move_x;
    int y = cur_tetramino->y[i] + cur_tetramino->move_y;

    if (y == SCREEN_HEIGHT || (y >= 0 && screen_field[y][x] == 1)) {
      *fl = true;
      stoper = true;
    }
  }

  int count = 0;
  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    int y = cur_tetramino->y[i] + cur_tetramino->move_y;
    if (y - 1 < 0 && *fl == true) count++;
  }

  if (count == TETRAMINO_BLOCKS) end_condition = true;

  if (*fl) {
    cur_tetramino->move_y -= 1;
    addTetraminoInScreen(screen_field, tetra_info->current);
    nextTetramino(tetra_info);
  }

  return end_condition;
}

void calculateNextScreen(GameInfo_t *game_info, TetraInfo_t tetra_info) {
  int counter = clearFullStrings(game_info->field, game_info->next);
  calculateScore(&game_info->score, counter);
  addTetraminoInScreen(game_info->next, tetra_info.current);
}

void calculateScore(int *score, int counter) {
  switch (counter) {
    case 1:
      *score += 100;
      break;
    case 2:
      *score += 300;
      break;
    case 3:
      *score += 700;
      break;
    case 4:
      *score += 1500;
      break;
    default:
      break;
  }
}

void nextTetramino(TetraInfo_t *tetra_info) {
  freeTetramino(&tetra_info->current);
  tetra_info->current = tetra_info->next;
  tetra_info->next = initTetramino();
}

int clearFullStrings(int **field, int **next) {
  int counter = 0;

  int next_i = SCREEN_HEIGHT - 1;

  for (int i = SCREEN_HEIGHT - 1; i >= 0; i--) {
    int fl = true;

    int stoper = false;
    for (int j = 0; j < SCREEN_WIDTH && !stoper; j++) {
      if (field[i][j] != 1) {
        fl = false;
        stoper = true;
      }
    }

    if (!fl) {
      memcpy(next[next_i], field[i], SCREEN_WIDTH * sizeof(int));
      next_i--;
    } else {
      counter++;
    }
  }

  while (next_i >= 0) {
    memset(next[next_i], 0, SCREEN_WIDTH * sizeof(int));
    next_i--;
  }

  return counter;
}

void addTetraminoInScreen(int **screen_field, Tetramino_t tetramino) {
  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    int x = tetramino.x[i] + tetramino.move_x;
    int y = tetramino.y[i] + tetramino.move_y;
    if (y >= 0) screen_field[y][x] = 1;
  }
}

void rotateProcess(Tetramino_t *tetramino, int **field) {
  rotateTetramino(tetramino);

  int fl = true;

  int stoper = false;
  for (int i = 0; i < TETRAMINO_BLOCKS && !stoper; i++) {
    int x = tetramino->x[i] + tetramino->move_x;
    int y = tetramino->y[i] + tetramino->move_y;
    if (y >= SCREEN_HEIGHT || (y >= 0 && field[y][x] == 1)) {
      stoper = true;
      fl = false;
    }
  }

  if (fl) {
    for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
      int x = tetramino->x[i] + tetramino->move_x;
      while (x < 0) {
        tetramino->move_x += 1;
        x++;
      }
      while (x >= SCREEN_WIDTH) {
        tetramino->move_x -= 1;
        x--;
      }
    }
  } else {
    for (int j = 0; j < 3; j++) rotateTetramino(tetramino);
  }
}

int leftMoveChecking(Tetramino_t *tetramino, int **screen_field) {
  int result = true;

  int stoper = false;
  for (int i = 0; i < TETRAMINO_BLOCKS && !stoper; i++) {
    int x = tetramino->x[i] + tetramino->move_x;
    int y = tetramino->y[i] + tetramino->move_y;
    if (y >= 0) {
      if (x - 1 < 0 || screen_field[y][x - 1] == 1) {
        result = false;
        stoper = true;
      }
    } else {
      if (x - 1 < 0) {
        result = false;
        stoper = true;
      }
    }
  }

  return result;
}

int rightMoveChecking(Tetramino_t *tetramino, int **screen_field) {
  int result = true;

  int stoper = false;
  for (int i = 0; i < TETRAMINO_BLOCKS && !stoper; i++) {
    int x = tetramino->x[i] + tetramino->move_x;
    int y = tetramino->y[i] + tetramino->move_y;
    if (y >= 0) {
      if (x + 1 >= SCREEN_WIDTH || screen_field[y][x + 1] == 1) {
        result = false;
        stoper = true;
      }
    } else {
      if (x + 1 >= SCREEN_WIDTH) {
        result = false;
        stoper = true;
      }
    }
  }

  return result;
}

TetraInfo_t initTetraInfo() {
  TetraInfo_t tetra_info = {};

  tetra_info.current = initTetramino();
  tetra_info.next = initTetramino();

  return tetra_info;
}

Tetramino_t initTetramino() {
  Tetramino_t tetramino = {};

  tetramino.pos = 0;
  tetramino.move_x = 0;
  tetramino.move_y = -2;
  tetramino.x = (int *)malloc(TETRAMINO_BLOCKS * sizeof(int));
  tetramino.y = (int *)malloc(TETRAMINO_BLOCKS * sizeof(int));
  tetramino.type = randNum(1, 7);

  rotateTetramino(&tetramino);

  return tetramino;
}

void freeTetramino(Tetramino_t *tetramino) {
  free(tetramino->x);
  free(tetramino->y);
  tetramino->x = NULL;
  tetramino->y = NULL;
}

void freeTetraInfo(TetraInfo_t tetra_info) {
  freeTetramino(&tetra_info.current);
  freeTetramino(&tetra_info.next);
}

void rotateTetramino(Tetramino_t *tetramino) {
  switch (tetramino->type) {
    case 1:
      changePosTetraminoType1(tetramino);
      break;
    case 2:
      changePosTetraminoType2(tetramino);
      break;
    case 3:
      changePosTetraminoType3(tetramino);
      break;
    case 4:
      changePosTetraminoType4(tetramino);
      break;
    case 5:
      changePosTetraminoType5(tetramino);
      break;
    case 6:
      changePosTetraminoType6(tetramino);
      break;
    case 7:
      changePosTetraminoType7(tetramino);
      break;
    default:
      break;
  }
}

void changePosTetraminoType1(Tetramino_t *tetramino) {
  // Тип 1 -> ####

  if (tetramino->pos >= 2) tetramino->pos = 0;
  tetramino->pos += 1;

  switch (tetramino->pos) {
    case 1:
    case 3:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 5;
      tetramino->x[3] = 6;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 0;
      tetramino->y[3] = 0;
      break;
    case 2:
    case 4:
      tetramino->x[0] = 3;
      tetramino->x[1] = 3;
      tetramino->x[2] = 3;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 2;
      tetramino->y[3] = 3;
      break;
    default:
      break;
  }
}

void changePosTetraminoType2(Tetramino_t *tetramino) {
  // Тип 2 -> #
  //          ###

  if (tetramino->pos >= 4) tetramino->pos = 0;
  tetramino->pos += 1;

  switch (tetramino->pos) {
    case 1:
      tetramino->x[0] = 3;
      tetramino->x[1] = 3;
      tetramino->x[2] = 4;
      tetramino->x[3] = 5;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 1;
      tetramino->y[3] = 1;
      break;
    case 2:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 3;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 1;
      tetramino->y[3] = 2;
      break;
    case 3:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 5;
      tetramino->x[3] = 5;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 0;
      tetramino->y[3] = 1;
      break;
    case 4:
      tetramino->x[0] = 4;
      tetramino->x[1] = 4;
      tetramino->x[2] = 4;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 2;
      tetramino->y[3] = 2;
      break;
    default:
      break;
  }
}

void changePosTetraminoType3(Tetramino_t *tetramino) {
  // Тип 3 ->   #
  //          ###

  if (tetramino->pos >= 4) tetramino->pos = 0;
  tetramino->pos += 1;

  switch (tetramino->pos) {
    case 1:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 5;
      tetramino->x[3] = 5;

      tetramino->y[0] = 1;
      tetramino->y[1] = 1;
      tetramino->y[2] = 1;
      tetramino->y[3] = 0;
      break;
    case 2:
      tetramino->x[0] = 3;
      tetramino->x[1] = 3;
      tetramino->x[2] = 3;
      tetramino->x[3] = 4;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 2;
      tetramino->y[3] = 2;
      break;
    case 3:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 5;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 0;
      tetramino->y[3] = 1;
      break;
    case 4:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 4;
      tetramino->x[3] = 4;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 1;
      tetramino->y[3] = 2;
      break;
    default:
      break;
  }
}

void changePosTetraminoType4(Tetramino_t *tetramino) {
  // Тип 4 -> ##
  //          ##

  tetramino->x[0] = 3;
  tetramino->x[1] = 4;
  tetramino->x[2] = 3;
  tetramino->x[3] = 4;

  tetramino->y[0] = 0;
  tetramino->y[1] = 0;
  tetramino->y[2] = 1;
  tetramino->y[3] = 1;
}

void changePosTetraminoType5(Tetramino_t *tetramino) {
  // Тип 5 ->  ##
  //          ##

  if (tetramino->pos >= 2) tetramino->pos = 0;
  tetramino->pos += 1;

  switch (tetramino->pos) {
    case 1:
    case 3:
      tetramino->x[0] = 4;
      tetramino->x[1] = 5;
      tetramino->x[2] = 4;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 1;
      tetramino->y[3] = 1;
      break;
    case 2:
    case 4:
      tetramino->x[0] = 3;
      tetramino->x[1] = 3;
      tetramino->x[2] = 4;
      tetramino->x[3] = 4;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 1;
      tetramino->y[3] = 2;
      break;
    default:
      break;
  }
}

void changePosTetraminoType6(Tetramino_t *tetramino) {
  // Тип 6 ->  #
  //          ###

  if (tetramino->pos >= 4) tetramino->pos = 0;
  tetramino->pos += 1;

  switch (tetramino->pos) {
    case 1:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 5;
      tetramino->x[3] = 4;

      tetramino->y[0] = 1;
      tetramino->y[1] = 1;
      tetramino->y[2] = 1;
      tetramino->y[3] = 0;
      break;
    case 2:
      tetramino->x[0] = 3;
      tetramino->x[1] = 3;
      tetramino->x[2] = 3;
      tetramino->x[3] = 4;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 2;
      tetramino->y[3] = 1;
      break;
    case 3:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 5;
      tetramino->x[3] = 4;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 0;
      tetramino->y[3] = 1;
      break;
    case 4:
      tetramino->x[0] = 4;
      tetramino->x[1] = 4;
      tetramino->x[2] = 4;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 2;
      tetramino->y[3] = 1;
      break;
    default:
      break;
  }
}

void changePosTetraminoType7(Tetramino_t *tetramino) {
  // Тип 7 -> ##
  //           ##

  if (tetramino->pos >= 2) tetramino->pos = 0;
  tetramino->pos += 1;

  switch (tetramino->pos) {
    case 1:
    case 3:
      tetramino->x[0] = 3;
      tetramino->x[1] = 4;
      tetramino->x[2] = 4;
      tetramino->x[3] = 5;

      tetramino->y[0] = 0;
      tetramino->y[1] = 0;
      tetramino->y[2] = 1;
      tetramino->y[3] = 1;
      break;
    case 2:
    case 4:
      tetramino->x[0] = 4;
      tetramino->x[1] = 4;
      tetramino->x[2] = 3;
      tetramino->x[3] = 3;

      tetramino->y[0] = 0;
      tetramino->y[1] = 1;
      tetramino->y[2] = 1;
      tetramino->y[3] = 2;
      break;
    default:
      break;
  }
}
