#include <stdio.h>
#include "board.h"
#include "display.h"
#include "map_generator.h"



int main(int argc, char *argv[]) {
	clear();
	board *b = loadBoard("./boards/board_demo_input.txt");
	board *c = generateBoard(19,19,4,1);
	board *d = generateBoard2(25,25);
	
	displayBoard(c);
	getchar();
	
	displayBoard(d);
	getchar();
	
	displayBoard(b);
	getchar();
	
	safeMove(12,3,12,6,b);//imposible move
	displayBoard(b);
	getchar();
	
	safeMove(10,1,10,6,b);//posible move
	displayBoard(b);
	getchar();
	
	saveBoard(b,"./boards/board_demo_output.txt");
	
	b = loadBoard("./boards/board_demo_output.txt");
	displayBoard(b);
	getchar();
	
	return 0;
}