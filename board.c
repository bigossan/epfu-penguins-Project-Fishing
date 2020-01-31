#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "board.h"


//ads penguin to list
void push_penuin(penguin_node* head,penguin p){
	penguin_node * current = head;
	if(current->penguin.x >= 0){
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = (penguin_node *)malloc(sizeof(penguin_node));
		current->next->penguin = p;
		current->next->next = NULL;	
	}
	else{
		current->penguin = p;
		current->next = NULL;
	}	
};

//iniciates board 
//alocaes memory for penguin list
void iniciate(board *b){
	b->penguins = malloc(sizeof(penguin_node));
	b->penguins->next = NULL;
	b->penguins->penguin.x = -1;
	b->penguins->penguin.y = -1;
	b->penguins->penguin.id = -1;
	b->numOfPlayers=0;
};

//function loads game board from file and returns board structure
board *loadBoard(char *path){
	char str[1024], c;
	int player_check[] = {0,0,0,0,0,0,0,0,0}, pos, end_of_name, d1, d2;
	board *b;
	b = malloc(sizeof(board));
	iniciate(b);
	penguin p;
	FILE *fp; 
	fp = fopen(path, "r");
	
	fscanf(fp, "%i %i",&b->y_max,&b->x_max);
	//load map
	while (getc(fp)!='\n');
	alocateMemForMap(b);
	for(int i=0; i<b->y_max; i++){
		if(fgets(str, 1024, fp)==NULL){
			printf("Missing line nr %d in board file\n",i);
			exit(2);
		}
		if(strlen(str)<3*b->x_max-1){
			printf("Line nr %d is to short\n",i);
			exit(2);
		}
		pos=0;
		for(int j=0; j<b->x_max; j++){
			while (str[pos]==' ') {
				pos++;
			}
			d1=atoi(&str[pos])/10;
			pos++;
			if(str[pos]==' ' || str[pos]=='\n' || str[pos]=='\0'){
				printf("Error in tile data line %d col %d\n",i,j);
				exit(2);
			}
			d2=atoi(&str[pos]);
			pos++;
			
			b->map[j][i] = d1;
			// printf("%d\n",d1);
			if(d2>0){
				player_check[d1-1]=1;
				p.x=j;
				p.y=i;
				p.id=d2;
				push_penuin(b->penguins,p);
			}
		}
		//error
		while (pos<strlen(str)) {
			if(str[pos]!=' ' && str[pos]!='\n' && str[pos]!='\0'){
				printf("Excesive tile data in line %d",i);
				exit(2);
			}
			pos++;
		}
	}
	//loading players
	b->numOfPlayers = 0;
	for(int i=0;;i++){
		if(fgets(str, 1024, fp)==NULL){
			break;
		}
		end_of_name = index_of(str, ' ');
		if(end_of_name<1){
			printf("Mising name of player%d\n",i);
			exit(2);
		}
		pos = end_of_name;
		while(str[pos]==' '){
			pos++;
		}
		if (!is_dgt(str[pos]) || str[pos+1]!=' ') {
			printf("Error in player %d num\n",i);
			exit(2);
		}
		pos++;
		while(str[pos]==' '){
			pos++;
		}
		while(is_dgt(str[pos])){
			pos++;
		}
		while(str[pos]==' '){
			pos++;
		}
		if(str[pos]!='\n'){
			printf("Excesive data in player %d\n",i);
			exit(2);
		}
		sscanf(str, "%s %d %d",b->listOfPlayers[i].name, &b->listOfPlayers[i].number, &b->listOfPlayers[i].points);
		if(player_check[b->listOfPlayers[i].number-1]==-1){
			printf("Overlaping player num");
			exit(2);
		}
		player_check[b->listOfPlayers[i].number-1] = -1;
		b->numOfPlayers = i+1;
	}
	if(b->numOfPlayers>9){
		printf("To many players\n");
		exit(2);
	}
	for (int i=0;i<9;i++) {
		if(player_check[i]==1){
			printf("Penguin without asociated player\n");
			exit(2);
		}
	}
	
	fclose(fp);
	return b;
};

