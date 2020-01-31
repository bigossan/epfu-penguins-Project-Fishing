#include <stdlib.h> 
#include <stdio.h> 
#include "board.h"
#include "movement.h"

//assign score to move according to the algorithm
int evaluateMove(moveCords c, board *b){
	int score = 0;
	int x = c.to_x;
	int y = c.to_y;
	
	score += b->map[x][y]*2;
	
	if(isFish(x, y-1, b))score += b->map[x][y-1];
	if(isFish(x-1, y, b))score += b->map[x-1][y];
	if(isFish(x, y+1, b))score += b->map[x][y+1];
	if(isFish(x+1, y, b))score += b->map[x+1][y];
	
	return score;
};

//calculate the best move for player(playerID)
moveCords getBestMove(int playerNum,board *b){
	int s,w,n,e,x,y;
	moveCords bestMove, newMove;
	int bestScore=0,score;
	
	penguin_node  *currentp =  b->penguins;
	while(currentp != NULL){
		if(currentp->penguin.id==playerNum){
			s=1;
			w=1;
			n=1;
			e=1;
			x=currentp->penguin.x;
			y=currentp->penguin.y;
			newMove.from_x=x;
			newMove.from_y=y;
			for(int i=1;s==1||w==1||n==1||e==1;i++){
				if(s && isFish(x, y-i, b)){
					newMove.to_x=x;
					newMove.to_y=y-i;
					score = evaluateMove(newMove, b);
					if(score>bestScore){
						bestScore=score;
						bestMove=newMove;
					}
				}
				else{
					s=0;
				}
				if(w && isFish(x-i, y, b)){
					newMove.to_x=x-i;
					newMove.to_y=y;
					score = evaluateMove(newMove, b);
					if(score>bestScore){
						bestScore=score;
						bestMove=newMove;
					}
				}
				else{
					w=0;
				}
				if(n && isFish(x, y+i, b)){
					newMove.to_x=x;
					newMove.to_y=y+i;
					score = evaluateMove(newMove, b);
					if(score>bestScore){
						bestScore=score;
						bestMove=newMove;
					}
				}
				else{
					n=0;
				}
				if(e && isFish(x+i, y, b)){
					newMove.to_x=x+i;
					newMove.to_y=y;
					score = evaluateMove(newMove, b);
					if(score>bestScore){
						bestScore=score;
						bestMove=newMove;
					}
				}
				else{
					e=0;
				}
			}
		}
		currentp = currentp->next;
	}
	if(bestScore==0){
		printf("No moves\n");
		exit(1);
	}
	return bestMove;
};