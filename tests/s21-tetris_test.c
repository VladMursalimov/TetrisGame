#include "s21_tetris_test.h"

int run_suite(Suite *suite) {
  int failed;
  SRunner *sr = srunner_create(suite);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);

  srunner_free(sr);
  return failed;
}

int main() {
  Suite *suites[] = {
      s21_create(),
      s21_line(),
  };
  int failed = 0, length = sizeof(suites) / sizeof(suites[0]);

  for (int i = 0; i < length; i++) {
    fprintf(stdout, "RUN TEST: %d\n", i + 1);
    failed += run_suite(suites[i]);
  }

  return failed != 0;
}