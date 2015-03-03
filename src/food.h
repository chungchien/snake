#ifndef SNAKE_SRC_FOOD_H_
#define SNAKE_SRC_FOOD_H_

struct Point;

struct Food *food_init();
void food_delete(struct Food *food);
void food_set_pos(struct Food *food, struct Point pos);
const struct Point *food_get_pos(const struct Food *food);
void food_paint(const struct Food *food);

#endif // SNAKE_SRC_FOOD_H_
