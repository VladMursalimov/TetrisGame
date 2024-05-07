OS = $(shell uname)
CC = gcc

CFLAGS := -Wall -Wextra -Werror -std=c11 -Wpedantic
SOURCE_TESTS_FUNC = $(wildcard ./tests/*.c)
det_OS = -lcheck -lsubunit -lm -pthread -lrt -lncurses

build: all
rebuild:clean all
all:install play

GUI := gui/cli/cli.c
TETRIS:= brick_game/tetris/tetris.c
BUILD_DIR := build
DOCS_DIR := docs
TESTS_DIR := tests
REPORT_DIR := report
DIST_DIR := dist
TEST_DIR := test

play:
	echo ${det_OS}
	cd ${BUILD_DIR} && ./tetris

install:build_dir tetris

uninstall: clean
	rm -rf ${BUILD_DIR}

cli.o:
	gcc ${CFLAGS} -c -lncurses ${GUI} $(det_OS) -o cli.o

tetris:clean tetris.a cli.o
	gcc ${CFLAGS} cli.o main.c tetris.a -o ${BUILD_DIR}/tetris -lncurses $(det_OS)

tetris.o:
	gcc ${CFLAGS} -c -lncurses ${TETRIS} $(det_OS) -o tetris.o

tetris.a:tetris.o
	ar rc tetris.a tetris.o
	ranlib tetris.a

test: clean test_dir tetris.a
	@$(CC) $(SOURCE_TESTS_FUNC) tetris.a -o $(TEST_DIR)/s21_test_tetris $(det_OS)
	$(TEST_DIR)/s21_test_tetris

testv: clean test_dir tetris.a
	@$(CC) $(SOURCE_TESTS_FUNC) tetris.a -o $(TEST_DIR)/s21_test_tetris $(det_OS)
	valgrind --tool=memcheck --leak-check=full $(TEST_DIR)/s21_test_tetris

test_coverage: test_dir
	@$(CC) $(TETRIS) $(SOURCE_TESTS_FUNC) --coverage tetris.a -o $(TEST_DIR)/s21_test_tetris $(det_OS)
	$(TEST_DIR)/s21_test_tetris

gcov_report: clean tetris.a test_coverage
	mkdir report
	lcov -t "s21_test_tetris" -o test.info -c -d .
	genhtml -o report/ test.info
	google-chrome report/index.html

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info dist/ docs/ report/ test/

valgrind: install
	cd ${BUILD_DIR}/ && valgrind --tool=memcheck --leak-check=full ./tetris

clang:
	clang-format -i */*/*.c */*/*.h tests/*.c

dist: dist_dir
	tar cf $(DIST_DIR)/tetris-1.0.tar $(BUILD_DIR) $(DOCS_DIR)

dvi:
	rm -rf doxygen
	doxygen
	google-chrome docs/html/index.html

build_dir:
	mkdir -p $(BUILD_DIR)

dist_dir:
	mkdir -p $(DIST_DIR)

test_dir:
	mkdir -p $(TEST_DIR)