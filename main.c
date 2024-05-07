#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include "ncurses.h"
#include "gui/cli/cli.h"
#include "brick_game/tetris/tetris.h"


int main() {
  FILE *file = fopen("scores.txt", "r");
  if (file == NULL) file = fopen("scores.txt", "w");
  fclose(file);
  initCursesFunc();
  srand(time(NULL));
  int sleep_time = 10000000;
  int tet_template[][4][4] = {
          {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
          {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}},
  };

  gameLoop(sleep_time, tet_template);
  endwin();
  return 0;
}