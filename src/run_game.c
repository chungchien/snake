#include "config.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <curses.h>
#include "constants.h"
#include "point.h"
#include "snake.h"
#include "food.h"
#include "wall.h"

static struct Snake *snake;
static struct Food *food;
static struct Wall *wall;

static void game_over();
static void generate_food(struct Food *food);
static void advance(int signo);
static void redraw();
static void init_my_colors();

/* 在shot_after毫秒後起動定時器，間隔interval發送一個SIGALARM信號 */
static void start_timer(int shot_after, int interval);
/* 停止定時器 */
static void stop_timer();

void run_game()
{
  /* struct Snake *snake; */
  /* struct Food *food; */
  /* struct Wall *wall; */
  int key;

  /* 初始化curses环境 */
  initscr();
  if (has_colors() && start_color() == OK) {
    init_my_colors();
  }
  crmode();
  keypad(stdscr, TRUE);
  noecho();
  clear();

  /* 初始化墙 */
  wall = wall_init(0, 0, LINES - 1, COLS - 1);

  /* 初始化蛇 */
  snake = snake_init(COLS / 2, LINES / 2);

  /* 初始化食物 */
  food = food_init();
  generate_food(food);

  redraw();

  if (signal(SIGALRM, advance) == SIG_ERR) {
    mvprintw(5, 10, "signal error: %s", strerror(errno));
    endwin();
    return;
  }
  start_timer(10, DEFAULT_SNAKE_STEP_INTERVAL);

  key = getch();
  while (key != ERR && key != 'q') {
    /* 根據按鍵設置蛇的移動方向 */
    switch (key) {
      case KEY_RIGHT: snake_face_right(snake); break;
      case KEY_LEFT: snake_face_left(snake); break;
      case KEY_UP: snake_face_up(snake); break;
      case KEY_DOWN: snake_face_down(snake); break;
      case ' ':  snake_pause(snake); break;
      default: break;
    }

    key = getch();
  }

  snake_delete(snake);
  food_delete(food);
  endwin();
}

static void game_over()
{
  WINDOW *win;

  stop_timer();                                   /* 停止定時器 */

  win = newwin(5, 30, (LINES - 5) / 2, (COLS - 30) / 2);

  box(win, ACS_VLINE, ACS_HLINE);
  wmove(win, 2, 9);
  wprintw(win, "GAME OVER!!!");
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

static void start_timer(int shot_after, int interval)
{
  struct itimerval itimerval;

  itimerval.it_value.tv_sec = shot_after / 1000;
  itimerval.it_value.tv_usec = shot_after % 1000 * 1000;
  itimerval.it_interval.tv_sec = interval / 1000;
  itimerval.it_interval.tv_usec = interval % 1000 * 1000;
  if (setitimer(ITIMER_REAL, &itimerval, NULL) < 0) {
    mvprintw(5, 10, "setitimer error: %s", strerror(errno));
    endwin();
    return;
  }
}

static void stop_timer()
{
  struct itimerval itimerval;

  itimerval.it_value.tv_sec = 0;
  itimerval.it_value.tv_usec = 0;
  itimerval.it_interval.tv_sec = 0;
  itimerval.it_interval.tv_usec = 0;
  if (setitimer(ITIMER_REAL, &itimerval, NULL) < 0) {
    mvprintw(5, 10, "setitimer error: %s", strerror(errno));
    endwin();
    return;
  }

}

static void redraw()
{
  erase();
  wall_paint(wall);
  snake_paint(snake);
  food_paint(food);

  /* 将光标放在食物所在位置 */
  move(food_get_pos(food)->y, food_get_pos(food)->x);

  refresh();
}

static void advance(int signo)
{
  struct Point next_step;

  next_step = snake_next_step(snake);
  if (point_equal(next_step, *food_get_pos(food))) {  /* 如果下一步是食物 */
    snake_eat(snake);
    generate_food(food);
  } else if (wall_is_out_range(wall, &next_step) ||  /* 如果下一步撞到墻 */
             !snake_advance(snake)) {               /* 或是咬到自己 */
    game_over();                                     /* 結果游戲 */
    return;
  }  /* else do nothing */

  redraw();
}

static void init_my_colors()
{
  init_pair(WALL_COLOR, COLOR_GREEN, COLOR_GREEN);
  init_pair(FOOD_COLOR, COLOR_YELLOW, COLOR_BLACK);
  init_pair(SNAKE_HEAD_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(SNAKE_BODY_COLOR, COLOR_CYAN, COLOR_WHITE);
  init_pair(ERROR_COLOR, COLOR_RED, COLOR_WHITE);
  init_pair(ERROR_COLOR, COLOR_GREEN, COLOR_WHITE);
}
