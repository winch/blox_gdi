
#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "shape.h"

uint8_t shape_get_block(shapeBlock *shape, size_t x, size_t y);

bool shape_can_move(shapeBlock *shape, gameBoard *board, int x_offset,
                    int y_offset) {
  assert(x_offset >= -1 && x_offset <= 1);
  assert(y_offset >= -1 && y_offset <= 1);
  int shape_new_x = shape->x + x_offset;
  int shape_new_y = shape->y + y_offset;
  for (size_t y = 0; y < SHAPE_HEIGHT; y++) {
    for (size_t x = 0; x < SHAPE_WIDTH; x++) {
      int board_x = shape_new_x + x;
      int board_y = shape_new_y + y;
      if (board_valid_coordinate(board, board_x, board_y)) {
        if (shape_get_block(shape, x, y) > 0 &&
            board_get_block(board, board_x, board_y) > 0) {
          return false;
        }
      } else {
        if (shape_get_block(shape, x, y) > 0) {
          return false;
        }
      }
    }
  }
  return true;
}

void shape_clear(shapeBlock *shape) {
  shape->active = false;
  shape->x = 0;
  shape->y = 0;
  for (int i = 0; i < SHAPE_WIDTH * SHAPE_HEIGHT; i++) {
    shape->blocks[i] = 0;
  }
}

void shape_freeze_to_board(shapeBlock *shape, gameBoard *board) {
  shape->active = false;
  for (size_t y = 0; y < SHAPE_HEIGHT; y++) {
    for (size_t x = 0; x < SHAPE_WIDTH; x++) {
      int board_x = shape->x + x;
      int board_y = shape->y + y;
      if (board_valid_coordinate(board, board_x, board_y)) {
        uint8_t colour = shape_get_block(shape, x, y);
        if (colour > 0) {
          board_set_block(board, colour, board_x, board_y);
        }
      }
    }
  }
}

void shape_set_block(shapeBlock *shape, uint8_t block, size_t x, size_t y) {
  assert(x < SHAPE_WIDTH);
  assert(y < SHAPE_HEIGHT);
  *(shape->blocks + x + (SHAPE_WIDTH * y)) = block;
}

uint8_t shape_get_block(shapeBlock *shape, size_t x, size_t y) {
  assert(x < SHAPE_WIDTH);
  assert(y < SHAPE_HEIGHT);
  return *(shape->blocks + x + (SHAPE_WIDTH * y));
}

bool shape_random(shapeBlock *shape, gameBoard *board) {
  shape_clear(shape);
  shape->active = true;
  shape->x = 3;
  shape->y = -1;
  size_t colour = (rand() % (board->block_colour_count - 1)) + 1;
  assert(colour < BOARD_BLOCK_COLOUR_COUNT);
  assert(colour > 0);
  switch (rand() % 7) {
  case 0:
    // O shape
    shape_set_block(shape, colour, 1, 1);
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 1, 2);
    shape_set_block(shape, colour, 2, 2);
    break;
  case 1:
    // I shape
    shape_set_block(shape, colour, 0, 1);
    shape_set_block(shape, colour, 1, 1);
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 3, 1);
    break;
  case 2:
    // S shape
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 3, 1);
    shape_set_block(shape, colour, 1, 2);
    shape_set_block(shape, colour, 2, 2);
    break;
  case 3:
    // Z shape
    shape_set_block(shape, colour, 1, 1);
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 2, 2);
    shape_set_block(shape, colour, 3, 2);
    break;
  case 4:
    // L shape
    shape_set_block(shape, colour, 1, 1);
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 3, 1);
    shape_set_block(shape, colour, 1, 2);
    break;
  case 5:
    // J shape
    shape_set_block(shape, colour, 1, 1);
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 3, 1);
    shape_set_block(shape, colour, 3, 2);
    break;
  case 6:
    // T shape
    shape_set_block(shape, colour, 1, 1);
    shape_set_block(shape, colour, 2, 1);
    shape_set_block(shape, colour, 3, 1);
    shape_set_block(shape, colour, 2, 2);
    break;
  }
  return shape_can_move(shape, board, 0, 0);
}

void shape_rotate(shapeBlock *shape, gameBoard *board) {
  shapeBlock rotated;
  rotated.active = true;
  rotated.x = shape->x;
  rotated.y = shape->y;
  for (int x = 0; x < SHAPE_WIDTH; x++) {
    for (int y = 0; y < SHAPE_HEIGHT; y++) {
      uint8_t block = shape_get_block(shape, x, y);
      size_t new_x = y;
      size_t new_y = abs(x - (SHAPE_WIDTH - 1));
      shape_set_block(&rotated, block, new_x, new_y);
    }
  }
  if (shape_can_move(&rotated, board, 0, 0)) {
    memcpy(shape->blocks, &rotated.blocks, sizeof(rotated.blocks));
  }
}

void shape_paint_block(gameBoard *board, HDC dc, size_t x, size_t y,
                       uint8_t colour) {
  RECT r;
  assert(colour < BOARD_BLOCK_COLOUR_COUNT);
  r.left = board->x + x * board->block_size;
  r.right = r.left + board->block_size;
  r.top = board->y + y * board->block_size;
  r.bottom = r.top + board->block_size;
  FillRect(dc, &r, board->block_colour[colour]);
}

void shape_paint(shapeBlock *shape, gameBoard *board, HDC dc) {
  if (shape->active) {
    for (size_t y = 0; y < SHAPE_HEIGHT; y++) {
      for (size_t x = 0; x < SHAPE_WIDTH; x++) {
        int board_x = shape->x + x;
        int board_y = shape->y + y;
        if (board_valid_coordinate(board, board_x, board_y)) {
          uint8_t colour = shape_get_block(shape, x, y);
          if (colour > 0) {
            shape_paint_block(board, dc, board_x, board_y, colour);
          }
        }
      }
    }
  }
}
