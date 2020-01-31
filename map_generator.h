#include "board.h"

#ifndef _map_generator_h_
#define _map_generator_h_

int* noiseStart();
int fastfloor(double);

double dot(int *, double, double);
double noise(double xin, double yin, int*);

board *generateBoard2(int, int);
board *generateBoard(int, int, int, int);

void ffill(int, int, board*);
void seedRandom();

#endif