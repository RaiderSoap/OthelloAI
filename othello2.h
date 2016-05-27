
class RamOthello2 : public Othello
{
	public:
		RamOthello2(bool asBlack);
		virtual short aiMove();
	private:

		int evaluateBoard();
		int maxMin(bool maximizingPlayer, int depth);
		static const int EVALUATION_CHART[64];
		static const unsigned int AI_SEARCH_DEPTH = 7;
		static const short ENDING_GAME_TURN = 32;
};
