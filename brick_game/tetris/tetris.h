/************************************************************
 * @file tetris.h
 * @brief Game logic library header
 ************************************************************/

#ifndef TETRIS_H
#define TETRIS_H
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#define TET_TICKS 30 /*@brief количество тиков цикла на одно движение */
#define windowWidth 10
#define windowHeight 20

typedef enum {
  Start = 0,
  Pause = 1,
  Terminate = 2,
  Left = 3,
  Right = 4,
  Up = 5,
  Down = 6,
  Action = 7
} UserAction_t;

/** @brief Figure structure */
typedef struct TetFigure {
  int x;
  int y;
  int size;
  int **block;
} TetFigure;

typedef struct TetFiguresType {
  int count;
  int size;
  int ***blocks;
} TetFiguresType;

typedef struct TetPlayer {
  int action;
} TetPlayer;

enum { TET_GAMEOVER = 0, TET_PLAYING };

typedef struct GameInfo_t {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct TetGame {
  TetFigure *figure;
  TetFiguresType *figuresT;
  TetPlayer *player;
  int ticks;
  int ticks_left;
  int playing_state;
  TetFigure *nextfigure;
  GameInfo_t *info;
} TetGame;

/**
 * \brief Creates GameInfo object
 * malloc memory to its fields
 * @return GameInfo_t*
 */

GameInfo_t *createGameInfo();

/**
 * \brief Frees GameInfo object and its fields
 * @param gameinfo
 */

void freeGameInfo(GameInfo_t *gameinfo);

/**
 * \brief Registers user input
 * @param action Actual action
 * @param hold Is key being held
 */
void userInput(UserAction_t action, bool hold);

/**
 * \brief Updates the current state of the game
 * @return Information about the game
 */
GameInfo_t updateCurrentState();

/**
 * \brief Gets the current state of the game
 * @param gameinfo_ Pointer to game information
 * @return Pointer to the current state of the game
 */
GameInfo_t *getCurrentState(GameInfo_t *gameinfo_);

/**
 * \brief Calculates positions and status of the game by a tick
 * @param tetgame Pointer to the game
 */
void calculateTact(TetGame *tetgame);

/**
 * \brief Creates figure types so you can adjust figures types
 * @param count Number of figures
 * @param figures_size Size of figures
 * @param figures_template Template for every figures
 * @return Pointer to the figure types
 */
TetFiguresType *createFiguresType(int count, int figures_size,
                                  int figures_template[][4][4]);

/**
 * \brief Frees memory allocated for figure types
 * @param tfigt Pointer to figure types
 */
void freeFiguresType(TetFiguresType *tfigt);

/**
 * \brief Creates TetGame object and allocates memory for it
 * @param figure_size Size of the figure
 * @param count Number of figures
 * @param figures_template Template for figures
 * @return Pointer to the game
 */
TetGame *createGame(int figure_size, int count, int figures_template[][4][4]);

/**
 * \brief Frees memory allocated for the game
 * @param tgame Pointer to the game
 */
void freeGame(TetGame *tgame);

/**
 * \brief Moves the figure down
 * @param tgame Pointer to the game
 */
void moveFigureDown(TetGame *tgame);

/**
 * \brief Moves the figure up
 * @param tgame Pointer to the game
 */
void moveFigureUp(TetGame *tgame);

/**
 * \brief Moves the figure right
 * @param tgame Pointer to the game
 */
void moveFigureRight(TetGame *tgame);

/**
 * \brief Moves the figure left
 * @param tgame Pointer to the game
 */
void moveFigureLeft(TetGame *tgame);

/**
 * \brief Moves the figure to the floor
 * @param tgame
 */
void moveFigureEnd(TetGame *tgame);

/**
 * \brief Checks collisions of figures with another figures and sides of field
 * @param tgame Pointer to the game
 * @return Integer collision code
 */
int collisionTet(TetGame *tgame);

/**
 * \brief Places the current figure in the field matrix
 * @param tgame Pointer to the game
 */
void placeFigure(TetGame *tgame);

/**
 * \brief Checks if a line is filled
 * @param line line index
 * @param field 2D array representing the game field
 * @return 1 if line is filled, 0 otherwise
 */
int lineFilled(int line, int **field);

/**
 * \brief Drops a filled line
 * @param line line index
 * @param field 2D array representing the game field
 */
void dropLine(int line, int **field);

/**
 * \brief Erases filled lines in the game field
 * @param tgame Pointer to the game
 * @return Number of lines erased
 */
int eraseLines(TetGame *tgame);

/**
 * \brief Alloc memory for figure for the game
 * @param tgame Pointer to the game
 * @return Pointer to the created figure
 */
TetFigure *createFigure(TetGame *tgame);

/**
 * \brief Frees memory allocated for a figure
 * @param figure Pointer to the figure to free
 */
void freeFigure(TetFigure *figure);

/**
 * \brief Drop new randow figure to the start position
 * @param tgame Pointer to the game
 */
void dropNewFigure(TetGame *tgame);

/**
 * \brief Creates the next figure for the game
 * @param tgame Pointer to the game
 */
void createNextFigure(TetGame *tgame);

/**
 * \brief Rotates the current figure in the game
 * @param tgame Pointer to the game
 * @return Pointer to the rotated figure
 */
TetFigure *rotateFigure(TetGame *tgame);

/**
 * \brief Prints the score, high score and level of the game
 * @param score Current score
 * @param high_score High score
 * @param level Current level
 */
void printScoreLevel(int score, int high_score, int level);

/**
 * \brief Prints the game field to the console
 * @param tgame Pointer to the game
 */
void printTetGame(TetGame *tgame);

/**
 * \brief Performs game actions based on user input
 * @param tgame Pointer to the game
 */
void doActions(TetGame *tgame);

void deleteField(int **field);

int **createField();

#endif