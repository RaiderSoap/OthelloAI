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

class RamOthello : public Othello
{
	public:
		RamOthello(bool asBlack);
		virtual short aiMove();
	private:

		int evaluateBoard();
		int maxMin(bool maximizingPlayer, int depth);
		static const int EVALUATION_CHART[64];
		static const unsigned int AI_SEARCH_DEPTH = 7;
		static const short ENDING_GAME_TURN = 48;
};
const int RamOthello::EVALUATION_CHART[64] = {45,   -3,  11,  8,  8,  11,  -3,  45,
		                     				    -3,  -7,  -4,  1,  1,  -4,  -7,  -3,
		                     				    11,  -4,  2,   2,  2,   3,  -4,  11,
		                     				     8,  1,   2,  -3,  -3,  2,  1,   8,
		                     				     8,  1,   2,  -3,  -3,  2,  1,   8,
		                     				    11,  -4,  2,   2,  2,   3,  -4,  11,
		                     				    -3,  -7,  -4,  1,  1,  -4,  -7,  -3,
		                     				   45,   -3,  11,  8,  8,  11,  -3, 45};

Othello::Othello(bool b) : isBlack(b)
{}

Othello::~Othello()
{}


void Othello::setupConst(){
	for (short pos = 0; pos < 64; ++pos)
	{
		dirCanStart[pos][0] = pos%BOARD_SIZE!=0;
		dirCanStart[pos][1] = pos%BOARD_SIZE!=BOARD_SIZE-1;
		dirCanStart[pos][2] = pos/BOARD_SIZE!=0;
		dirCanStart[pos][3] = pos/BOARD_SIZE!=BOARD_SIZE-1;
		dirCanStart[pos][4] = pos%BOARD_SIZE!=0 && pos/BOARD_SIZE!=0;
		dirCanStart[pos][5] = pos%BOARD_SIZE!=BOARD_SIZE-1 && pos/BOARD_SIZE!=0;
		dirCanStart[pos][6] = pos%BOARD_SIZE!=0 && pos/BOARD_SIZE!=BOARD_SIZE-1;
		dirCanStart[pos][7] = pos%BOARD_SIZE!=BOARD_SIZE-1 && pos/BOARD_SIZE!=BOARD_SIZE-1;
	};
	for (unsigned int i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i)
	{
		hasChess[i] = false;
		isBlackChess[i] = false;
	};
	//only for 8*8
	hasChess[27] = true;
	hasChess[28] = true;
    hasChess[36] = true;
    hasChess[35] = true;
    isBlackChess[35] = true;
    isBlackChess[28] = true;
}

bool Othello::flipAble(short pos, bool asBlack){
	int shift = 0;
	for (unsigned int i = 0; i < 8; ++i){
		shift = 0;
		if (dirCanStart[pos][i] && hasChess[pos+deltaPos[i]] && isBlackChess[pos+deltaPos[i]] != asBlack)
		{
			while(true){
				shift+=deltaPos[i];
				if (!reachBoarder[i][pos+shift])
					//when new pos is not boarder
				{
					if (!hasChess[pos+shift+deltaPos[i]])
						//if new new pos has no chess
					{
						break;
					}else if (isBlackChess[pos+shift+deltaPos[i]] == asBlack)
						//if has chess at new new pos and is a same color
					{
						return true;
					}
				}
				else
					//when pos is at boarder
				{
					break;
				}
			};
		};
	};
	return false;
}

short* Othello::findPossibleMoves(bool asBlack){
	//vector<int> list;
	short* list = new short[30];
	//short list[30];
	short count = 1,sum = 0;
	for (unsigned int i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i){
		if (!hasChess[i] && flipAble(i,asBlack)){
			list[count] = i;
			sum++;
			++count;
		}
	}
	list[0] = sum;
	return list;
}

unsigned int Othello::flipAllAndReturn(short pos){
	int shift = 0;
	unsigned int result = 0,temp=0;
	for (int i = 0; i < 8; ++i)
		// for 8 direction
	{
		temp = 0;
		shift = 0;
		if (dirCanStart[pos][i] && hasChess[pos+deltaPos[i]] && isBlackChess[pos+deltaPos[i]] != isBlackChess[pos])
			//when not reaching boarder, has a chess at new pos, and the new chess is different color
		{
			while(true){
				shift+=deltaPos[i];
				if (!reachBoarder[i][pos+shift]&&hasChess[pos+shift+deltaPos[i]])
					//when new pos is not boarder and new new pos has chess
				{
					temp += pow7[i];
					if (isBlackChess[pos+shift+deltaPos[i]] == isBlackChess[pos])
						//if the new new pos is same color
					{
						result+=temp;
						temp = 0;
						break;
					}
				}
				else
					//when pos is at boarder
				{
					temp = 0;
					break;
				}

			};
		};
	};
	//flip all
	unsigned int d;
	for (unsigned int i = 0; i <= 7; ++i)
	{
		d = result/pow7[i]%7;
		for (unsigned int j = 1; j <= d; ++j)
		{

			isBlackChess[pos+j*deltaPos[i]] = !isBlackChess[pos+j*deltaPos[i]];
		};
	};

	//cout<<"result: "<<result<<endl;
	return result;
}

