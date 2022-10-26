
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

void board_remove_row(gameBoard *board, size_t y) {
  for (; y > 0; y--) {
    for (size_t x = 0; x < board->width; x++) {
      uint8_t block = board_get_block(board, x, y - 1);
      board_set_block(board, block, x, y);
    }
  }
}

size_t board_clear_completed_rows(gameBoard *board) {
  size_t lines = 0;
  for (size_t y = board->height - 1; y > 0; y--) {
    bool complete_line = true;
    for (size_t x = 0; x < board->width; x++) {
      if (board_get_block(board, x, y) == 0) {
        complete_line = false;
      }
    }
    if (complete_line == true) {
      board_remove_row(board, y);
      lines += 1;
      y++;
    }
  }
  return lines;
}

void board_init(gameBoard *board, const size_t width, const size_t height,
                int x, int y) {
  board->width = width;
  board->height = height;
  board->block_size = 20;
  board->x = x;
  board->y = y;
  board->block_colour_count = BOARD_BLOCK_COLOUR_COUNT;
  assert(BOARD_BLOCK_COLOUR_COUNT >= 6);
  board->block_colour[0] = CreateSolidBrush(RGB(0, 0, 0));
  board->block_colour[1] = CreateSolidBrush(RGB(255, 0, 0));
  board->block_colour[2] = CreateSolidBrush(RGB(0, 255, 0));
  board->block_colour[3] = CreateSolidBrush(RGB(0, 0, 255));
  board->block_colour[4] = CreateSolidBrush(RGB(255, 255, 0));
  board->block_colour[5] = CreateSolidBrush(RGB(255, 0, 255));
  board->board = calloc(width * height, sizeof(uint8_t));
}

void board_free(gameBoard *board) {
  board->width = 0;
  board->height = 0;
  for (int i = 0; i < BOARD_BLOCK_COLOUR_COUNT; i++) {
    DeleteObject(board->block_colour[i]);
    board->block_colour[i] = NULL;
  }
  if (board->board) {
    free(board->board);
    board->board = NULL;
  }
}

void board_set_block(gameBoard *board, uint8_t block, size_t x, size_t y) {
  assert(x <= board->width);
  assert(y <= board->height);
  *(board->board + x + (board->width * y)) = block;
}

uint8_t board_get_block(gameBoard *board, size_t x, size_t y) {
  assert(x <= board->width);
  assert(y <= board->height);
  return *(board->board + x + (board->width * y));
}

void board_paint_block(gameBoard *board, HDC dc, size_t x, size_t y) {
  RECT r;
  uint8_t block = board_get_block(board, x, y);
  assert(block < BOARD_BLOCK_COLOUR_COUNT);
  r.left = board->x + x * board->block_size;
  r.right = r.left + board->block_size;
  r.top = board->y + y * board->block_size;
  r.bottom = r.top + board->block_size;
  FillRect(dc, &r, board->block_colour[block]);
}

void board_paint(gameBoard *board, HDC dc) {
  for (size_t y = 0; y < board->height; y++) {
    for (size_t x = 0; x < board->width; x++) {
      board_paint_block(board, dc, x, y);
    }
  }
}

bool board_valid_coordinate(gameBoard *board, int x, int y) {
  if (x < 0 || y < 0) {
    return false;
  }
  return y < (int)board->height && x < (int)board->width;
}
