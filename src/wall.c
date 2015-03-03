#include <curses.h>
#include <stdlib.h>
#include <memory.h>
#include "wall.h"
#include "point.h"

struct Wall {
  int top;
  int left;
  int bottom;
  int right;
};

struct Wall *wall_init(int top, int left, int bottom, int right)
{
  struct Wall *wall = malloc(sizeof(struct Wall));

  wall->top = top;
  wall->left = left;
  wall->bottom = bottom;
  wall->right = right;
  return wall;
}

void wall_delete(struct Wall *wall)
{
  free(wall);
}

int wall_is_out_range(const struct Wall *wall, const struct Point *point)
{
  return point->x <= wall->left || point->x >= wall->right ||
      point->y <= wall->top || point->y >= wall->bottom;
}

void wall_paint(const struct Wall *wall)
{
  int x, y;

  /* 畫行 */
  for (x = wall->left; x < wall->right; x++) {
    mvaddch(wall->top, x, '-');
    mvaddch(wall->bottom, x + 1, '-');
  }

  /* 畫列 */
  for (y = wall->top; y < wall->bottom; y++) {
    mvaddch(y + 1, wall->left, '|');
    mvaddch(y, wall->right, '|');
  }
}