//save game board to file
void saveBoard(board *b, char *path){
	FILE *fp;
	penguin_node * current = b->penguins;
	int x,y, **o;
	fp = fopen(path, "w");
	o =array2d(b->x_max,b->y_max);
	for (int i=0; i<b->y_max;i++){
		for (int j=0; j<b->x_max;j++){
			o[j][i]=0;
		}
	}
	while (current != NULL) {
		if (current->penguin.x==-1){
			break;
		}
		o[current->penguin.x][current->penguin.y]=current->penguin.id;
		current = current->next;
	}
	
	fprintf(fp, "%i %i\n",b->y_max,b->x_max);
	for(int i=0; i<b->y_max; i++){
		for(int j=0; j<b->x_max; j++){
			fprintf(fp,"%i%i ",b->map[j][i],o[j][i]);
		}
		fprintf(fp,"\n");
	}

	for(int i=0;i<b->numOfPlayers;i++){
		fprintf(fp,"%s %d %d\n",b->listOfPlayers[i].name,b->listOfPlayers[i].number,b->listOfPlayers[i].points);
	}

	fclose(fp);
	free(o);
};


//chek if tile (x,y) contains fish
int isFish(int x,int y, board *b){
	if(x>=b->x_max || y>=b->y_max || x<0 || y<0 || b->map[x][y]==0)return 0;
	return 1;
};

//chek if tile (x,y) contains player
int isPlayer(int x,int y, board *b){
	penguin_node * current = b->penguins;
	while (current != NULL) {
		if (current->penguin.x==-1){
			break;
		}
		if(current->penguin.x == x && current->penguin.y ==y){
			return 1;
		}
		current = current->next;
	}
	return 0;
};

//moves player from (from_x, ftom_y) to (t0_x, to_y) and adds points acording to number of fishes plnguin colected
void move(int from_x,int from_y,int to_x,int to_y,board *b){
	penguin *p = getPenguin(from_x,from_y,b);
	if (p!=NULL){
		int playerNumber = p->id;
		int fishNumber = b->map[to_x][to_y];
		//move to new tile
		(*b).map[to_x][to_y] = 0;
		p->x=to_x;
		p->y=to_y;
		//empty old tile
		(*b).map[from_x][from_y] = 0;
		addPoints(playerNumber, fishNumber, b);
	}
};
void moveC (moveCords c, board *b){
	move(c.from_x, c.from_y, c.to_x, c.to_y,b);
};

//adds p points to gicen player 
void addPoints(int player, int p, board *b){
	for(int i=0;i<b->numOfPlayers;i++){
		if((*b).listOfPlayers[i].number==player){
			(*b).listOfPlayers[i].points+=p;
			break;
		}
	}
};

//cheks if move from (from_x, ftom_y) to (t0_x, to_y) is posible
int isMove(int from_x,int from_y,int to_x,int to_y,board *b){
	int small , big;
	//is move from player to fish
	if(isPlayer(from_x, from_y, b) && isFish(to_x, to_y, b)){
		//are xs in one line
		if(from_x==to_x){
			small = from_y<to_y ? from_y : to_y;
			big = from_y>to_y ? from_y : to_y;
			//chek if path is free
			for (int i = small+1; i<big;i++){
				if (isFish(from_x, i, b)==0){
					return 0;
				}
			}
		}
		//are ys in one line
		else if(from_y==to_y){
			small = from_x<to_x ? from_x : to_x;
			big = from_x>to_x ? from_x : to_x;
			//chek if path is free
			for (int i = small+1; i<big;i++){
				if (isFish(i, from_y, b)==0){
					return 0;
				}
			}
		}
		else {
			return 0;
		}
		return 1;
	}
	return 0;	
};
int isMoveC(moveCords c,board *b){
	return isMove(c.from_x, c.from_y, c.to_x, c.to_y, b);
}
//move player from (from_x, ftom_y) to (t0_x, to_y) if move is posible
int safeMove(int from_x,int from_y,int to_x,int to_y,board *b){
	if (isMove(from_x, from_y, to_x, to_y, b)){
		move(from_x, from_y, to_x, to_y, b);
		return 1;
	}
	return 0;
};
int safeMoveC(moveCords c, board *b){
	return safeMove(c.from_x, c.from_y, c.to_x, c.to_y, b);
}

