#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include<unistd.h>

#include "board.h"
#include "display.h"
#include "map_generator.h"

int main(int argc, char *argv[]) {
	int x,y;
	board *b;
	printf("x: y:\n");
	scanf("%d %d",&x,&y);
	for(int i=0;;i++){
		b = generateBoard2(x, y);
		displayBoard(b);
		printf("%d\n",i);
		getchar();
	}
}