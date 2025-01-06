#include "tetris_test.h"

START_TEST(testInitTetramino) {
  Tetramino_t tetramino = initTetramino();
  ck_assert_int_eq(tetramino.move_x, 0);
  ck_assert_int_eq(tetramino.move_y, -2);
  ck_assert_int_eq(tetramino.pos, 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testAddTetraminoInScreen) {
  int **screen_field = initScreenField();
  Tetramino_t tetramino = {};
  tetramino.pos = 0;
  tetramino.move_x = 2;
  tetramino.move_y = 3;
  tetramino.x = (int *)malloc(TETRAMINO_BLOCKS * sizeof(int));
  tetramino.y = (int *)malloc(TETRAMINO_BLOCKS * sizeof(int));
  tetramino.type = 1;
  rotateTetramino(&tetramino);
  addTetraminoInScreen(screen_field, tetramino);
  for (int j = 5; j < TETRAMINO_BLOCKS; j++) {
    ck_assert_int_eq(screen_field[3][j], 1);
  }
  freeTetramino(&tetramino);
  freeScreenField(screen_field);
}
END_TEST

START_TEST(testEraseTetramino) {
  int **screen_field = initScreenField();
  Tetramino_t tetramino = initTetramino();
  tetramino.move_x = 6;
  ck_assert_int_eq(eraseTetramino(screen_field, tetramino), 1);
  freeTetramino(&tetramino);
  freeScreenField(screen_field);
}
END_TEST

START_TEST(testFreeTetramino) {
  Tetramino_t tetramino = initTetramino();
  freeTetramino(&tetramino);
  ck_assert_ptr_null(tetramino.x);
  ck_assert_ptr_null(tetramino.y);
}
END_TEST

START_TEST(testNextTetramino) {
  TetraInfo_t tetra_info = initTetraInfo();
  int *next_x = tetra_info.next.x;
  int *next_y = tetra_info.next.y;
  nextTetramino(&tetra_info);
  ck_assert_ptr_eq(next_x, tetra_info.current.x);
  ck_assert_ptr_eq(next_y, tetra_info.current.y);
  freeTetraInfo(tetra_info);
}
END_TEST

START_TEST(testEraseNextTetramino) {
  Tetramino_t tetramino = initTetramino();
  tetramino.move_y = 0;
  char **console_field = (char **)malloc(CONSOLE_HEIGHT * sizeof(char *));
  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    console_field[i] = (char *)malloc(CONSOLE_WIDTH * sizeof(char));
  }
  addNextTetraminoInScreen(tetramino, console_field);
  eraseNextTetramino(console_field);
  for (int j = 0; j < TETRAMINO_BLOCKS; j++) {
    ck_assert_int_eq(console_field[12][13 + j], EMPTY_PIXEL);
    ck_assert_int_eq(console_field[13][13 + j], EMPTY_PIXEL);
  }

  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    free(console_field[i]);
  }
  free(console_field);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testLeftMoveChecking1) {
  Tetramino_t tetramino = initTetramino();
  tetramino.move_x -= 3;
  tetramino.move_y = 0;
  int **screen_field = initScreenField();
  ck_assert_int_eq(leftMoveChecking(&tetramino, screen_field), 0);
  freeScreenField(screen_field);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testLeftMoveChecking2) {
  Tetramino_t tetramino = initTetramino();
  tetramino.move_x -= 3;
  int **screen_field = initScreenField();
  ck_assert_int_eq(leftMoveChecking(&tetramino, screen_field), 0);
  freeScreenField(screen_field);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRightMoveChecking1) {
  Tetramino_t tetramino = initTetramino();
  tetramino.move_x += 6;
  tetramino.move_y = 0;
  int **screen_field = initScreenField();
  ck_assert_int_eq(rightMoveChecking(&tetramino, screen_field), 0);
  freeScreenField(screen_field);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRightMoveChecking2) {
  Tetramino_t tetramino = initTetramino();
  tetramino.move_x += 6;
  int **screen_field = initScreenField();
  ck_assert_int_eq(rightMoveChecking(&tetramino, screen_field), 0);
  freeScreenField(screen_field);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateProcess1) {
  int **screen_field = initScreenField();
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 1;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  rotateProcess(&tetramino, screen_field);
  Tetramino_t new_pos_tetramino = initTetramino();
  new_pos_tetramino.type = 1;
  new_pos_tetramino.pos = 1;
  rotateTetramino(&new_pos_tetramino);
  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    ck_assert_int_eq(tetramino.x[i], new_pos_tetramino.x[i]);
    ck_assert_int_eq(tetramino.y[i], new_pos_tetramino.y[i]);
  }
  freeScreenField(screen_field);
  freeTetramino(&tetramino);
  freeTetramino(&new_pos_tetramino);
}
END_TEST

