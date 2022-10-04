
#include <time.h>
#include <windows.h>

#include "board.h"
#include "game.h"
#include "shape.h"

void game_drop_shape(gameStruct *game) {
  while (shape_can_move(game->shape, game->board, 0, 1)) {
    game->shape->y += 1;
  }
}

bool game_init(gameStruct *game) {
  srand(time(NULL));
  game->board = calloc(1, sizeof(gameBoard));
  if (!game->board) {
    return false;
  }
  game->shape = calloc(1, sizeof(shapeBlock));
  if (!game->shape) {
    free(game->board);
    game->board = NULL;
    return false;
  }
  board_init(game->board, 10, 20, 20, 20);
  return (game->board);
}

void game_free(gameStruct *game) {
  if (game->board) {
    board_free(game->board);
    free(game->board);
    game->board = NULL;
  }
  if (game->shape) {
    free(game->shape);
    game->shape = NULL;
  }
}

void game_rotate_shape(gameStruct *game) {
  shape_rotate(game->shape, game->board);
}

void game_user_input(gameStruct *game, gameInput input) {
  int x = 0;
  int y = 0;
  switch (input) {
  case INPUT_LEFT:
    x--;
    break;
  case INPUT_RIGHT:
    x++;
    break;
  case INPUT_UP:
    y--;
    break;
  case INPUT_DOWN:
    y++;
    break;
  case INPUT_DROP:
    game_drop_shape(game);
    break;
  case INPUT_ROTATE:
    game_rotate_shape(game);
    break;
  }
  if (x != 0 || y != 0) {
    if (shape_can_move(game->shape, game->board, x, y)) {
      game->shape->x += x;
      game->shape->y += y;
    }
  }
}

void game_start(gameStruct *game) { game->running = true; }

void game_step(gameStruct *game) {
  if (game->running) {
    if (game->shape->active) {
      if (shape_can_move(game->shape, game->board, 0, 1)) {
        game->shape->y += 1;
      } else {
        shape_freeze_to_board(game->shape, game->board);
        board_clear_completed_rows(game->board);
        shape_random(game->shape, game->board);
      }
    } else {
      shape_random(game->shape, game->board);
    }
  }
}

void game_paint(gameStruct *game, HDC dc) {
  board_paint(game->board, dc);
  shape_paint(game->shape, game->board, dc);
}
