#ifndef TESTS_H
#define TESTS_H

#define OUTFILE "C:\\Users\\sam\\Documents\\cpp\\chess.html"

#include "ChessBoard.h"
#include "BoardIO.h"
#include "SearchTree.h"

class Tests
{
	public:
		Tests();
		virtual ~Tests();
	protected:
	private:
};

void abstractGame(ChessMove (*whitePlayer)(ChessBoard &,int),ChessMove (*blackPlayer)(ChessBoard &,int));
ChessMove randomPlayer(ChessBoard &board, int color);
ChessMove aiPlayer(ChessBoard &board, int color);
ChessMove consolePlayer(ChessBoard &board, int color);
void consoleGame();

#endif // TESTS_H
