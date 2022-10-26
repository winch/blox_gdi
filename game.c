
#include <time.h>
#include <windows.h>
#include <stdio.h>

#include "board.h"
#include "game.h"
#include "shape.h"

void game_drop_shape(gameStruct *game) {
  while (shape_can_move(game->shape, game->board, 0, 1)) {
    game->shape->y += 1;
  }
}

bool game_init(gameStruct *game, int x, int y) {
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
  game->x = x;
  game->y = y;
  board_init(game->board, 10, 20, game->x + 20, game->y + 20);
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

int game_get_delay_for_level(int level) {
  int delay = ((11 - level) * 50);
  return delay;
}

void game_update_level(gameStruct *game) {
  int level = (game->lines / 10) + 1;
  level += game->start_level - 1;
  if (level > 10) {
    level = 10;
  }
  game->level = level;
  game->delay = game_get_delay_for_level(game->level);
}

void game_start(gameStruct *game, int level) {
  game->running = true;
  game->over = false;
  game->score = 0;
  game->lines = 9;
  game->level = level;
  game->start_level = game->level;
  game->delay = game_get_delay_for_level(game->level);
}

void game_step(gameStruct *game) {
  if (game->running) {
    if (game->shape->active) {
      if (shape_can_move(game->shape, game->board, 0, 1)) {
        game->shape->y += 1;
      } else {
        shape_freeze_to_board(game->shape, game->board);
        int lines = board_clear_completed_rows(game->board);
        game->lines += lines;
        if (lines > 0) {
          game_update_level(game);
        }
        if (shape_random(game->shape, game->board) == false) {
          game->running = false;
          game->over = true;
        }
      }
    } else {
      shape_random(game->shape, game->board);
    }
  }
}

void game_paint(gameStruct *game, HDC dc) {
  char text[255];
  int text_length = sizeof(text);
  board_paint(game->board, dc);
  shape_paint(game->shape, game->board, dc);
  // draw scoreboard
  RECT r;
  r.left = game->board->x + 20 + (game->board->width * game->board->block_size);
  r.right = r.left + 60;
  r.top = game->y + 20;
  r.bottom = r.top + 50;
  snprintf(text, text_length, "score : %d\nlines : %d\nlevel : %d", game->score, game->lines, game->level);
  DrawText(dc, text, -1, &r, DT_EDITCONTROL);
}
