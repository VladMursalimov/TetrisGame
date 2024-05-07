/************************************************************
 * @file tetris.c
 * @brief Game logic library source
 ************************************************************/

#ifndef TETRIS_C
#define TETRIS_C

#include "tetris.h"

#include "stdlib.h"

TetFigure *createFigure(TetGame *tgame) {
  TetFigure *newfigure = (TetFigure *)calloc(1, sizeof(TetFigure));
  newfigure->x = 0;
  newfigure->y = 0;
  newfigure->size = tgame->figuresT->size;
  newfigure->block = calloc(4, sizeof(int *));
  if (newfigure->block == NULL) {
    return NULL;
  }
  for (int i = 0; i < 4; i++) {
    newfigure->block[i] = calloc(4, sizeof(int));
    if (newfigure->block[i] == NULL) {
      return NULL;
    }
    for (int j = 0; j < 4; j++) {
      newfigure->block[i][j] = 0;
    }
  }
  return newfigure;
}

void freeFigure(TetFigure *figure) {
  if (figure) {
    if (figure->block) {
      for (int i = 0; i < 4; i++) {
        free(figure->block[i]);
      }
      free(figure->block);
    }
    free(figure);
  }
}

TetFiguresType *createFiguresType(int count, int figures_size,
                                  int figures_template[][4][4]) {
  TetFiguresType *tfigtype =
      (TetFiguresType *)calloc(1, sizeof(TetFiguresType));
  tfigtype->count = count;
  tfigtype->size = figures_size;
  tfigtype->blocks = (int ***)malloc(count * sizeof(int **));
  for (int i = 0; i < count; i++) {
    tfigtype->blocks[i] = (int **)malloc(4 * sizeof(int *));
    for (int j = 0; j < 4; j++) {
      tfigtype->blocks[i][j] = (int *)malloc(4 * sizeof(int));
      for (int k = 0; k < 4; k++) {
        tfigtype->blocks[i][j][k] = figures_template[i][j][k];
      }
    }
  }
  return tfigtype;
}

void freeFiguresType(TetFiguresType *tfigtype) {
  if (tfigtype) {
    for (int i = 0; i < tfigtype->count; i++) {
      for (int j = 0; j < 4; j++) {
        free(tfigtype->blocks[i][j]);
      }
      free(tfigtype->blocks[i]);
    }
    free(tfigtype->blocks);
    free(tfigtype);
  }
}

int **createField() {
  int **field = calloc(windowHeight, sizeof(int *));
  if (field == NULL) {
    return NULL;
  }
  for (int i = 0; i < windowHeight; i++) {
    field[i] = calloc(windowWidth, sizeof(int));
    if (field[i] == NULL) {
      return NULL;
    }
    for (int j = 0; j < windowWidth; j++) {
      field[i][j] = 0;
    }
  }
  return field;
}

void deleteField(int **field) {
  if (field) {
    for (int i = 0; i < windowHeight; i++) {
      free(field[i]);
    }
    free(field);
  }
}

GameInfo_t *createGameInfo() {
  GameInfo_t *gameinfo = (GameInfo_t *)calloc(1, sizeof(GameInfo_t));
  if (gameinfo == NULL) {
    exit(1);
  }
  gameinfo->field = createField();
  gameinfo->next = NULL;
  gameinfo->score = 0;
  gameinfo->high_score = 0;
  gameinfo->level = 1;
  gameinfo->speed = 0;
  gameinfo->pause = Pause;
  return gameinfo;
}

void freeGameInfo(GameInfo_t *gameinfo) {
  if (gameinfo == NULL) {
    return;
  }
  if (gameinfo->field != NULL) {
    deleteField(gameinfo->field);
    gameinfo->field = NULL;
  }
  if (gameinfo->next != NULL) {
    free(gameinfo->next);
    gameinfo->next = NULL;
  }
  free(gameinfo);
}

TetGame *createGame(int figure_size, int count, int figures_template[][4][4]) {
  TetGame *tgame = (TetGame *)calloc(1, sizeof(TetGame));
  tgame->figuresT = createFiguresType(count, figure_size, figures_template);
  tgame->ticks = 30;
  tgame->ticks_left = 30;
  tgame->playing_state = TET_PLAYING;
  tgame->figure = NULL;
  tgame->nextfigure = NULL;
  return tgame;
}

