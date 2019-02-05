/*
 * poker.c
 *
 *  Created on: Oct 5, 2017
 *      Author: Dan Wagar
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "poker.h"

// The 2+2 lookup table.
int HR[32487834];

//Card 0 is reserved as a BAD card, so begin with card 1
void buildDeck(int *deck){
	for(int i=1; i<=DECK_SIZE;i++){
		deck[i-1]=i;
	}
}

void printDeck(int *deck){
	for (int i=0; i<DECK_SIZE; i++){
		printf("card is %s\n", card_str[deck[i]]);
	}
}

//May need to investigate better random number generator
void shuffleDeck(int *d){
	srand(time(NULL));
	rand();
	for (int i = DECK_SIZE - 1; i > 0; i--){
		int r = rand() % DECK_SIZE;
		int temp=d[i];
		d[i]=d[r];
		d[r]=temp;
	}
	//printDeck(d);
}

void deal(int *deck, int num, PLAYER *players){
	int cardsToDeal=2;
	int counter=0;
	for(int i=0; i<num; i++){
		if(players[i].button==TRUE){
			players[i].button=FALSE;
			if(i==(num-1))
				players[0].button=TRUE;
			else
				players[i+1].button=TRUE;
		}
	}
	for(int i=0; i<cardsToDeal; i++){
		for(int j=0; j<num; j++){
			players[j].holding[i]=deck[counter];
			counter++;
		}
	}
}

void flop(int *d, int *b, int n){
	int topCard=n*2;
	printf("BOARD: ");
	for(int i=topCard;i<(topCard+NUM_FLOP);i++){
		b[i-topCard]=d[i];
		printf("%s ", card_str[d[i]]);
	}
}

void turn(int *d, int *b, int n){
	int topCard=(n*2 + NUM_FLOP);
	b[3]=d[topCard];
	printf("%s ", card_str[b[3]]);
}

void river(int *d, int *b, int n){
	int topCard=(n*2 + NUM_FLOP + NUM_TURN);
	b[4]=d[topCard];
	printf("%s\n\n", card_str[b[4]]);
}

void printArr(int *arr, int s){
	//int boardLength=sizeof(b)/sizeof(CARD);
    for(int i=0; i<s; i++){
    	printf("%s ", card_str[arr[i]]);
    }
	printf("\n");
}

/*
void bet(PLAYER *p, int s){
	//Find button
	int buttonIndex;
	for(int i=0; i<s; i++){
		if (p[0].button==TRUE){
			buttonIndex=i;
			break;
		}
	}
	for(int i=(buttonIndex+1); i<(s+buttonIndex); i++){
		if(i>=s)
			int index=i-s;
		if(p[index])

	}
}
*/

void showholding(PLAYER *p){
	printf("Player %d has ",p->position);
	for(int i=0; i<2; i++){
		printf( "%s ", card_str[p->holding[i]]);
	}
    printf("\n");
}

// 2+2 hand evaluator algorithm using the 2+2 lookup table
int GetHandValue(int* pCards){
    int p = HR[53 + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    return HR[p + *pCards++];
}

void initEvaluator(){
    memset(HR, 0, sizeof(HR));
    FILE * fin = fopen("HANDRANKS.DAT", "rb");
    // Load the HANDRANKS.DAT file data into the HR array
    size_t bytesread = fread(HR, sizeof(HR), 1, fin);
    fclose(fin);
}

int play() {

	initEvaluator();

	PLAYER players[2]={0,0,0,0,{0},100.0,{0}};
    int deck[52]={0};
    PLAYER p1;
    PLAYER p2;
    p1.button=TRUE;
    p1.position=1;
    p2.position=2;
    players[0]=p1;
    players[1]=p2;
    int board[5]={0};

    buildDeck(deck);

    shuffleDeck(deck);

    //printDeck(deck);


    int numPlayers=(sizeof(players) / sizeof(players[0]));
    deal(deck, numPlayers, players);

    showholding(&players[0]);
    showholding(&players[1]);

    //bet(players, numPlayers);

    flop(deck, board, numPlayers);
    turn(deck, board, numPlayers);
    river(deck, board, numPlayers);

    for(int i=0; i<numPlayers; i++){
		int boardHand[7]={board[0], board[1], board[2], board[3], board[4],
				players[i].holding[0], players[i].holding[1]};

		int handInfo = GetHandValue(boardHand);
		int handCategory = handInfo >> 12;
		int rankWithinCategory = handInfo & 0x00000FFF;
		printf("hand category is %d\n", handCategory);
		printf("Player %d has a %s\n", i+1, rank_str[handCategory]);
		printf("Rank in category is %d\n\n", rankWithinCategory);
    }
   return 0;
}
