#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "display.h"
#include "map_generator.h"


int main(int argc, char *argv[]) {
	
	clear();
	char path[] = "./boards/board01.txt";
	loadBoard(path);
	displayBoard(loadBoard(path));


	
	return 0;
}
