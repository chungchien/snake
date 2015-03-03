#include "config.h"
#include <printf.h>

void show_version()
{
  printf("%s %d.%d.%d\n", PROJECT_NAME, MAJOR_VERSION,
         MINOR_VERSION, PATCH_VERSION);
  /* printf("%s\n", PROGRAM_COPYRIGHT); */
  /* printf("\n"); */
  /* printf("%s\n", PROGRAM_LICENCE); */
}
