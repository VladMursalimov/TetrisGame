#include "s21_tetris_test.h"

START_TEST(test_drop_line) {
  int tet_template[][4][4] = {
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}},
  };
  TetGame *game = createGame(4, 7, tet_template);
  game->figure = createFigure(game);
  game->info = createGameInfo();
  dropLine(1, game->info->field);
  ck_assert_int_eq(lineFilled(1, game->info->field), 0);
  dropLine(2, game->info->field);
  ck_assert_int_eq(lineFilled(2, game->info->field), 0);
  dropLine(3, game->info->field);
  ck_assert_int_eq(lineFilled(3, game->info->field), 0);
  dropLine(4, game->info->field);
  ck_assert_int_eq(lineFilled(4, game->info->field), 0);
  dropLine(5, game->info->field);
  ck_assert_int_eq(lineFilled(5, game->info->field), 0);
  dropLine(6, game->info->field);
  ck_assert_int_eq(lineFilled(6, game->info->field), 0);
  dropLine(7, game->info->field);
  ck_assert_int_eq(lineFilled(7, game->info->field), 0);
  ck_assert_ptr_nonnull(game);

  freeFiguresType(game->figuresT);
  freeFigure(game->figure);
  freeGameInfo(game->info);
  free(game);
}
END_TEST

START_TEST(test_collision) {
  int tet_template[][4][4] = {
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}},
  };
  TetGame *game = createGame(4, 7, tet_template);
  game->info = createGameInfo();
  createNextFigure(game);
  dropNewFigure(game);
  ck_assert_int_eq(collisionTet(game), 0);
  ck_assert_ptr_nonnull(game);

  freeFigure(game->figure);
  freeFigure(game->nextfigure);

  freeGameInfo(game->info);
  freeFiguresType(game->figuresT);
  free(game);
}
END_TEST

START_TEST(test_calculate_tact) {
  int tet_template[][4][4] = {
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
  };
  TetGame *game = createGame(4, 7, tet_template);
  game->info = createGameInfo();
  game->ticks_left = 0;
  createNextFigure(game);
  dropNewFigure(game);
  calculateTact(game);
  ck_assert_int_eq(game->figure->y, 0);
  game->figure->y = 17;
  game->ticks_left = 0;
  calculateTact(game);
  ck_assert_int_eq(game->figure->y, -1);
  moveFigureDown(game);
  ck_assert_int_eq(game->figure->y, 0);
  moveFigureLeft(game);
  ck_assert_int_eq(game->figure->x, 2);
  moveFigureRight(game);
  ck_assert_int_eq(game->figure->x, 3);
  moveFigureUp(game);
  ck_assert_int_eq(game->figure->y, -1);
  ck_assert_ptr_nonnull(game);

  freeFigure(game->figure);
  freeFigure(game->nextfigure);
  freeGameInfo(game->info);
  freeFiguresType(game->figuresT);
  free(game);
}
END_TEST

START_TEST(test_print) {
  int tet_template[][4][4] = {
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
  };
  TetGame *game = createGame(4, 7, tet_template);
  GameInfo_t *gameInfo = createGameInfo();
  game->info = getCurrentState(gameInfo);
  game->ticks_left = 0;
  createNextFigure(game);
  dropNewFigure(game);
  createNextFigure(game);

  printTetGame(game);
  ck_assert_ptr_nonnull(game);
  freeGame(game);
  freeGameInfo(gameInfo);
}
END_TEST

START_TEST(test_actions) {
  int tet_template[][4][4] = {
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
  };
  TetGame *game = createGame(4, 7, tet_template);
  game->info = createGameInfo();
  game->info->pause = Start;
  game->ticks_left = 0;
  TetPlayer *player = (TetPlayer *)malloc(sizeof(TetPlayer));
  game->player = player;
  game->player->action = Left;
  createNextFigure(game);
  dropNewFigure(game);
  calculateTact(game);
  ck_assert_int_eq(game->figure->x, 2);
  game->player->action = Right;
  calculateTact(game);
  ck_assert_int_eq(game->figure->x, 3);

  game->player->action = Action;
  calculateTact(game);
  ck_assert_int_eq(game->figure->y, 0);

  game->player->action = Down;
  calculateTact(game);
  ck_assert_int_eq(game->figure->y, 17);

  free(player);
  freeFigure(game->figure);
  freeFigure(game->nextfigure);
  freeFiguresType(game->figuresT);
  freeGameInfo(game->info);
  free(game);
}
END_TEST

Suite *s21_line() {
  Suite *s = suite_create("line_test");
  TCase *tc_core = tcase_create("line_test");

  tcase_add_test(tc_core, test_drop_line);
  tcase_add_test(tc_core, test_collision);
  tcase_add_test(tc_core, test_calculate_tact);
  tcase_add_test(tc_core, test_print);
  tcase_add_test(tc_core, test_actions);
  suite_add_tcase(s, tc_core);
  return s;
}