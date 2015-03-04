#include "config.h"
#include <curses.h>
#include <stdlib.h>
#include <assert.h>
#include "snake.h"

enum SnakeMoveDirection {
  SnakeNoMove,
  SnakeMoveRight,
  SnakeMoveLeft,
  SnakeMoveUp,
  SnakeMoveDown
};

struct SnakeNode {
  struct Point pos;
  struct SnakeNode *prev;
  struct SnakeNode *next;
};

struct Snake {
  struct SnakeNode *head;
  struct SnakeNode *tail;
  int direction;  /* 蛇的移動方向 */
};

struct Snake *snake_init(int x, int y)
{
  struct Snake *snake = malloc(sizeof(struct Snake));

  snake->head = malloc(sizeof(struct SnakeNode));
  snake->head->pos.x = x;
  snake->head->pos.y = y;
  snake->head->prev = NULL;
  snake->head->next = NULL;
  snake->tail = snake->head;
  snake->direction = SnakeNoMove;

  return snake;
}

void snake_delete(struct Snake *snake)
{
  struct SnakeNode *p, *n;
  for (p = snake->head; p != NULL; p = n) {
    n = p->next;
    free(p);
  }
  free(snake);
}



void snake_face_left(struct Snake *snake)
{
  snake->direction = SnakeMoveLeft;
}

void snake_face_right(struct Snake *snake)
{
  snake->direction = SnakeMoveRight;
}

void snake_face_up(struct Snake *snake)
{
  snake->direction = SnakeMoveUp;
}

void snake_face_down(struct Snake *snake)
{
  snake->direction = SnakeMoveDown;
}

void snake_pause(struct Snake *snake)
{
  snake->direction = SnakeNoMove;
}

int snake_is_to_left(const struct Snake *snake)
{
  return snake->direction == SnakeMoveLeft;
}

int snake_is_to_right(const struct Snake *snake)
{
  return snake->direction == SnakeMoveRight;
}

int snake_is_to_up(const struct Snake *snake)
{
  return snake->direction == SnakeMoveUp;
}

int snake_is_to_down(const struct Snake *snake)
{
  return snake->direction == SnakeMoveDown;
}

int snake_is_pause(const struct Snake *snake)
{
  return snake->direction == SnakeNoMove;
}

/* 將蛇移動一步，next_step是下一步的位置 */
static void snake_step(struct Snake *snake, struct Point next_step)
{
  snake->tail->pos = next_step;
  if (snake->head != snake->tail) {
    snake->tail->next = snake->head;
    snake->head->prev = snake->tail;
    snake->head = snake->tail;
    snake->tail = snake->tail->prev;
    snake->head->prev = NULL;
    snake->tail->next = NULL;
  }
}

static int is_ate_body(const struct Snake *snake)
{
  const struct SnakeNode *p;
  for (p = snake->head->next; p != NULL; p = p->next) {
    if (point_equal(p->pos, snake->head->pos))
      return 1;
  }
  return 0;
}

int snake_advance(struct Snake *snake)
{
  struct Point next_step = snake_next_step(snake);
  switch (snake->direction) {
    case SnakeMoveRight: snake_step(snake, next_step); break;
    case SnakeMoveLeft: snake_step(snake, next_step); break;
    case SnakeMoveUp: snake_step(snake, next_step); break;
    case SnakeMoveDown: snake_step(snake, next_step); break;
    default: break;
  }

  return !is_ate_body(snake);
}

void snake_eat(struct Snake *snake)
{
  struct SnakeNode *new_head;

  assert(snake->direction != SnakeNoMove);

  new_head = malloc(sizeof(struct SnakeNode));
  new_head->pos = snake_next_step(snake);
  new_head->prev = NULL;
  new_head->next = snake->head;
  snake->head->prev = new_head;
  snake->head = new_head;
}

struct Point snake_next_step(const struct Snake *snake)
{
  struct Point next_step = snake->head->pos;

  switch (snake->direction) {
    case SnakeMoveRight: next_step.x += 1; break;
    case SnakeMoveLeft: next_step.x -= 1; break;
    case SnakeMoveUp: next_step.y -= 1; break;
    case SnakeMoveDown: next_step.y += 1; break;
    default: break;
  }
  return next_step;
}


void snake_paint(const struct Snake *snake)
{
  const struct SnakeNode *p = snake->head;

  /* 𤲯頭 */
  mvaddch(p->pos.y, p->pos.x, 'O');
  p = p->next;

  /* 𤲯身子 */
  while (p) {
    mvaddch(p->pos.y, p->pos.x, '*');
    p = p->next;
  }
}
