#ifndef MAIN_H
#define MAIN_H

#include "../FSMs/tetris_machine/tetris_machine.h"

typedef enum {
  Tetris,
  ComingSoon,
  EnumCount  // Используется для определения количества игр
} Game_t;

typedef struct {
  Game_t page;
  char **names_pages;
} MenuPages_t;

// Запускает меню BrickGame
int startMenu();

// Приостанавливает программу и ожидает действие
void banner(int *flag, const char *action_name);

// Инициализирует меню
MenuPages_t initMenu();

// Обрабатывает вывод страницы меню
void currentMenuPage(char **console_field, MenuPages_t menu);

// Обрабатывает нажатия в меню
void menuActionProcessing(UserAction_t menu_action, int *end_flag,
                          MenuPages_t *menu, char **console_field);

// Запускает выбранную игру
void startGame(MenuPages_t *menu, char **console_field);

// Заполняет поле консоли соответствующей страницей меню
void fillScreenMenuPage(char **console_field, MenuPages_t menu);

// Освобождает память menu
void freeMenu(MenuPages_t menu);

#endif