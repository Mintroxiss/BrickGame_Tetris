#ifndef TETRIS_H
#define TETRIS_H

#include <time.h>

#include "../../essential/essential.h"

#define TETRAMINO_BLOCKS 4

#define TETRIS_MAX_LEVEL 10
#define NEXT_LEVEL_POINTS 600

typedef struct {
  int type;
  int pos;
  int *x;
  int *y;
  int move_x;
  int move_y;
} Tetramino_t;

typedef struct {
  Tetramino_t current;
  Tetramino_t next;
} TetraInfo_t;

// Инициализиует тетрамино
Tetramino_t initTetramino();

// Добавляет тетрамино на поле экрана
void addTetraminoInScreen(int **screen_field, Tetramino_t tetramino);

// Удаляет тетрамино с экрана. Возвращает 1, если пытаться удалить тетрамино,
// которого не может существовать
int eraseTetramino(int **screen_field, Tetramino_t tetramino);

// Освобождает память, выделенную под тетрамино
void freeTetramino(Tetramino_t *tetramino);

// Инициализирует структуру tetra_info
TetraInfo_t initTetraInfo();

// Меняет текущее тетрамино на следующую
void nextTetramino(TetraInfo_t *tetra_info);

// Освобождает память, выделенную под tetra_info
void freeTetraInfo(TetraInfo_t tetra_info);

// Проверяет возможность сдвинуть тетрамино  влево
int leftMoveChecking(Tetramino_t *tetramino, int **screen_field);

// Проверяет возможность сдвинуть тетрамино вправо
int rightMoveChecking(Tetramino_t *tetramino, int **screen_field);

// Поворачивает тетрамино с выталкиванием в пределы экрана
void rotateProcess(Tetramino_t *tetramino, int **field);

// Поворачивает тетрамино
void rotateTetramino(Tetramino_t *tetramino);

// Просчитывает следующее поле экрана
void calculateNextScreen(GameInfo_t *game_info, TetraInfo_t tetra_info);

// Просчитывает падение тетрамино
void tetrisGameTick(clock_t *start_timer, int speed, Tetramino_t *tetramino);

// Обрабатывает приземление тетрамино. Возвращает 0 если игра продолжается, 1 -
// если заканчивается
int fallingAnalyse(int **screen_field, TetraInfo_t *tetra_info, int *fl);

// Переносит в поле следующего экрана все строки, кроме заполненных. Возвращает
// количество заполненных строк на текущем экране
int clearFullStrings(int **field, int **next);

// Начисляет очки за заполненные строки
void calculateScore(int *score, int counter);

// Меняет положение тетрамино 1-ого типа
void changePosTetraminoType1(Tetramino_t *tetramino);

// Меняет положение тетрамино 2-ого типа
void changePosTetraminoType2(Tetramino_t *tetramino);

// Меняет положение тетрамино 3-ого типа
void changePosTetraminoType3(Tetramino_t *tetramino);

// Меняет положение тетрамино 4-ого типа
void changePosTetraminoType4(Tetramino_t *tetramino);

// Меняет положение тетрамино 5-ого типа
void changePosTetraminoType5(Tetramino_t *tetramino);

// Меняет положение тетрамино 6-ого типа
void changePosTetraminoType6(Tetramino_t *tetramino);

// Меняет положение тетрамино 7-ого типа
void changePosTetraminoType7(Tetramino_t *tetramino);

#endif