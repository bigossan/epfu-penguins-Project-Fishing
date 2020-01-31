#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "interactive.h"
#include "board.h"
#include "display.h"
#include "map_generator.h"


//main function of interactive mode calls other functions in proper order to play the game
void interactiveMain(){
	clear();
		// vars
		board *b;
		int players, penguins;
		result gameResult;
		
		//display hllo meseage
		displayFile("MSG/EPFU19Z.txt");
		printf("\n\n\n");
		
		//select num of players
		players = selectNumOfPlayers();
		printf("Nuber of players = %d\n\n",players);
		
		//select num of penguins
		penguins = selectNumOfPnguins();
		printf("Nuber of penguins = %d\n\n",penguins);
		
		//ask if should generate board
		b = selectBoard(players,penguins); 
		
		//select player mames
		selectPlayerNames(players, b);
		
		displayBoard(b);
		//penguin pacemet
		penguinPacemetPhase(penguins,b);
		
		//set CTRL-C to exit game and display score
		gameInProgres = 1;
		signal(SIGINT, endGame);
		
		//main game loop
		penguinMovementPhase(b);
		
		//calculete best score 
		gameResult = calculateResult(b);
		clear();
		
		//displays best score
		printf("%s won with %d points\n", gameResult.name, gameResult.max_points);
		
		displayFile("MSG/Congratulations.txt");

}


//signal handler to stop the game after round is finished
void endGame(int signum) {
	gameInProgres = 0;
	printf("\nGame will end after this ruound\n");
	printf("To force game end press CTRL-C\n");
	signal(SIGINT, SIG_DFL);//back to default handling
}

//print txt files to terminal window
void displayFile(char *fileName){
	FILE *file;
	char c;
	file = fopen(fileName, "r");
	if (file) {
		while ((c = getc(file)) != EOF){
			putchar(c);
		}
	fclose(file);
	}
}

//asks usr for numper of players
int selectNumOfPlayers(){
	int players = 0;
	while(players < 1 || players > 9){
		printf("Nuber of players?\n");
		scanf("%d",&players);
		clearbuf();
	}
	return players;
}

//ask usr for number of penguins per player
int selectNumOfPnguins(){
	int penguins = 0;
	while(penguins < 1){
		printf("Nuber of penguins?\n");
		scanf("%d",&penguins);
		clearbuf();
	}
	return penguins;
}

//asks usr to select beetwin generating and loading from file, then perform action acording o usr choise, and returns board
board *selectBoard(int players, int penguins){
	board *b;
	int  generate=-1, size_x=0, size_y=0, boardid;
	while(generate != 0 && generate != 1){
		printf("generate random board?\n1-YES\t0-NO\n");
		scanf("%d",&generate);
		clearbuf();
	}
	if(!generate){
		//loads board from file
		printBoardList();
		printf("Chose board\n");
		scanf("%d", &boardid);
		clearbuf();
		b = loadBoardById(boardid);
	}
	else{
		//generates board 
		while(size_x < 1 || size_x > 30 || size_y < 1 || size_y > 50){
			printf("Size?\nX Y\n");
			scanf("%d %d",&size_x, &size_y);
			clearbuf();
		}
		printf("X:%d Y:%d\n\n",size_x,size_y);
		
		b = generateBoard2(size_x, size_y);
		
//		b = generateBoard(size_x, size_y, players, penguins);
	}
	return b;
}

//ask usr for names of players
void selectPlayerNames(int players,board *b){
	char name[128];
	for(int i = 0;i<players;i++){
		printf("P:%d Name?\n",i+1);
		scanf("%s",name);
		clearbuf();
		addPlayer(name,i,b);
	}
}

//loop for penguin placement in turn ask ich player to place penguin on board
void penguinPacemetPhase(int penguins, board *b){
	int x,y;
	char xc;
	for(int i = 0; i < penguins; i++){
		for(int j = 0;j < b->numOfPlayers;j++){
			x=-1;
			y=-1;
			while(placePenguin(x,y,j,b)){
				printf("P:%s Plce penguin\ncol row\n",b->listOfPlayers[j].name);
				scanf("%c %d", &xc , &y);
				clearbuf();
				x = getColNumber(xc);
				if(x>b->x_max||x<0||y>b->y_max||y<0){
					x=-1;
					y=-1;
				}
			}
			displayBoard(b);
		}
	}
}

//changes char describing column to num acording to:
//A-Z -> 0-25
//a-z -> 26-51
int getColNumber(char xc){
	int x;
	if(xc>90){
		xc-=6;
	}
	x = xc - 'A';
	return x;
}

//loop for penguin movement in turn ask ich player to make move
//terminates when no moves are posible or game have bean canceld using CIRL-c
void penguinMovementPhase(board *b){
	char to_xc, from_xc;
	int from_x, from_y, to_x, to_y;
	penguin *pe;
	
	for(int turn = 0; gameInProgres==1; turn++){
		for(int i = 0; i<b->numOfPlayers;){
			//if player can move ask for move
			//else next player
			if (canPlayerMove(i,b)){
				//ask for move
				//make move if posible
				printf("T:%d\nP:%s move penguin\nfrom_x from_y to_x to_y\n",turn,b->listOfPlayers[i].name);
				scanf("%c %d %c %d", &from_xc, &from_y, &to_xc, &to_y);
				clearbuf();
				
				from_x = getColNumber(from_xc);
				to_x = getColNumber(to_xc);
				
				printf("%d %d %d %d\n", from_x, from_y, to_x, to_y);
				pe = getPenguin(from_x, from_y,b);
				if (pe!=NULL&&pe->id==i){
					if(safeMove(from_x,from_y,to_x,to_y,b)){
						i++;
					}
				}
				displayBoard(b);
			}
			else{
				i++;
			}
		}
		//check if any move posible
		if(gameInProgres == 1){
			gameInProgres = 0;
			for(int i = 0; i<b->numOfPlayers;i++){
				if(canPlayerMove(i,b)){
					gameInProgres = 1;
					break;
				}
			}
		}
	}

}

//claculates best score and returns result
result calculateResult(board *b){
	result r;
	r.max_points = 0;
	for(int i = 0 ;i<b->numOfPlayers;i++){
		if(r.max_points < b->listOfPlayers[i].points){
			r.max_points = b->listOfPlayers[i].points;
			strcpy(r.name,b->listOfPlayers[i].name);
		}
		else if (r.max_points == b->listOfPlayers[i].points) {
			strcat(r.name, " and ");
			strcat(r.name, b->listOfPlayers[i].name);
		}
	}
	return r;
}

//prints list of files in ./boards
void printBoardList(){
	DIR *dir;
	struct dirent *ent;
	int i = 0;
	if ((dir = opendir ("./boards")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			printf ("[%d] %s\n",i , ent->d_name);
			i++;
		}
		closedir (dir);
	}
	else{
		printf("\nMissing ./boards dirctory\n");
		abort();
	}
}

//choses board of pos id in ./boards 
board *loadBoardById(int id){
	DIR *dir;
	struct dirent *ent;
	char r[128]="./boards/";
	int i = 0;
	dir = opendir ("./boards");
	while ((ent = readdir (dir)) != NULL) {
		if(id==i){
			strcat(r,ent->d_name);
			break;
		}
		i++;
	}
	closedir (dir);
	return loadBoard(r);
};