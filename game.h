
#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

#include "board.h"
#include "shape.h"

typedef struct {
  gameBoard *board;
  shapeBlock *shape;
  int x;
  int y;
  bool running;
  bool over;
  int score;
  int lines;
  int level;
  int start_level;
  int delay;
} gameStruct;

typedef enum {
  INPUT_LEFT,
  INPUT_RIGHT,
  INPUT_UP,
  INPUT_DOWN,
  INPUT_DROP,
  INPUT_ROTATE
} gameInput;

bool game_init(gameStruct *game, int x, int y);

void game_free(gameStruct *game);

void game_user_input(gameStruct *game, gameInput input);

void game_start(gameStruct *game, int level);

void game_step(gameStruct *game);

void game_paint(gameStruct *game, HDC dc);

#endif
