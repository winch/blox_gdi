
#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <stdbool.h>
#include <stdint.h>

#define SHAPE_WIDTH 4
#define SHAPE_HEIGHT 4

typedef struct {
  bool active;
  int x;
  int y;
  uint8_t blocks[SHAPE_WIDTH * SHAPE_HEIGHT];
} shapeBlock;

bool shape_can_move(shapeBlock *shape, gameBoard *board, int x, int y);

void shape_clear(shapeBlock *shape);

void shape_freeze_to_board(shapeBlock *shape, gameBoard *board);

bool shape_random(shapeBlock *shape, gameBoard *board);

void shape_rotate(shapeBlock *shape, gameBoard *board);

void shape_paint(shapeBlock *shape, gameBoard *board, HDC dc);

#endif
