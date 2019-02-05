/*
 * poker.h
 *
 *  Created on: Oct 5, 2017
 *      Author: Dan Wagar
 */

#ifndef POKER_H_
#define POKER_H_
#define DECK_SIZE 52
#define NUM_FLOP  3
#define NUM_TURN  1
#define NUM_RIVER 1


static char *card_str[] = {
		"BAD", "2c", "2d", "2h", "2s",
		"3c", "3d", "3h", "3s",
		"4c", "4d", "4h", "4s",
		"5c", "5d", "5h", "5s",
		"6c", "6d", "6h", "6s",
		"7c", "7d", "7h", "7s",
		"8c", "8d", "8h", "8s",
		"9c", "9d", "9h", "9s",
		"Tc", "Td", "Th", "Ts",
		"Jc", "Jd", "Jh", "Js",
		"Qc", "Qd", "Qh", "Qs",
		"Kc", "Kd", "Kh", "Ks",
		"Ac", "Ad", "Ah", "As",
};

static char * rank_str[] = {
	"INVALID HAND",
	"HIGH CARD",
	"PAIR",
	"TWO PAIR",
	"SET",
	"STRAIGHT",
	"FLUSH",
	"FULLHOUSE",
	"QUADS",
	"STRAIGHT FLUSH"
};

typedef enum boolean{
	FALSE,
	TRUE
}bool;

typedef struct player{
	bool human;
	bool button;
	bool fold;
	bool winner;
	int holding[2];
	int position;
	double stack;
	int hand[5];
}PLAYER;

void initEvaluator();

int GetHandValue(int* pCards);

void printArr(int *arr, int s);

#endif /* POKER_H_ */
