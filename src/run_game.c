#include "config.h"
#include <stdlib.h>
#include <curses.h>
#include "point.h"
#include "snake.h"
#include "food.h"
#include "wall.h"

struct Wall {
  int top;
  int left;
  int bottom;
  int right;
};

static int is_on_food(struct Point point, const struct Food *food);

static void game_over();
static void generate_food(struct Food *food);

void run_game()
{
  struct Snake *snake;
  struct Food *food;
  struct Wall *wall;
  int key;

  initscr();
  crmode();
  keypad(stdscr, TRUE);
  noecho();
  clear();

  wall = wall_init(0, 0, LINES - 1, COLS - 1);
  wall_paint(wall);


  food = food_init();
  generate_food(food);
  food_paint(food);

  snake = snake_init(COLS / 2, LINES / 2);
  snake_paint(snake);

  refresh();

  key = getch();
  while (key != ERR && key != 'q') {
    struct Point next_step;

    /* 根據按鍵設置蛇的移動方向 */
    switch (key) {
      case KEY_RIGHT: snake_face_right(snake); break;
      case KEY_LEFT: snake_face_left(snake); break;
      case KEY_UP: snake_face_up(snake); break;
      case KEY_DOWN: snake_face_down(snake); break;
      case ' ':  snake_pause(snake); break;
      default: break;
    }

    next_step = snake_next_step(snake);
    if (is_on_food(next_step, food)) {  /* 如果下一步是食物 */
      snake_eat(snake);
      generate_food(food);
    } else if (wall_is_out_range(wall, &next_step) ||  /* 如果下一步撞到墻 */
               !snake_advance(snake)) {               /* 或是咬到自己 */
      game_over();                                     /* 結果游戲 */
      break;
    }  /* else do nothing */

    erase();
    wall_paint(wall);
    food_paint(food);
    snake_paint(snake);
    refresh();
    key = getch();
  }

  snake_delete(snake);
  food_delete(food);
  endwin();
}

static void game_over()
{
  WINDOW *win = newwin(5, 40, (LINES - 5) / 2, (COLS - 40) / 2);
  wprintw(win, "GAME OVER!");
  wrefresh(win);
  wgetch(win);
  delwin(win);
  touchwin(stdscr);
  wrefresh(stdscr);
}

static void generate_food(struct Food *food)
{
  struct Point pos;
  chtype ch;

  do {
    pos.x = rand() % COLS;
    pos.y = rand() % LINES;
    ch = mvinch(pos.y, pos.x);
  } while (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r');

  food_set_pos(food, pos);
}

static int is_on_food(struct Point point, const struct Food *food)
{
  return point_equal(point, *food_get_pos(food));
}