void Othello::putAPiece(short pos, bool asBlack){
	hasChess[pos] = true;
	isBlackChess[pos] = asBlack;
	flipAllAndReturn(pos);
}
void Othello::removePiece (short pos, unsigned int fliped) {
	hasChess[pos] = false;
	unsigned int d;
	for (unsigned int i = 0; i <= 7; ++i)
	{
		d = fliped/pow7[i]%7;
		for (unsigned int j = 1; j <= d; ++j)
		{
			isBlackChess[pos+j*deltaPos[i]] = !isBlackChess[pos+j*deltaPos[i]];
		}
	}
}
void Othello::displayDebugBoard(bool asBlack){
	int blackNumber = 0;
	int whiteNumber = 0;
	short *v;
	v = findPossibleMoves(asBlack);

	for (short i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i)
	{
		if (i%BOARD_SIZE==0)
		{
			cout<<endl;
		}
		if (hasChess[i])
		{
			if (isBlackChess[i]){
				blackNumber++;
				cout<<"B ";
			}
			else{
				whiteNumber++;
				cout<<"W ";
			}
		}else{
			bool found = false;
			for (short it = 1; it <= *v; ++it)
			{
				if (*(v+it)==i)
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				cout<<"* ";
			}else{
				cout<<"_ ";
			}
		}
	};
	cout<<endl;
	cout<<"Black number: "<<blackNumber;
	cout<<endl;
	cout<<"White number: "<<whiteNumber;
	cout<<endl;
}

void Othello::debugPutChess(bool asBlack){
	cout<<"------------------"<<endl;
	short *v;
	v = findPossibleMoves(asBlack);

	if (*v!=0)
	{
		for (short i = 1; i <= *v; ++i)
		{
			cout<< i <<": " <<*(v+i)<<" "<<endl;
		}
		bool found = false;
		while(!found){
			int input;
			cin>>input;


			for (int it = 1; it <= *v; ++it)
			{
				if (*(v+it)==input)
				{
					found = true;
					break;
				}
			}
			if (found)
			{
				//put a piece
				putAPiece(input,asBlack);
				cout<<"INPUT: "<<input<<endl;
			}else{
				cout<<"WRONG INPUT"<<endl;
			}
		}

	}else{
		cout<<"Player can not move, enter anything to confirm"<<endl;
		int input;
		cin>>input;
	}

	delete v;
}

RamOthello::RamOthello(bool asBlack): Othello (asBlack){
}


short RamOthello::aiMove(){
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
		cout << "AI1 can not move this turn" << endl;
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

		if (score > highest) {
			highest = score;
			highestMove = piecePut;
		}

		//remove
		removePiece (piecePut, fliped);
	}
	  //end the timer and dis play time elapsed
	  clock_t end = clock();
	  double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
	  cout<<"------------"<<endl;
	  cout<<"Time elapsed: "<<elapsedSecs<<endl;
	 //return the best move
	  delete v;

	  cout<<"============================"<<endl;
	  cout<<"       AI1 Moved to "<<highestMove<<endl;
	  cout<<"============================"<<endl;

	return highestMove;
}

int RamOthello::maxMin(bool maximizingPlayer, int depth){
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
			//cout<<"score: "<<score<<" ,level "<<depth<<", turn:"<<maximizingPlayer<<endl;
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

int RamOthello::evaluateBoard(){
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
			}
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
int main()
{
	//initialize game
	Othello::setupConst();
	cout<<"Enter 1 for AI Black, 0 for AI White."<<endl;
	int ip;
	cin>>ip;

	bool AIIsBlack = ip;
	Othello* p1;
	Othello* p2;
	if (AIIsBlack) {
		//AI go first
		p1 = new RamOthello(AIIsBlack);
		p2 = new RamOthello2 (!AIIsBlack);
	} else {
		p2 = new RamOthello(!AIIsBlack);
		p1 = new RamOthello2 (AIIsBlack);
	}
	short aiGoal;
	short moved;
	do {
		moved = 0;
		Othello::displayDebugBoard(AIIsBlack);
		aiGoal = p1->aiMove();
		if (aiGoal != -1) {
			Othello::putAPiece(aiGoal, AIIsBlack);
			moved++;
		}
		Othello::displayDebugBoard(!AIIsBlack);
		aiGoal = p2->aiMove();
		if (aiGoal != -1) {
			Othello::putAPiece(aiGoal, !AIIsBlack);
			moved++;
		}
	} while (moved > 0);

	// game over
	cout<<"====================Game Over======"<<endl;

	return 0;
}