void freeGame(TetGame *tgame) {
  if (tgame) {
    freeFiguresType(tgame->figuresT);
    freeFigure(tgame->figure);
    freeFigure(tgame->nextfigure);
    free(tgame);
  }
}

void moveFigureEnd(TetGame *tgame) {
  while (!collisionTet(tgame)) {
    tgame->figure->y++;
  }
}

void moveFigureDown(TetGame *tgame) { tgame->figure->y++; }

void moveFigureUp(TetGame *tgame) { tgame->figure->y--; }

void moveFigureRight(TetGame *tgame) { tgame->figure->x++; }

void moveFigureLeft(TetGame *tgame) { tgame->figure->x--; }

int collisionTet(TetGame *tgame) {
  TetFigure *figure = tgame->figure;
  int **field = tgame->info->field;
  for (int i = 0; i < figure->size; i++) {
    for (int j = 0; j < figure->size; j++) {
      if (figure->block[i][j] != 0) {
        int bx = figure->x + j;
        int by = figure->y + i;
        if (bx < 0 || bx >= windowWidth || by < 0 || by >= windowHeight) {
          return 1;
        }
        if (field[by][bx] != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}

void placeFigure(TetGame *tgame) {
  TetFigure *figure = tgame->figure;
  int **field = tgame->info->field;
  for (int i = 0; i < figure->size; i++) {
    for (int j = 0; j < figure->size; j++) {
      if (figure->block[i][j] != 0) {
        int bx = figure->x + j;
        int by = figure->y + i;
        field[by][bx] = figure->block[i][j];
      }
    }
  }
}

int lineFilled(int line, int **field) {
  for (int j = 0; j < windowWidth; j++) {
    if (field[line][j] == 0) {
      return 0;
    }
  }
  return 1;
}

void dropLine(int line, int **field) {
  if (line == 0) {
    for (int j = 0; j < windowWidth; j++) {
      field[j] = 0;
    }
  } else {
    for (int k = line; k > 0; k--) {
      for (int j = 0; j < windowWidth; j++) {
        field[k][j] = field[(k - 1)][j];
      }
    }
  }
}

int eraseLines(TetGame *tgame) {
  int **field = tgame->info->field;
  int count = 0;
  for (int i = windowHeight - 1; i >= 0; i--) {
    while (lineFilled(i, field)) {
      dropLine(i, field);
      count++;
    }
  }
  if (count == 1)
    count = 100;
  else if (count == 2)
    count = 300;
  else if (count == 3)
    count = 700;
  else if (count == 4)
    count = 1500;
  return count;
}

void dropNewFigure(TetGame *tgame) {
  TetFigure *newfigure = createFigure(tgame);
  newfigure->y = -1;
  newfigure->x = windowWidth / 2 - newfigure->size / 2;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      newfigure->block[i][j] = tgame->nextfigure->block[i][j];
    }
  }
  freeFigure(tgame->figure);
  tgame->figure = newfigure;
}

void createNextFigure(TetGame *tgame) {
  TetFigure *nextfigure = createFigure(tgame);
  int nfsize = nextfigure->size;
  nextfigure->x = 0;
  nextfigure->y = 0;
  int fnum = rand() % tgame->figuresT->count;
  for (int i = 0; i < nfsize; i++) {
    for (int j = 0; j < nfsize; j++) {
      nextfigure->block[i][j] = tgame->figuresT->blocks[fnum][i][j];
    }
  }
  freeFigure(tgame->nextfigure);
  tgame->nextfigure = nextfigure;
}

TetFigure *rotateFigure(TetGame *tgame) {
  TetFigure *newf = createFigure(tgame);
  TetFigure *oldf = tgame->figure;
  newf->x = oldf->x;
  newf->y = oldf->y;
  for (int i = 0; i < newf->size; i++) {
    for (int j = 0; j < newf->size; j++) {
      newf->block[i][j] = oldf->block[newf->size - 1 - j][i];
    }
  }
  return newf;
}

void scoreToFile(int high_score) {
  FILE *file;
  char filename[] = "scores.txt";
  file = fopen(filename, "w");
  fprintf(file, "%d", high_score);
  fclose(file);
}

void calculateTact(TetGame *tgame) {
  if (tgame->ticks_left <= 0) {
    tgame->ticks_left = tgame->ticks;
    moveFigureDown(tgame);
    if (collisionTet(tgame)) {
      moveFigureUp(tgame);
      placeFigure(tgame);
      tgame->info->score += eraseLines(tgame);
      if (tgame->info->score > tgame->info->high_score) {
        scoreToFile(tgame->info->score);
        tgame->info->high_score = tgame->info->score;
      }
      if ((600 * tgame->info->level <= tgame->info->score) &&
          tgame->info->level < 10) {
        tgame->info->speed++;
        tgame->info->level++;
      }
      dropNewFigure(tgame);
      createNextFigure(tgame);

      if (collisionTet(tgame)) {
        tgame->playing_state = TET_GAMEOVER;
        return;
      }
    }
  }
  doActions(tgame);
  tgame->ticks_left -= tgame->info->speed;
}

void printNextFigure(TetGame *tgame) {
  TetFigure *next = tgame->nextfigure;
  for (int i = 0; i < 4; i++) {
    for (int j = windowWidth + 1; j < windowWidth + 5; j++) {
      int sym = 1;
      int x = j - next->x - windowWidth - 1;
      int y = i - next->y;
      if (next->block[y][x]) sym = 2;
      attron(COLOR_PAIR(sym));
      mvaddch(i, j, ' ');
      attroff(COLOR_PAIR(sym));
    }
  }
}

void copyField(int **field1, int **field2) {
  for (int i = 0; i < windowHeight; i++) {
    for (int j = 0; j < windowWidth; j++) {
      field1[i][j] = field2[i][j];
    }
  }
}

GameInfo_t *getCurrentState(GameInfo_t *gameinfo_) {
  static GameInfo_t gameinfo;
  if (gameinfo_ != NULL) {
    gameinfo.field = gameinfo_->field;
    copyField(gameinfo.field, gameinfo_->field);
    gameinfo.next = gameinfo_->next;
    gameinfo.score = gameinfo_->score;
    gameinfo.high_score = gameinfo_->high_score;
    gameinfo.level = gameinfo_->level;
    gameinfo.speed = gameinfo_->speed;
    gameinfo.pause = gameinfo_->pause;
  }
  return &gameinfo;
}

GameInfo_t updateCurrentState() { return *getCurrentState(NULL); }

void doActions(TetGame *tgame) {
  if (tgame->info->pause != Pause) {
    TetFigure *figure = NULL;
    switch (tgame->player->action) {
      case Right:
        moveFigureRight(tgame);
        if (collisionTet(tgame)) moveFigureLeft(tgame);
        break;
      case Left:
        moveFigureLeft(tgame);
        if (collisionTet(tgame)) moveFigureRight(tgame);
        break;
      case Down:
        moveFigureEnd(tgame);
        if (collisionTet(tgame)) moveFigureUp(tgame);
        break;
      case Action:
        figure = rotateFigure(tgame);
        TetFigure *oldFigure = tgame->figure;
        tgame->figure = figure;
        if (collisionTet(tgame)) {
          tgame->figure = oldFigure;
          freeFigure(figure);
        } else {
          freeFigure(oldFigure);
        }
        break;
      case Terminate:
        tgame->playing_state = TET_GAMEOVER;
        break;
      default:
        break;
    }
  }
}

void userInput(UserAction_t action, bool hold) {
  GameInfo_t gameinfo = updateCurrentState();
  bool hold_ = hold;
  if (hold_) {
    gameinfo.pause = action;
  }
  if (gameinfo.field == NULL) {
    freeGameInfo(&gameinfo);
  }
}

void printScoreLevel(int score, int high_score, int level) {
  attron(COLOR_PAIR(1));
  mvprintw(5, 10, "Score: %d", score);
  mvprintw(6, 10, "Highest score: %d", high_score);
  mvprintw(7, 10, "Level: %d", level);
  attroff(COLOR_PAIR(1));
}

void printTetGame(TetGame *tgame) {
  int **field = tgame->info->field;
  TetFigure *t = tgame->figure;
  for (int i = 0; i < windowHeight; i++) {
    for (int j = 0; j < windowWidth; j++) {
      int sym = 1;
      if (field[i][j]) {
        sym = 2;
      } else {
        int x = j - t->x;
        int y = i - t->y;
        if (x >= 0 && x < t->size && y >= 0 && y < t->size)
          if (t->block[y][x]) sym = 2;
      }
      attron(COLOR_PAIR(sym));
      mvaddch(i, j, ' ');
      attroff(COLOR_PAIR(sym));
      printNextFigure(tgame);
      printScoreLevel(tgame->info->score, tgame->info->high_score,
                      tgame->info->level);
    }
  }
  fflush(stdout);
}
#endif