#include "essential.h"

int randNum(int min, int max) { return min + rand() % (max - min + 1); }

int **initScreenField() {
  int **screen_field = (int **)malloc(SCREEN_HEIGHT * sizeof(int *));
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    screen_field[i] = (int *)calloc(SCREEN_WIDTH, sizeof(int));
  }

  return screen_field;
}

void freeScreenField(int **screen_field) {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    free(screen_field[i]);
  }
  free(screen_field);
}

GameInfo_t initGameInfo(int game) {
  GameInfo_t new_game_info = {};
  new_game_info.field = initScreenField();
  new_game_info.next = initScreenField();
  new_game_info.score = 0;
  new_game_info.high_score = getHighScore(SCORE_FILE, game);
  new_game_info.level = 1;
  new_game_info.speed = START_SPEED;
  new_game_info.pause = 0;

  return new_game_info;
}

void freeGameInfo(GameInfo_t game_info) {
  freeScreenField(game_info.field);
  freeScreenField(game_info.next);
}

int getHighScore(char *filename, int line_number) {
  int value;

  int stoper = false;

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    value = 0;
    stoper = true;
  }

  for (int i = 0; i < line_number - 1 && !stoper; i++) {
    if (fscanf(file, "%d", &value) != 1) {
      stoper = true;
      value = 0;
    }
  }
  if (!stoper) {
    fscanf(file, "%d", &value);
    fclose(file);
  }
  return value;
}

int updateHighScore(int new_number, int line_number) {
  int error_code = 0;

  FILE *file = fopen(SCORE_FILE, "r");
  if (!file) {
    error_code = 1;
  } else {
    FILE *temp_file = fopen("temp.txt", "w+");
    if (!temp_file) {
      fclose(file);
      error_code = 1;
    } else {
      char buffer[1024];
      int current_line = 1;

      while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line == line_number) {
          fprintf(temp_file, "%d\n", new_number);
        } else {
          fputs(buffer, temp_file);
        }
        current_line++;
      }
      fclose(file);
      fclose(temp_file);

      remove(SCORE_FILE);
      rename("temp.txt", SCORE_FILE);
    }
  }

  return error_code;
}