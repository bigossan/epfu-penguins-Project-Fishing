#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "map_generator.h"

//flood fill map with fish
void ffill(int px,int py, board *b){
	b->map[px][py]=1+(rand()%2);
	if (px+1<b->x_max && b->map[px+1][py]==0) {
		ffill(px+1, py, b);
	}
	if (px-1>=0 && b->map[px-1][py]==0) {
		ffill(px-1, py, b);
	}
	if (py+1<b->y_max && b->map[px][py+1]==0) {
		ffill(px, py+1, b);
	}
	if (py-1>=0 && b->map[px][py-1]==0) {
		ffill(px, py-1, b);
	}
}

//2nd board generator based on simplex noise
board *generateBoard2(int x, int y){
	board *b;
	b= malloc(sizeof(board));
	iniciate(b);
	b->x_max = x;
	b->y_max = y;
	alocateMemForMap(b);
	double sx, sy, n, r, o;
	int *perm = noiseStart();
	int mx,my;
	o = 0.125;
	r = sqrt(x*x+y*y)/1.8;
	for (int i=0; i<y;i++){
		for (int j=0; j<x;j++){
			b->map[j][i]=0;
		}
	}
	seedRandom();
	for(double a = 0; a<7; a+=0.001){
		sy = sin(a);
		sx = cos(a);
		n = (noise(sx, sy, perm)*(1-o))+o;
		mx = fastfloor(sx*r*n+x/2);
		my = fastfloor(sy*r*n+y/2);
		if (!(mx<x))mx=x-1;
		if (!(mx>0))mx=0;
		if (!(my<y))my=y-1;
		if (!(my>0))my=0;
		b->map[mx][my]=1+(rand()%2);
		
	}
	ffill(x/2, y/2, b);
	return b;
}

//generates seed for simplex noise
int* noiseStart(){
	int *perm = malloc(512*sizeof(int));
	seedRandom();
	for(int i=0; i<256; i++){
		perm[i] = (perm[i+256] = rand() % 255);
	}
	return perm;
};

//fast floor
int fastfloor(double x) {
	return x>0 ? (int)x : (int)x-1;
}

//dot product of g and (x,y)
double dot(int g[], double x, double y){
	return g[0]*x + g[1]*y;
}

//calculates simplex noise at xin yin
//based on http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
double noise(double xin, double yin, int *perm){
	
	static int grad2[][2] = {{1,1},{-1,1},{1,-1},{-1,-1},{1,0},{-1,0},{1,0},{-1,0},{0,1},{0,-1},{0,1},{0,-1}};
	static int simplex[][4] = {
	{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0}, {0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0}, {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}, {1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0}, {1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0}, {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}, {2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0}, {2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}};

	double n0, n1, n2; // Noise contributions from the three corners
	// Skew the input space to determine which simplex cell we're in
	double F2 = 0.5*(sqrt(3.0)-1.0);
	double s = (xin+yin)*F2; // Hairy factor for 2D
	int i = fastfloor(xin+s);
	int j = fastfloor(yin+s);
	
	double G2 = (3.0-sqrt(3.0))/6.0;
	double t = (i+j)*G2;
	double X0 = i-t; // Unskew the cell origin back to (x,y) space
	double Y0 = j-t;
	double x0 = xin-X0; // The x,y distances from the cell origin
	double y0 = yin-Y0;
	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
	if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
	else {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)
	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6
	double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	double y1 = y0 - j1 + G2;
	double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
	double y2 = y0 - 1.0 + 2.0 * G2;
	// Work out the hashed gradient indices of the three simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int gi0 = perm[ii+perm[jj]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1]] % 12;
	int gi2 = perm[ii+1+perm[jj+1]] % 12;
	// Calculate the contribution from the three corners
	double t0 = 0.5 - x0*x0-y0*y0;
	if(t0<0) n0 = 0.0;
	else {
		t0 *= t0;
		n0 = t0 * t0 * dot(grad2[gi0], x0, y0); // (x,y) of grad2 used for 2D gradient
	}
	double t1 = 0.5 - x1*x1-y1*y1; 
	if(t1<0) n1 = 0.0;
	else {
		t1 *= t1;
		n1 = t1 * t1 * dot(grad2[gi1], x1, y1);
	}
	double t2 = 0.5 - x2*x2-y2*y2;
	if(t2<0) n2 = 0.0;
	else {
		t2 *= t2;
		n2 = t2 * t2 * dot(grad2[gi2], x2, y2);
	}
	// Add contributions from each corner to get the  noise value.
	// The result is scaled to return values in the interval [0,1].
	return (70.0 * (n0 + n1 + n2)+1.0)/2.0;
}

//seeds rand with time 
void seedRandom(){
	static int iniciate = 0;
	if (!iniciate) {
		srand((unsigned) time(NULL));
		iniciate = 1;
	}
}

//generates random board with given dimenctions x, y, nmber of players, number of penguins per player
board *generateBoard(int x, int y, int players, int penguins){
	board *b;
	b= malloc(sizeof(board));
	iniciate(b);
	b->x_max = x;
	b->y_max = y;
	alocateMemForMap(b);
	seedRandom();
	for(int i=0; i<b->y_max; i++){
		for(int j=0; j<b->x_max; j++){
			b->map[j][i] = rand() % 4;
		}
	}
	for(int i = 0; i < players*penguins; i++){
		b->map[rand() % b->x_max][rand() % b->y_max] = 1;
	}
	return b;
};
