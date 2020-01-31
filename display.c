#include <stdio.h>

#include "display.h"
#include "board.h"

//display board to terminal
void displayBoard(board *b){
	clear();//clear
	resetStyle();
	int o,x,y;
	char col = 'A';
	//print row and col names
	for (int i=0; i<b->x_max; i++){
		gotoxy(2, generate_cord(i)+1);
		printf("%c", col);
		col++;
		if(col==91){
			col=97;
		}
	}
	for (int i=0; i<b->y_max; i++){
		gotoxy(generate_cord(i)+1, 2);
		printf("%d", i);
	}
	
	for(int i=0; i<b->y_max; i++){
		//print fish tile
		for(int j=0; j<b->x_max; j++){
			if((o=b->map[j][i])==0){
				continue;
			}
			y= generate_cord(i);
			x= generate_cord(j);
			//draw tille to terminalwindow
			gotoxy(y, x);
			
			printTile(x,y,o,7,2);
			
		}
	}
	//print penguins
	penguin_node * current = b->penguins;
	while (current != NULL) {
		if (current->penguin.x==-1){
			break;
		} 
		x = generate_cord(current->penguin.x);
		y = generate_cord(current->penguin.y);
		o = current->penguin.id;
		printTile(x,y,o,1,3);
		current = current->next;
	}
	resetStyle();
	//display score board
	gotoxy(1, (b->x_max+1)*3+5);
	printf("Number");
	gotoxy(1, (b->x_max+1)*3+20);
	printf("Name");
	gotoxy(1, (b->x_max+1)*3+35);
	printf("Points");
	for(int i=0;i<b->numOfPlayers;i++){
		gotoxy(i+2, (b->x_max+1)*3+5);
		printf("%d",b->listOfPlayers[i].number);
		gotoxy(i+2,(b->x_max+1)*3+20);
		printf("%s",b->listOfPlayers[i].name);
		gotoxy(i+2, (b->x_max+1)*3+35);
		printf("%d",b->listOfPlayers[i].points);
	}
	//returncart to bttom
	gotoxy(generate_cord(b->y_max+1), 0);
};

//generate coordinats on wich to print tile in col ro row
int generate_cord(int x){
	return x*2+4;
};

//--------------------------------------------------------------------------------------------------------
//Windows specyfic functions
//--------------------------------------------------------------------------------------------------------
#if defined(_WIN32) || defined(_WIN64)
#warning WIN
#include <windows.h>
//moves console cursor to y x;
void gotoxy(int y, int x) { 
	COORD pos = {x, y};
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
};

//change print color to color x
void changeColor(int x){
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	int k;
	switch (x) {
		case 0:
			k=0;
			break;
		case 1:
			k=4;
			break;
		case 2:
			k=10;
			break;
		case 3:
			k=6;
			break;
		case 4:
			k=3;
			break;
		case 5:
			k=5;
			break;
		case 6:
			k=11;
			break;
		case 7:
			k=15;
			break;
	}
	SetConsoleTextAttribute(output, k);
};

//resets color and style of printing
void resetStyle(){
	changeColor(7);
};

//print tile on pos x,y with value 0
void printTile(int x,int y,int o, int border_color, int val_color){
	changeColor(border_color);
	gotoxy(y, x);
	printf("%c%c%c",219,219,219);
	gotoxy(y+1, x);
	printf("%c %c",219,219);
	gotoxy(y+2, x);
	printf("%c%c%c",219,219,219);
	changeColor(val_color);
	gotoxy(y+1, x+1);
	printf("%d",o);
}

//clear terminal wingow
void clear(){
	system("cls");
}
//--------------------------------------------------------------------------------------------------------
//Unix specyfic functions
//--------------------------------------------------------------------------------------------------------
#endif
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#warning UNIX

//moves console cursor to y x;
void gotoxy(int y, int x) { 
	printf("\033[%d;%dH",y,x);
};


//change print color to color x
void changeColor(int x){
	switch (x) {
		case 0:
			printf("\x1b[90m");
			break;
		case 1:
			printf("\x1b[91m");
			break;
		case 2:
			printf("\x1b[92m");
			break;
		case 3:
			printf("\x1b[93m");
			break;
		case 4:
			printf("\x1b[94m");
			break;
		case 5:
			printf("\x1b[95m");
			break;
		case 6:
			printf("\x1b[96m");
			break;
		case 7:
			printf("\x1b[97m");
			break;
	}
};

//resets color and style of printing
void resetStyle(){
	printf("\x1b[0m");
};

//print tile on pos x,y with value 0
void printTile(int x,int y,int o, int border_color, int val_color){
	changeColor(border_color);
	gotoxy(y, x);
	printf("\u2588\u2588\u2588");
	gotoxy(y+1, x);
	printf("\u2588 \u2588");
	gotoxy(y+2, x);
	printf("\u2588\u2588\u2588");
	changeColor(val_color);
	gotoxy(y+1, x+1);
	printf("%d",o);
};

//clear terminal wingow
void clear(){
	printf("\033[H\033[J");
}
#endif 