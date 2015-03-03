#include "config.h"
#include <stdio.h>

static const char *help_list[] = {
  "-h: 顯示該幫助\n",
  "-v: 顯示版本號\n",
  "\n",
  "游戲操作說明：\n",
  "\t上下左右鍵移動蛇，空格鍵暫停游戲。當蛇吃到食物時增長一節，\n",
  "\t碰到墻或吃到自身時游戲結束。\n",
  NULL
};

void show_help()
{
  const char **p = help_list;

  while (*p) {
    printf("%s", *p++);
  }
}
