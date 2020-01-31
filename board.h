#ifndef _board_h_
#define _board_h_


//penguin data
typedef struct penguin {
	int x;
	int y;
	int id;
}penguin;


//penguin list node
typedef struct penguin_node{
	penguin penguin;
	struct penguin_node *next;
}penguin_node;


//structure to keep board game
typedef struct boards {
	unsigned int x_max; //board size in x dimencio 
	unsigned int y_max; //board size in y dimencio 
	unsigned int numOfPlayers : 4;
	penguin_node * penguins;
	int **map;
	struct player{
		char name[128];
		unsigned int number;
		unsigned int points;
	}listOfPlayers[9];
} board;


//structure to keep move cordinates
typedef struct moveCords{
	int from_x;
	int from_y;
	int to_x;
	int to_y;
}moveCords;


//structure to keep placement cordinates 
typedef struct placementCords{
	int x;
	int y;
}placementCords;

void push_penuin(penguin_node*,penguin);

void iniciate(board *);
void saveBoard(board *, char *);
void move(int, int, int, int, board *);
void moveC(moveCords, board *);
void addPoints(int, int, board *);
void addPlayer(char *,int,board *);
void alocateMemForMap(board *);

int isFish(int, int, board *);
int isPlayer(int, int, board *);
int isMove(int,int,int,int,board *);
int isMoveC(moveCords,board *);
int safeMove(int, int, int, int , board *);
int safeMoveC(moveCords, board *);

int placePenguin(int,int,int,board *);
int canMove(penguin *,board *);
int canPlayerMove(int,board *);

int index_of(char s[],char c);
int is_dgt(char);

int playerNameToNum(char *, board *);
int isPlayerNumTaken(int , board *);

int **array2d(int,int);



board *loadBoard(char *);

penguin * getPenguin(int, int, board *);

#endif