#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>

#include "../../essential/essential.h"

#define CONSOLE_WIDTH 20
#define CONSOLE_HEIGHT 22

#define FILLED_PIXEL '#'
#define EMPTY_PIXEL ' '

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

// Заполняет массив с полем консоли
void fillConsoleField(char **console_field);

// Заполняет поле консоли обновленными данными
int refreshConsoleField(char **console_field, GameInfo_t game_info);

// Выводит поле консоли и название состояния
void printConsoleField(char **console_field, char *name_state);

// Считывает нажатия. Возвращает новое значение action
UserAction_t userInput();

// Возвращает новые данные для отрисовки интерфейса
GameInfo_t updateCurrentState(GameInfo_t game_info, int next_level_points,
                              int max_level);

// Инициализирует поле консоли
char **initConsoleField();

// Освобождает память, выделенную под поле консоли
void freeConsoleField(char **console_field);

// Инициализирует библиотеку ncurses
void initNcurses();

// Дублирует строку в новую память
char *strDup(const char *s);

// Возвращает количество разрядов в положительном числе
int intLen(int num);

// Возвращает указанный разряд числа типа int
int getDigit(int num, int digit);

// Обновляет информацию на вспомогательной панели
void updateHelpBar(char **console_field, int score, int high_score, int level);

// Очищает информацию на вспомогательной панели
void cleanHelpBar(char **console_field);

// Выводит на вспомогательную панель сообщение о паузе
void printPauseMessage(char **console_field);

// Стирает сообщение о паузе со вспомогательной панели
void erasePauseMessage(char **console_field);

// Очищает экран на консоли
void cleanScreen(char **console_field);

#endif