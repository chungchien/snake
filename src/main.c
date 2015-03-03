#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

enum OPTION {
  RUN_GAME,
  SHOW_HELP,
  SHOW_VERSION
};

void show_help();
void show_version();
void run_game();

int main(int argc, char *argv[])
{
  int opt;

  struct option longopts[] = {
    {"help", 0, NULL, SHOW_HELP},
    {"version", 0, NULL, SHOW_VERSION},
    {0, 0, 0, 0}
  };

  /* 如果用戶未輸入任何參數，則運行游戲；否則處理參數。 */
  if (argc < 2) {
    run_game();
    exit(EXIT_SUCCESS);
  }

  while ((opt = getopt_long_only(argc, argv, "", longopts, NULL)) != -1) {
    switch (opt) {
      case SHOW_HELP:
        show_help();
        exit(EXIT_SUCCESS);  /* 不再處理後面的選項 */

      case SHOW_VERSION:
        show_version();  /*  */
        exit(EXIT_SUCCESS);

      case '?':
        fprintf(stderr, "unknow option: %c\n", opt);
        break;
    }
  }

  while (optind < argc)
    fprintf(stderr, "unknow argument: %s\n", argv[optind++]);

  exit(EXIT_SUCCESS);
}
