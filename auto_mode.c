#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "auto_mode.h"
#include "interactive.h"
#include "board.h"
#include "display.h"
#include "movement.h"
#include "placement.h"

void placement(int penguins, char* input, char* output){
	board* b = loadBoard(input);
	int playerId = playerNameToNum(PLAYER_NAME, b);
	if(playerId==-1){
		for(playerId=1;isPlayerNumTaken(playerId, b);playerId++);
		addPlayer(PLAYER_NAME, playerId, b);
	}
	// displayBoard(b);
	placementCords move = getBestPlacement(playerId, penguins ,b);//{13,9};
	// getchar();
	placePenguin(move.x, move.y, playerId, b);
	// displayBoard(b);
	saveBoard(b, output);
}

void movement(char* input, char* output){
	board* b = loadBoard(input);
	int playerId = playerNameToNum(PLAYER_NAME, b);
	// displayBoard(b);
	moveCords move = getBestMove(playerId, b);
	// getchar();
	safeMoveC(move, b);
	// displayBoard(b);
	saveBoard(b, output);
}

void handle_automode_args(int argc, char* argv[]){
	if(argc < 2)
		usage(argv[0]);
	if(!strcmp(argv[1], "id")){
		printf("%s\n", PLAYER_NAME);
	} else if(strcmp(argv[1], "phase=placement")==0){
		//check if 2nd argument starts with 'penguins='
		//9 is the length of the string 'penguins='
		if(argc != 5 || strncmp(argv[2], "penguins=", 9) != 0){
			usage(argv[0]);
		}
		//parse second argument as an integer, starting from the 10th character.
		int penguins = atoi(argv[2]+9); //TODO: more checks
		char* input = argv[3];
		char* output = argv[4];
		placement(penguins, input, output);
	} else if(strcmp(argv[1], "phase=movement")==0){
		if(argc != 4)
			usage(argv[0]);
		char* input = argv[2];
		char* output = argv[3];
		movement(input, output);
	} else {
		usage(argv[0]);
	}
}


// Print the usage message and quit with error code.
//name - name of the program, obtained from argv[0].
void usage(char* name){
	printf("Usage:\n");
	printf("  %s phase=placement penguins=<N> <inputboardfile> <outputboardfile>\n", name);
	printf("  %s phase=movement <inputboardfile> <outputboardfile>\n", name);
	printf("  %s id\n", name);
	exit(3);
}