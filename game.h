
#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

#include "board.h"
#include "shape.h"

typedef struct {
  gameBoard *board;
  shapeBlock *shape;
  bool running;
} gameStruct;

typedef enum {
  INPUT_LEFT,
  INPUT_RIGHT,
  INPUT_UP,
  INPUT_DOWN,
  INPUT_DROP,
  INPUT_ROTATE
} gameInput;

bool game_init(gameStruct *game);

void game_free(gameStruct *game);

void game_user_input(gameStruct *game, gameInput input);

void game_start(gameStruct *game);

void game_step(gameStruct *game);

void game_paint(gameStruct *game, HDC dc);

#endif
