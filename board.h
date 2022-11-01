
#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <windows.h>

#define BOARD_BLOCK_COLOUR_COUNT 10

typedef struct {
  size_t width;
  size_t height;
  size_t block_size;
  int x;
  int y;
  size_t block_colour_count;
  HBRUSH block_colour[BOARD_BLOCK_COLOUR_COUNT];
  uint8_t *board;
} gameBoard;

size_t board_clear_completed_rows(gameBoard *board);

void board_init(gameBoard *board, const size_t width, const size_t height,
                int x, int y);

void board_free(gameBoard *board);

void board_set_block(gameBoard *board, uint8_t block, size_t x, size_t y);

uint8_t board_get_block(gameBoard *board, size_t x, size_t y);

void board_paint(gameBoard *board, HDC dc);

bool board_valid_coordinate(gameBoard *board, int x, int y);

#endif
