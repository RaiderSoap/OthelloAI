const static unsigned short BOARD_SIZE = 8;
const static  bool reachBoarder[8][64] = {
	{
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false
	},
	{
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true
	},
	{
	   true,true,true,true,true,true,true,true,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false
	},
	{
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   false,false,false,false,false,false,false,false,
	   true,true,true,true,true,true,true,true
	},
	{
	   true,true,true,true,true,true,true,true,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false
	},
	{
	   true,true,true,true,true,true,true,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true
	},
	{
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,false,false,false,false,false,false,false,
	   true,true,true,true,true,true,true,true
	},
	{
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   false,false,false,false,false,false,false,true,
	   true,true,true,true,true,true,true,true
	}
};

static const unsigned int pow7[] = {1,7,49,343,2401,16807,117649,823543};
static const short deltaPos[8] = {
	-1, 1, -BOARD_SIZE, BOARD_SIZE,-1-BOARD_SIZE,1-BOARD_SIZE,-1+BOARD_SIZE,1+BOARD_SIZE
};


//-------------------------------
static bool dirCanStart[64][8];
static bool hasChess[BOARD_SIZE*BOARD_SIZE];
static bool isBlackChess[BOARD_SIZE*BOARD_SIZE];

// Othello
class Othello
{
	public:
		Othello(bool asBlack);
		static void displayDebugBoard(bool asBlack);
		static void debugPutChess(bool asBlack);
		static void setupConst();
		static bool flipAble(short pos, bool asBlack);
		static short* findPossibleMoves(bool asBlack);
		static unsigned int flipAllAndReturn(short pos);
		static void putAPiece(short pos, bool asBlack);
		static void removePiece (short pos, unsigned int flipped);

		virtual short aiMove() = 0;
		virtual ~Othello();

	protected:
		bool isBlack;
};
