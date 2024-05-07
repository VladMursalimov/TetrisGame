/************************************************************
 * @file cli.c
 * @brief CLI GUI source
 ************************************************************/

#include "cli.h"

#include <stdlib.h>
#ifdef __linux__
#include <threads.h>
#endif
#include <time.h>
#include <unistd.h>

#include "../../brick_game/tetris/tetris.h"

void initCursesFunc() {
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  attron(COLOR_PAIR(1));
  mvprintw(
      10, 20,
      "||S TO START||P TO PAUSE||ARROWS TO MOVE||R TO ROTATE||Q TO QUIT||");
  attroff(COLOR_PAIR(1));
}

int scoreFromFile() {
  FILE *file;
  char filename[] = "scores.txt";
  file = fopen(filename, "r");
  char line[256];
  int high_score = 0;
  if (fgets(line, 256, file) != NULL) {
    high_score = atoi(line);
  }
  fclose(file);
  return high_score;
}

void gameLoop(int sleep_time, int tet_template[][4][4]) {
  TetGame *tgame = createGame(4, 7, tet_template);
  GameInfo_t *gameInfo = createGameInfo();
  tgame->info = getCurrentState(gameInfo);
  TetPlayer player;
  tgame->player = &player;
  tgame->info->high_score = scoreFromFile();
  createNextFigure(tgame);
  dropNewFigure(tgame);
  createNextFigure(tgame);
  while (tgame->playing_state != TET_GAMEOVER) {
    int action = getch();
    switch (action) {
      case KEY_LEFT:
        player.action = Left;
        break;
      case KEY_UP:
        player.action = Up;
        break;
      case KEY_RIGHT:
        player.action = Right;
        break;
      case KEY_DOWN:
        player.action = Down;
        break;
      case 'r':
        player.action = Action;
        break;
      case 'q':
        player.action = Terminate;
        break;
      case 'p':
        player.action = Pause;
        tgame->info->speed = 0;
        tgame->info->pause = Pause;
        break;
      case 's':
        player.action = Start;
        tgame->info->speed = tgame->info->level;
        tgame->info->pause = Start;
        break;
      default:
        player.action = Pause;
        break;
    }
    *(tgame->info) = updateCurrentState();
    userInput(player.action, 1);
    calculateTact(tgame);
    printTetGame(tgame);
#ifdef __linux__
    thrd_sleep(&(struct timespec){.tv_nsec = sleep_time}, NULL);
#endif
#ifdef __APPLE__
    nanosleep(&(struct timespec){.tv_nsec = sleep_time}, NULL);
#endif
    refresh();
  }
  freeGame(tgame);
  freeGameInfo(gameInfo);
}
