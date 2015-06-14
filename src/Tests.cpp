#include "Tests.h"

Tests::Tests()
{
	//ctor
}

Tests::~Tests()
{
	//dtor
}

void randomGame() {
	ChessBoard board;
	unsigned int i,j;
	vector<ChessMove> moves;

	BoardIO boardWriter;
	vector<ChessBoard> boards;
	int color=WHITE;
	for (i=0;i<16;i++) {
		boards.push_back(board);
		board.getMoves(color,moves);
		j=rand()%moves.size();
		board.applyMove(moves[j]);
		cout << moveString(moves[j]) << "\n";
		color=getOpponent(color);
	}
	boardWriter.saveBoards(boards, "C:\\Users\\sam\\Documents\\cpp\\chess.html");
}


void testCheck() {

}
