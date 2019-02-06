/*
 * preflop.h
 *
 *  Created on: Oct 15, 2018
 *      Author: Dan Wagar
 */

#ifndef PREFLOP_H_
#define PREFLOP_H_

typedef struct hand{
	int card1;
	int card2;
}HAND;

typedef struct preflop{
	HAND hand;
	uint numWins;
	uint numLosses;
	int numTies;
}PF;


#endif /* PREFLOP_H_ */
