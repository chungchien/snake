#include "config.h"
#include <stdio.h>

static const char *help_list[] = {
  "usage: snake [--version] [--help]\n",
  "\n"
  "--help     Print usage information and exit.\n",
  "--version  Print version number and copyright, then exit.\n",
  NULL
};

void show_help()
{
  const char **p = help_list;

  while (*p) {
    printf("%s", *p++);
  }
}
