#include "s21_tetris_test.h"

int tet_template[][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}},
};

START_TEST(test_create_game) {
  TetGame *game = createGame(4, 7, tet_template);
  ck_assert_ptr_ne(game, NULL);
  ck_assert_int_eq(game->ticks_left, 30);
  ck_assert_int_eq(game->ticks, 30);
  freeFiguresType(game->figuresT);
  free(game);
}
END_TEST
START_TEST(test_create_figure) {
  TetGame *game = createGame(4, 7, tet_template);
  game->figure = createFigure(game);
  ck_assert_ptr_ne(game->figure, NULL);
  ck_assert_int_eq(game->figure->x, 0);
  ck_assert_int_eq(game->figure->y, 0);
  ck_assert_int_eq(game->figure->size, 4);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_ne(game->figure->block[i], NULL);
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(game->figure->block[i][j], 0);
    }
  }
  freeFigure(game->figure);
  freeFiguresType(game->figuresT);
  free(game);
}
END_TEST

START_TEST(test_create_figures_type) {
  TetFiguresType *figure_type = createFiguresType(2, 4, tet_template);
  ck_assert_ptr_ne(figure_type, NULL);
  ck_assert_int_eq(figure_type->count, 2);
  ck_assert_int_eq(figure_type->size, 4);
  for (int i = 0; i < 2; i++) {
    ck_assert_ptr_ne(figure_type->blocks[i], NULL);
    for (int j = 0; j < 4; j++) {
      ck_assert_ptr_ne(figure_type->blocks[i][j], NULL);
      for (int k = 0; k < 4; k++) {
        ck_assert_int_eq(figure_type->blocks[i][j][k], tet_template[i][j][k]);
      }
    }
  }
  freeFiguresType(figure_type);
}
END_TEST

START_TEST(test_create_gameinfo) {
  GameInfo_t *info = createGameInfo();
  ck_assert_ptr_ne(info, NULL);
  ck_assert_int_eq(info->field[0][0], 0);
  ck_assert_int_eq(info->score, 0);
  ck_assert_int_eq(info->high_score, 0);
  ck_assert_int_eq(info->level, 1);
  ck_assert_int_eq(info->speed, 0);
  ck_assert_int_eq(info->pause, Pause);
  freeGameInfo(info);
}
END_TEST

Suite *s21_create(void) {
  Suite *s = suite_create("create_test");
  TCase *tc_core = tcase_create("create_test");
  tcase_add_test(tc_core, test_create_figure);
  tcase_add_test(tc_core, test_create_figures_type);
  tcase_add_test(tc_core, test_create_gameinfo);
  tcase_add_test(tc_core, test_create_game);
  suite_add_tcase(s, tc_core);
  return s;
}