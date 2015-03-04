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

enum GAME_STATU {
  GAME_STOPED  = 0,
  GMAE_RUNNING = 1,
  GAME_PAUSED  = 2
};

static int game_status;
static struct Snake *snake = NULL;
static struct Food *food = NULL;
static struct Wall *wall = NULL;

static void init_screen();    /* 初始化屏幕顯示設置 */
static void init_my_colors();  /* 初始化自定義的顏色 */
static void show_options();    /* 顯示操作說明 */
static void new_game();        /* 結束當前游戲開始新游戲 */
static void end_game();        /* 結束游戲 */
static void pause_game();      /* 暫停游戲 */
static void resum_game();      /* 繼續游戲 */
static void redraw();          /* 重繪界面 */
static void game_over();       /* 顯示 GAME OVER 信息並結束該局游戲 */
static void advance();         /* 更新動畫畫面 */
static void generate_food();   /* 爲食物生成一個位置 */
static void sig_alarm(int);    /* 處理 SIGALARM */

/* 在when_short毫秒後起動定時器，每隔interval發送一個SIGALARM信號 */
static void start_timer(int when_shot, int interval);
/* 停止定時器 */
static void stop_timer();

void run_game()
{
  int key;

  init_screen();  /* 初始化終端配置 */

  show_options();  /* 顯示操作說明 */

  new_game(); /* 開始新游戲 */

  key = getch();
  while (key != ERR && key != 'q') {
    /* 根據按鍵設置蛇的移動方向 */
    switch (key) {
      case KEY_RIGHT:
        if (game_status) snake_face_right(snake);
        break;
      case KEY_LEFT:
        if (game_status) snake_face_left(snake);
        break;
      case KEY_UP:
        if (game_status) snake_face_up(snake);
        break;
      case KEY_DOWN:
        if (game_status) snake_face_down(snake);
        break;
      case ' ':
        if (game_status) snake_pause(snake);
        break;
      case 'n': new_game(); break;
      case 'h': show_options(); break;
      default: break;
    }

    key = getch();
  }

  end_game();
  endwin();
}

static void show_options()
{
  const char title[] = "OPERATING INSTRUCTION";
  const char prompt[] = "(Press any key back to the game)";
  const char *keys[] = {
    "left arrow",
    "right arrow",
    "up arrow",
    "down arrow",
    "space",
    "n",
    "q",
    "h"
  };
  const char *options[] = {
    "move left",
    "move right",
    "move up",
    "move down",
    "pause",
    "new game",
    "exit game",
    "show operating instruction"
  };

  int i, n, width, height;
  WINDOW *win;

  pause_game();

  n = sizeof(options) / sizeof(options[0]);
  width = 50;
  height = n + 7;

  win = newwin(height, width, 3, 10);
  box(win, ACS_VLINE, ACS_HLINE);

  wattron(win, A_BOLD);
  wmove(win, 1, (width - sizeof(title)) / 2);
  wprintw(win, title);
  wattroff(win, A_BOLD);

  wmove(win, 3, 3);
  wprintw(win, prompt);

  wattron(win, A_BOLD | A_UNDERLINE);
  for (i = 0; i < n; i++) {
    wmove(win, i + 5, 3);
    wprintw(win, keys[i]);
  }
  wattroff(win, A_BOLD | A_UNDERLINE);

  for (i = 0; i < n; i++) {
    wmove(win, i + 5, 18);
    wprintw(win, options[i]);
  }

  wrefresh(win);

  wgetch(win);
  delwin(win);
  touchwin(stdscr);
  resum_game();
}

static void init_screen()
{
  initscr();
  if (has_colors() && start_color() == OK) {
    init_my_colors();
  }
  crmode();
  keypad(stdscr, TRUE);
  noecho();
  clear();
}

static void new_game()
{
  if (game_status)
    end_game();  /* 如果游戲正在進行或是暫停狀態，先結束 */

  wall = wall_init(0, 0, LINES - 1, COLS - 1);  /* 初始化墻 */
  snake = snake_init(COLS / 2, LINES / 2);      /* 初始化蛇 */
  food = food_init();                           /* 初始化食物 */
  generate_food();

  redraw();  /* 繪制游戲畫面 */

  /* 安裝定時器處理函數 */
  if (signal(SIGALRM, sig_alarm) == SIG_ERR) {
    mvprintw(5, 10, "signal error: %s", strerror(errno));
    endwin();
    exit(EXIT_FAILURE);
  }
  /* 啟動定時器 */
  start_timer(10, DEFAULT_SNAKE_STEP_INTERVAL);

  game_status = GMAE_RUNNING;
}

static void end_game()
{
  game_status = GMAE_STOPED;

  stop_timer();
  if (snake) {
    snake_delete(snake);
    snake = NULL;
  }
  if (food) {
    food_delete(food);
    food = NULL;
  }
  if (wall) {
    wall_delete(wall);
    wall = NULL;
  }
}

static void pause_game()
{
  if (game_status == GMAE_RUNNING) {
    stop_timer();
    game_status = GAME_PAUSED;
  }
}

static void resum_game()
{
  if (game_status == GAME_PAUSED) {
    start_timer(1, DEFAULT_SNAKE_STEP_INTERVAL);
    game_status = GMAE_RUNNING;
  }
}

static void game_over()
{
  WINDOW *win;

  //stop_timer();                                   /* 停止定時器 */
  end_game();

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

static void generate_food()
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

static void advance()
{
  struct Point next_step;

  next_step = snake_next_step(snake);
  if (point_equal(next_step, *food_get_pos(food))) {
    /* 如果下一步是食物 */
    snake_eat(snake);  /* 蛇進食 */
    generate_food();   /* 生成新的食物 */
  } else if (wall_is_out_range(wall, &next_step) || !snake_advance(snake)) {
    /* 如果下一步蛇會撞到墻或前進之後咬到自己 */
    game_over();    /* 結束該局游戲 */
    return;
  }  /* else do nothing */

  redraw();  /* 重繪畫面 */
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

static void sig_alarm(int signo)
{
  advance();
}