START_TEST(testRotateProcess2) {
  int **screen_field = initScreenField();
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 1;
  tetramino.pos = 0;
  tetramino.move_y = SCREEN_HEIGHT;
  rotateTetramino(&tetramino);
  rotateProcess(&tetramino, screen_field);
  Tetramino_t new_pos_tetramino = initTetramino();
  new_pos_tetramino.type = 1;
  new_pos_tetramino.pos = 0;
  rotateTetramino(&new_pos_tetramino);
  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    ck_assert_int_eq(tetramino.x[i], new_pos_tetramino.x[i]);
    ck_assert_int_eq(tetramino.y[i], new_pos_tetramino.y[i]);
  }
  freeScreenField(screen_field);
  freeTetramino(&tetramino);
  freeTetramino(&new_pos_tetramino);
}
END_TEST

START_TEST(testRotateTetramino1) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 2;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 3);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 5);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino2) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 2;
  tetramino.pos = 1;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 3);
  ck_assert_int_eq(tetramino.x[3], 3);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 2);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino3) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 2;
  tetramino.pos = 2;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 5);
  ck_assert_int_eq(tetramino.x[3], 5);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 0);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino4) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 2;
  tetramino.pos = 3;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 4);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 3);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 2);
  ck_assert_int_eq(tetramino.y[3], 2);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino5) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 3;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 5);
  ck_assert_int_eq(tetramino.x[3], 5);
  ck_assert_int_eq(tetramino.y[0], 1);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 0);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino6) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 3;
  tetramino.pos = 1;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 3);
  ck_assert_int_eq(tetramino.x[2], 3);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 2);
  ck_assert_int_eq(tetramino.y[3], 2);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino7) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 3;
  tetramino.pos = 2;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 5);
  ck_assert_int_eq(tetramino.x[3], 3);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 0);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino8) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 3;
  tetramino.pos = 3;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 2);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino9) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 4;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 3);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino10) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 5;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 4);
  ck_assert_int_eq(tetramino.x[1], 5);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 3);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino11) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 5;
  tetramino.pos = 1;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 3);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 2);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino12) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 6;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 5);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 1);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 0);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino13) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 6;
  tetramino.pos = 1;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 3);
  ck_assert_int_eq(tetramino.x[2], 3);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 2);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino14) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 6;
  tetramino.pos = 2;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 5);
  ck_assert_int_eq(tetramino.x[3], 4);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 0);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino15) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 6;
  tetramino.pos = 3;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 4);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 3);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 2);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino16) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 7;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 3);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 4);
  ck_assert_int_eq(tetramino.x[3], 5);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 0);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 1);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testRotateTetramino17) {
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 7;
  tetramino.pos = 1;
  rotateTetramino(&tetramino);
  ck_assert_int_eq(tetramino.x[0], 4);
  ck_assert_int_eq(tetramino.x[1], 4);
  ck_assert_int_eq(tetramino.x[2], 3);
  ck_assert_int_eq(tetramino.x[3], 3);
  ck_assert_int_eq(tetramino.y[0], 0);
  ck_assert_int_eq(tetramino.y[1], 1);
  ck_assert_int_eq(tetramino.y[2], 1);
  ck_assert_int_eq(tetramino.y[3], 2);
  freeTetramino(&tetramino);
}
END_TEST

START_TEST(testCalculateNextScreen) {
  TetraInfo_t tetra_info = initTetraInfo();
  tetra_info.current.move_y = 0;
  GameInfo_t game_info = initGameInfo(0);
  calculateNextScreen(&game_info, tetra_info);
  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    ck_assert_int_eq(
        game_info.next[tetra_info.current.move_y + tetra_info.current.y[i]]
                      [tetra_info.current.move_x + tetra_info.current.x[i]],
        1);
  }
  freeTetraInfo(tetra_info);
  freeGameInfo(game_info);
}
END_TEST

START_TEST(testGameTick) {
  int speed = 2000000;
  clock_t start_timer = clock() - speed;
  Tetramino_t tetramino = initTetramino();
  tetramino.type = 1;
  tetramino.pos = 0;
  rotateTetramino(&tetramino);

  Tetramino_t copy_tetramino = initTetramino();
  copy_tetramino.type = 1;
  copy_tetramino.pos = 0;
  rotateTetramino(&tetramino);

  tetrisGameTick(&start_timer, speed, &tetramino);

  for (int i = 0; i < TETRAMINO_BLOCKS; i++) {
    ck_assert_int_eq(tetramino.move_y, copy_tetramino.move_y + 1);
  }

  freeTetramino(&tetramino);
  freeTetramino(&copy_tetramino);
}
END_TEST

