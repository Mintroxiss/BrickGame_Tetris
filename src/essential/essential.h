#ifndef ESSENTIAL_H
#define ESSENTIAL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 20

#define START_SPEED 2000000
#define END_SPEED 500000

#define SCORE_FILE "score_list.txt"

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

// Инициализирует game_info
GameInfo_t initGameInfo(int game);

// Освобождает структуру game_info
void freeGameInfo(GameInfo_t game_info);

// Инициализирует поле экрана
int **initScreenField();

// Освобождает память, выделенную под поле экрана
void freeScreenField(int **field);

// Считывает рекорд из файла и возвращает его
int getHighScore(char *filename, int line_number);

// Обновляет информацию о рекорде в файле. Возвращает код ошибки
int updateHighScore(int new_number, int line_number);

// Возвращает рандомное число в диапозоне от минимального до максимального
// включительно
int randNum(int min, int max);

#endif