// return pointer to penguin on x y 
penguin * getPenguin(int x, int y ,board * b){
	penguin_node * current = b->penguins;
	while (current != NULL) {
		if (current->penguin.x==x&&current->penguin.y==y){
			return &current->penguin;
		}
		current = current->next;
	}
	return NULL;
};

//adds player with given name and id to the board 
void addPlayer(char * name,int id,board * b){
	strcpy(b->listOfPlayers[b->numOfPlayers].name,name);
	b->listOfPlayers[b->numOfPlayers].number = id;
	b->listOfPlayers[b->numOfPlayers].points = 0;
	b->numOfPlayers++;
};

//places penguin on tile (x,y) for layer determined by player ID   
int placePenguin(int x,int y,int id,board *b){
	if(x<b->x_max && x>=0 && y<b->y_max && y>=0 && b->map[x][y]==1){
		penguin p;
		p.x=x;
		p.y=y;
		p.id=id;
		push_penuin(b->penguins, p);
		b->map[x][y]=0;
		addPoints(id, 1, b);
		return 0;
	}
	return 1;
};

//cheks if penguin can move
int canMove(penguin *p,board *b){
	int x = p->x;
	int y = p->y;
	if(x+1<b->x_max && isMove(x, y, x+1, y, b)){
		return 1;
	}
	if(x-1>=0 &&isMove(x, y, x-1, y, b)){
			return 1;
		}

	if(y+1<b->y_max &&isMove(x, y, x, y+1, b)){
			return 1;
		}

	if(y-1>=0 && isMove(x, y, x, y-1, b)){
			return 1;
		}
	return 0;
};

//cheks if player can move
int canPlayerMove(int id,board *b){
	penguin_node * current = b->penguins;
		while (current != NULL) {
			if (current->penguin.x==-1){
				break;
			}
			if(current->penguin.id==id && canMove(&current->penguin, b)){
				return 1;
			}
			current = current->next;
		}
		return 0;
};


//alocates memory for map and creates 2d arry 
void alocateMemForMap(board *b){
	b->map = array2d(b->x_max,b->y_max);
};

//create 2d array size x y
int **array2d(int x,int y){
	int len = sizeof(int *) * x + sizeof(int) * y * x; 
	int **arr = (int **)malloc(len); 
	// ptr is now pointing to the first element in of 2D array 
	int *ptr = (int *)(arr + x); 
	for(int i = 0; i < x; i++){
		arr[i] = (ptr + y * i);
	}
	return arr;
}

//returns index of 1st char c in string s
int index_of(char s[],char c){
	int i=0;
	while(s[i]!='\0'){
		if(s[i]==c){
			return i;
		}
		i++;
	}
	return -1;
}

//checks if c is a dgt
int is_dgt(char c){
	if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9'){
		return 1;
	}
	return 0;
}

//return index of player "name"
int playerNameToNum(char name[], board *b){
	for(int i=0; i<b->numOfPlayers; i++){
		if (strcmp(b->listOfPlayers[i].name, name)==0) {
			return b->listOfPlayers[i].number;
		}
	}
	return -1;
};

//check if player num is taken
int isPlayerNumTaken(int num, board *b){
	for(int i=0; i<b->numOfPlayers; i++){
		if (b->listOfPlayers[i].number==num) {
			return 1;
		}
	}
	return 0;
};