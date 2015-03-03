#include <curses.h>
#include <stdlib.h>
#include "food.h"
#include "point.h"

struct Food {
  struct Point pos;
};

struct Food *food_init()
{
  struct Food *food = malloc(sizeof(struct Food));
  food->pos.x = 0;
  food->pos.y = 0;
  return food;
}

void food_delete(struct Food *food)
{
  free(food);
}

void food_set_pos(struct Food *food, struct Point pos)
{
  food->pos = pos;
}

const struct Point *food_get_pos(const struct Food *food)
{
  return &food->pos;
}

void food_paint(const struct Food *food)
{
  mvaddch(food->pos.y, food->pos.x, '@');
}
