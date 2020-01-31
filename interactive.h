#include "board.h"
#ifndef _interactive_h_
#define _interactive_h_

#define clearbuf() while(getchar()!='\n')


//structure to keep results of the game
typedef struct result {
	int max_points;
	char name[128];
}result;

int gameInProgres;//flag

void interactiveMain();
//signal handler
void endGame(int);
void displayFile(char *);
void selectPlayerNames(int, board *);
void penguinPacemetPhase(int, board *);
void penguinMovementPhase(board *);
void printBoardList();

int selectNumOfPlayers();
int selectNumOfPnguins();
int getColNumber(char);

board* loadBoardById(int);

board* selectBoard(int,int);


result calculateResult(board *);
#endif