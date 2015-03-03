#ifndef SNAKE_SRC_SNAKE_H_
#define SNAKE_SRC_SNAKE_H_

#include "point.h"

struct Snake;

struct Snake *snake_init(int x, int y);
void snake_delete(struct Snake *snake);

/* 下面四個函數改變蛇的移動方向但並不移動蛇 */
void snake_face_left(struct Snake *snake);
void snake_face_right(struct Snake *snake);
void snake_face_up(struct Snake *snake);
void snake_face_down(struct Snake *snake);
/* 使蛇暫停移動 */
void snake_pause(struct Snake *snake);
/* 根據蛇當前的移動方向將蛇向前移動一步，
 * 如果咬到自己了返回零，否則返回非零值。 */
int snake_advance(struct Snake *snake);

/* 測試蛇的移動方向，如果在向所測試的方向移動則返回非零值 */
int snake_is_to_left(const struct Snake *snake);
int snake_is_to_right(const struct Snake *snake);
int snake_is_to_up(const struct Snake *snake);
int snake_is_to_down(const struct Snake *snake);
int snake_is_pause(const struct Snake *snake);

/* 返回蛇下一步要走的位置 */
struct Point snake_next_step(const struct Snake *snake);

/* 如果蛇撞到自身就返回非零值 */
int snake_collice_self(const struct Snake *snake);

/* 使蛇吃掉下一步的食物，蛇不必須在移動中 */
void snake_eat(struct Snake *snake);

void snake_paint(const struct Snake *snake);

#endif // SNAKE_SRC_SNAKE_H_
