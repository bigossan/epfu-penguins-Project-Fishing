#include "board.h"

#ifndef _display_h_
#define _display_h_

void gotoxy(int,int);
void displayBoard(board *);
void changeColor(int);
void resetStyle();
void printTile(int x,int y,int o,int, int);
int generate_cord(int);
void clear();

#endif