START_TEST(testFallingAnalyse) {
  int **screen_field = initScreenField();

  TetraInfo_t tetra_info = initTetraInfo();
  tetra_info.current.type = 1;
  tetra_info.current.pos = 0;
  tetra_info.current.move_y = SCREEN_HEIGHT;
  rotateTetramino(&tetra_info.current);

  char **console_field = (char **)malloc(CONSOLE_HEIGHT * sizeof(char *));
  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    console_field[i] = (char *)malloc(CONSOLE_WIDTH * sizeof(char));
  }

  fallingAnalyse(screen_field, &tetra_info, console_field);

  int result = fallingAnalyse(screen_field, &tetra_info, console_field);

  ck_assert_int_eq(result, 0);

  freeScreenField(screen_field);
  freeTetraInfo(tetra_info);
  for (int i = 0; i < CONSOLE_HEIGHT; i++) {
    free(console_field[i]);
  }
  free(console_field);
}
END_TEST

START_TEST(testCalculateScore1) {
  int score = 0;
  int counter = 1;
  calculateScore(&score, counter);
  ck_assert_int_eq(score, 100);
}
END_TEST

START_TEST(testCalculateScore2) {
  int score = 0;
  int counter = 2;
  calculateScore(&score, counter);
  ck_assert_int_eq(score, 300);
}
END_TEST

START_TEST(testCalculateScore3) {
  int score = 0;
  int counter = 3;
  calculateScore(&score, counter);
  ck_assert_int_eq(score, 700);
}
END_TEST

START_TEST(testCalculateScore4) {
  int score = 0;
  int counter = 4;
  calculateScore(&score, counter);
  ck_assert_int_eq(score, 1500);
}
END_TEST

Suite *testingTetrisLibrary(void) {
  Suite *suite = suite_create("testingTetrisLibrary");
  TCase *tc_core = tcase_create("core_of_create_matrix");
  tcase_add_test(tc_core, testInitTetramino);
  tcase_add_test(tc_core, testAddTetraminoInScreen);
  tcase_add_test(tc_core, testEraseTetramino);
  tcase_add_test(tc_core, testFreeTetramino);
  tcase_add_test(tc_core, testNextTetramino);
  tcase_add_test(tc_core, testEraseNextTetramino);
  tcase_add_test(tc_core, testLeftMoveChecking1);
  tcase_add_test(tc_core, testLeftMoveChecking2);
  tcase_add_test(tc_core, testRightMoveChecking1);
  tcase_add_test(tc_core, testRightMoveChecking2);
  tcase_add_test(tc_core, testRotateProcess1);
  tcase_add_test(tc_core, testRotateProcess2);
  tcase_add_test(tc_core, testRotateTetramino1);
  tcase_add_test(tc_core, testRotateTetramino2);
  tcase_add_test(tc_core, testRotateTetramino3);
  tcase_add_test(tc_core, testRotateTetramino4);
  tcase_add_test(tc_core, testRotateTetramino5);
  tcase_add_test(tc_core, testRotateTetramino6);
  tcase_add_test(tc_core, testRotateTetramino7);
  tcase_add_test(tc_core, testRotateTetramino8);
  tcase_add_test(tc_core, testRotateTetramino9);
  tcase_add_test(tc_core, testRotateTetramino10);
  tcase_add_test(tc_core, testRotateTetramino11);
  tcase_add_test(tc_core, testRotateTetramino12);
  tcase_add_test(tc_core, testRotateTetramino13);
  tcase_add_test(tc_core, testRotateTetramino14);
  tcase_add_test(tc_core, testRotateTetramino15);
  tcase_add_test(tc_core, testRotateTetramino16);
  tcase_add_test(tc_core, testRotateTetramino17);
  tcase_add_test(tc_core, testCalculateNextScreen);
  tcase_add_test(tc_core, testGameTick);
  tcase_add_test(tc_core, testFallingAnalyse);
  tcase_add_test(tc_core, testCalculateScore1);
  tcase_add_test(tc_core, testCalculateScore2);
  tcase_add_test(tc_core, testCalculateScore3);
  tcase_add_test(tc_core, testCalculateScore4);
  suite_add_tcase(suite, tc_core);

  return suite;
}

void suitExecution(Suite *suite, int *failed_count, char *suite_name) {
  SRunner *suite_runner = srunner_create(suite);
  srunner_set_log(suite_runner, suite_name);
  srunner_run_all(suite_runner, CK_NORMAL);
  *failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
}

int main(void) {
  int failed_count = 0;

  suitExecution(testingTetrisLibrary(), &failed_count,
                "tests/testingTetrisLibrary.log");

  return failed_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}