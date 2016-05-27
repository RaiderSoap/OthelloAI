//============================================================================
// Name        : RamOthello.cpp
// Author      : IvanXu
// Version     :
// Copyright   : MIT
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <stdlib.h>
#include <limits>
#include <ctime>
#include "othello.h"
#include "othello2.h"

using namespace std;
const int RamOthello2::EVALUATION_CHART[64] = {45,   -3,  11,  8,  8,  11,  -3,  45,
		                     				    -3,  -7,  -4,  1,  1,  -4,  -7,  -3,
		                     				    11,  -4,  2,   2,  2,   3,  -4,  11,
		                     				     8,  1,   2,  -3,  -3,  2,  1,   8,
		                     				     8,  1,   2,  -3,  -3,  2,  1,   8,
		                     				    11,  -4,  2,   2,  2,   3,  -4,  11,
		                     				    -3,  -7,  -4,  1,  1,  -4,  -7,  -3,
		                     				   45,   -3,  11,  8,  8,  11,  -3, 45};
RamOthello2::RamOthello2(bool asBlack): Othello(asBlack)
{}

short RamOthello2::aiMove(){
	//start timer
	clock_t begin = clock();
	//AI_SEARCH_DEPTH
	int highest = numeric_limits<int>::min();
	short highestMove = -1;
	short *v;
	v = findPossibleMoves(isBlack);
	//check
	if (*v == 0)
	{
		cout << "AI2 can not move this turn" << endl;
		return -1;
	}
	for (short it = 1; it <= *v; ++it){
		//make move
		int piecePut = *(v+it);

		hasChess[piecePut] = true;
		isBlackChess[piecePut] = isBlack;
		unsigned int fliped = flipAllAndReturn(piecePut);

		cout<<"Calculating move "<<piecePut;
		int score = maxMin(!isBlack,AI_SEARCH_DEPTH-1);
		cout<<"...  Score:"<<score<<endl;

		if (score >= highest) {
			highest = score;
			highestMove = piecePut;
		}

		//remove
		removePiece(piecePut, fliped);
	}
	  //end the timer and dis play time elapsed
	  clock_t end = clock();
	  double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
	  cout<<"------------"<<endl;
	  cout<<"Time elapsed: "<<elapsedSecs<<endl;
	 //return the best move
	  delete v;

	  cout<<"============================"<<endl;
	  cout<<"       AI2 Moved to "<<highestMove<<endl;
	  cout<<"============================"<<endl;

	return highestMove;
}
int RamOthello2::maxMin(bool maximizingPlayer, int depth){
	if (depth == 0)
	{
		return evaluateBoard();
	}else
	{
		int maxScore = numeric_limits<int>::min();
		int minScore = numeric_limits<int>::max();
		short *v;
		v = findPossibleMoves(maximizingPlayer);
		//check if any possible move
		if (*v==0)
		{
			return maxMin(!maximizingPlayer,depth-1);
		}

		//for each move
		for (short it = 1; it <= *v; ++it){
			//make move
			int piecePut = *(v+it);
			hasChess[piecePut] = true;
			isBlackChess[piecePut] = maximizingPlayer;
			unsigned int fliped = flipAllAndReturn(piecePut);
			//the recursion is at here
			int score = maxMin(!maximizingPlayer,depth-1);
			if (score > maxScore){
				maxScore = score;
			}
			if (score < minScore){
				minScore = score;
			}
			//remove
			removePiece (piecePut, fliped);

		}
		delete v;

		//returns
		if (maximizingPlayer==isBlack){ // if my turn
			return maxScore;
		}else{
			return minScore;
		}
	}
}

int RamOthello2::evaluateBoard(){
	int sum = 0;
	short B=0,W=0;
	for (unsigned int i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i)
	{
		if (hasChess[i])
		{
			if (isBlackChess[i] == isBlack)
			{
				sum += EVALUATION_CHART[i];
			}
			else
			{
				sum -= EVALUATION_CHART[i];
			};
			if (isBlackChess[i]){
				B++;
			}
			else{
				W++;
			}
		}
	}
	int additional = 0;
	if (B+W>ENDING_GAME_TURN)
	{
		if (isBlack)
		{
			additional = 200*(B-W)/(B+W);
		}
		else
		{
			additional = -200*(B-W)/(B+W);
		}
		return sum*(64-(B+W))/64+additional*(B+W)/64;
	}
	return sum;
}
