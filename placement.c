#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "placement.h"

//assign score to placement according to the algorithm
int evaluatePlacement(placementCords c,board *b){
	int x = c.x>(b->x_max/2) ? (b->x_max)-c.x : c.x; //distance to board edge in x -xis
	int y = c.y>(b->y_max/2) ? (b->y_max)-c.y : c.y; //distance to board edge in y-axis
	int score=x*x+y*y;
	if(isFish(c.x+1, c.y, b)==0)score=score/2;
	if(isFish(c.x-1, c.y, b)==0)score=score/2;
	if(isFish(c.x, c.y+1, b)==0)score=score/2;
	if(isFish(c.x, c.y-1, b)==0)score=score/2;
	if(score<0)score=0;
	return score;
};

//calculates the best placement for player(playerID)
placementCords getBestPlacement(int playerID, int penguins, board *b){
	placementCords bestPlacement, current;
	penguin_node *currentp = b->penguins;
	while(currentp!=NULL){
		if(currentp->penguin.id==playerID)penguins--;
		currentp=currentp->next;
	}
	if (penguins<=0){
		printf("All penguins are placed");
		exit(1);
	}
	int bestScore=-1,score=0;
	for(int i=0; i<b->x_max;i++){
			for(int j=0; j<b->y_max;j++){
				if(b->map[i][j]==1){
					current.x=i;
					current.y=j;
					score = evaluatePlacement(current, b);
					if (score > bestScore){
						bestScore=score;
						bestPlacement=current;
					}
				}
			}
		}
	return bestPlacement;
};