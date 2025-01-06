#ifndef TETRIS_MACHINE_H
#define TETRIS_MACHINE_H

#include "../../../gui/cli/interface.h"
#include "../../tetris/tetris.h"

typedef enum {
  StateStart,
  StatePrint,
  StateAction,
  StateShifting,
  StateCalculation,
  StatePause,
  StateGameOver,
  StateClose  // Используется для завершения цикла
} TetrisGameState_t;

// Запускает игру Тетрис
int startTetris(char **console_field, int game, char *name);

// Обрабатывает состояние старта в тетрисе
TetrisGameState_t tetrisStateStart(GameInfo_t *game_info,
                                   TetraInfo_t *tetra_info,
                                   char **console_field, int game, char *name);

// Обрабатывает состояние вывода в тетрисе
TetrisGameState_t tetrisStatePrint(GameInfo_t *game_info,
                                   TetraInfo_t *tetra_info,
                                   char **console_field, UserAction_t *action,
                                   char *name);

// Обрабатывает состояние считывания действия в тетрисе
TetrisGameState_t tetrisStateAction(UserAction_t action, Tetramino_t *tetramino,
                                    int **field, char **console_field,
                                    int *pause, clock_t *start_timer);

// Обрабатывает состояние Смещения фигуры вниз в тетрисе
TetrisGameState_t tetrisStateShifting(clock_t *start_timer,
                                      GameInfo_t *game_info,
                                      TetraInfo_t *tetra_info,
                                      char **console_field);

// Обрабатывает состояние вычисления следующего поля в тетрисе
TetrisGameState_t tetrisStateCalculation(GameInfo_t *game_info,
                                         TetraInfo_t *tetra_info,
                                         char **console_field);

// Обрабатывает состояние завершения работы в тетрисе
TetrisGameState_t tetrisStateGameOver(GameInfo_t *game_info,
                                      TetraInfo_t *tetra_info, int game);

// Обрабатывает состояние паузы в тетрисе
TetrisGameState_t tetrisStatePause(int *pause, char **console_field,
                                   clock_t *start_timer);

// Удаляет следующее тетрамино с навигационного поля
void eraseNextTetramino(char **console_field);

// Добавляет следующее тетрамино на вспомогательное поле
void addNextTetraminoInScreen(Tetramino_t nextTetramino, char **console_field);
#endif