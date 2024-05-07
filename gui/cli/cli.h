/************************************************************
 * @file cli.h
 * @brief CLI GUI header
 ************************************************************/

#ifndef BRICKGAME1_CLI_H
#define BRICKGAME1_CLI_H

/**
 * \brief Initializes the ncurses library for the game interface
 */
void initCursesFunc();

/**
 * \brief Reads the high score from a file
 * @return High score value
 */
int scoreFromFile();

/**
 * \brief Main game loop that handles game logic
 * @param sleep_time Time to sleep between game iterations
 * @param tet_template Array containing tetromino templates
 */
void gameLoop(int sleep_time, int tet_template[][4][4]);

/**
 * \brief Main function of the program
 * @return Exit status of the program
 */
int main();

#endif  // BRICKGAME1_CLI_H
