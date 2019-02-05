/*
 * preflop.c
 *
 *  Created on: Oct 15, 2018
 *      Author: Dan Wagar
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include "poker.h"
#include "preflop.h"

void printPFarr(PF pf[][13]){
	for(int i=0; i<13; i++){
		for(int j=0; j<13; j++){
			printf("%s", card_str[pf[i][j].hand.card1]);
			printf("%s ", card_str[pf[i][j].hand.card2]);

			double wins=(double)pf[i][j].numWins;
			double losses=(double)pf[i][j].numLosses;

			if((wins+losses) != 0){
				double winrate=(wins/(wins+losses))*100;
				//printf("wins = %f losses = %f ",wins, losses);
				printf("win rate is %2.2f ", winrate);
			}
		}
		printf("\n");
	}
}

void print1DPF(PF *pf){
	for (int i=0; i<169; i++){
		printf("%s", card_str[pf[i].hand.card1]);
		printf("%s ", card_str[pf[i].hand.card2]);

		double wins=(double)pf[i].numWins;
		double losses=(double)pf[i].numLosses;
		double ties=(double)pf[i].numTies;
		double total=wins+losses+ties;
		if((wins+losses+ties) != 0){
			double winrate=(wins/total)*100;
			double equity=(wins+(ties/2))/total*100;;
			//printf("wins = %f losses = %f ",wins, losses);
			printf("WR=%2.2f EQ=%2.2f ", winrate, equity);
		}
		if((i+1)%13==0)
			printf("\n");
	}
}

void convertTo1D(PF pf[][13], PF *pf1){
	for(int i=0; i<13; i++){
		for(int j=0; j<13; j++){
			pf1[i*13+j]=pf[i][j];
		}
	}
}


//The preflop array pf1D represents the 169 possible 2 card holdings
//that are possible preflop.  In order to calculate the odds of any
//given two card holdings winning at showdown we must find the number
//of times the holding wins against all other possible  opponent holdings
//on all possible boards.  This is 52! - 47!
int main(){
    time_t mytime;
	int64_t counter=0;
	int boardHand[7]={0};
	initEvaluator();
	//PF preflop[13][13]={{0}, 0, 0, 0};
	PF pf1D[169]={0};

	play();

/*	Code for original 2d preflop array.
 * 	//Add Pairs
	for(int i=4; i<=52; i+=4){
		//getIndex
		int index=(i/4)-1;
		preflop[index][index].hand.card1=i;
		preflop[index][index].hand.card2=i-1;
	}

	//Add suited
	for (int i=8; i<=52; i+=4){
		for(int j=i-4; j>=1; j-=4){
			int index1=(i-4)/4;
			int index2=index1 + ((j-i)/4);
			preflop[index1][index2].hand.card1=i;
			preflop[index1][index2].hand.card2=j;
		}
	}

	//Add offsuit
	for (int i=4; i<=52; i+=4){
		for (int j=1; j<=13 - (i/4); j++){
			int index=(i/4)-1;
			preflop[index][j+index].hand.card1=i;
			preflop[index][j+index].hand.card2=i+(j*4)-1;
		}
	}
*/
	for(int i=52; i>0; i-=4){
		int row=13-(i/4);
		for(int j=0; j<13; j++){
			int index;
			//add offsuit
			if(j<row){
				index=(row*13)+j;
				pf1D[index].hand.card1=52-(j*4)-1;
				pf1D[index].hand.card2=i;
			}
			//add suited
			if(j>row){
				index=(row*13)+(j);
				pf1D[index].hand.card1=i;
				pf1D[index].hand.card2=i-((j-row)*4);
			}
			else{
				index=row*13+row;
				pf1D[index].hand.card1=i;
				pf1D[index].hand.card2=i-1;
			}
		}
	}

    mytime = time(NULL);
    //int handCombos=0;
	for(int g=0; g<169; g++){
		int pfP1H1 = pf1D[g].hand.card1;
		int pfP1H2 = pf1D[g].hand.card2;
		int pfP2H1=0;
		int pfP2H2=0;
		//for(int h=g+1; h<169; h++){
		for(int h=1; h<52; h++){
			if(h==pfP1H1||h==pfP1H2){
				continue;
			}else
				pfP2H1=h;
			for (int h1=h+1; h1<53; h1++){
				if(h1==pfP1H1||h1==pfP1H2)
					continue;
				else
					pfP2H2=h1;
			//int pfP2H1 = pf1D[h].hand.card1;
			//int pfP2H2 = pf1D[h].hand.card2;
			//handCombos++;
			for(int i=1; i<49; i++){
				if(i==pfP1H1||i==pfP1H2||i==pfP2H1||i==pfP2H2)
					continue;
				for (int j=i+1; j<50; j++){
					if(j==pfP1H1||j==pfP1H2||j==pfP2H1||j==pfP2H2)
						continue;
					for (int k=j+1; k<51; k++){
						if(k==pfP1H1||k==pfP1H2||k==pfP2H1||k==pfP2H2)
							continue;
						for (int l=k+1; l<52; l++){
							if(l==pfP1H1||l==pfP1H2||l==pfP2H1||l==pfP2H2)
								continue;
							for (int m=l+1; m<53; m++){
								if(m==pfP1H1||m==pfP1H2||m==pfP2H1||m==pfP2H2)
									continue;
								boardHand[0]=pfP1H1;
								boardHand[1]=pfP1H2;
								boardHand[2]=i;
								boardHand[3]=j;
								boardHand[4]=k;
								boardHand[5]=l;
								boardHand[6]=m;
								int handInfo = GetHandValue(boardHand);
								int handCategoryP1 = handInfo >> 12;
								int rankWithinCategoryP1 = handInfo & 0x00000FFF;
								boardHand[0]=pfP2H1;
								boardHand[1]=pfP2H2;
								handInfo = GetHandValue(boardHand);
								int handCategoryP2 = handInfo >> 12;
								int rankWithinCategoryP2 = handInfo & 0x00000FFF;
								counter++;
								if(handCategoryP1 > handCategoryP2){
									pf1D[g].numWins++;
									//pf1D[h].numLosses++;
								}else if(handCategoryP1 < handCategoryP2){
									pf1D[g].numLosses++;
									//pf1D[h].numWins++;
								}else if(rankWithinCategoryP1 > rankWithinCategoryP2){
									pf1D[g].numWins++;
									//pf1D[h].numLosses++;
								}else if(rankWithinCategoryP1 < rankWithinCategoryP2){
									pf1D[g].numLosses++;
									//pf1D[h].numWins++;
								}else
									pf1D[g].numTies++;
							}
						}
					}
				}
			}
		}
	}
	}


	printf("Start Time : %s", ctime(&mytime));
	mytime = time(NULL);
	printf("Current Time : %s", ctime(&mytime));
	//printf("handCombos is %d\n", handCombos);
	printf("counter is %" PRId64 "\n", counter);
	//printf("Error count is %d\n", errorCount);
	print1DPF(pf1D);

	return 0;
}

