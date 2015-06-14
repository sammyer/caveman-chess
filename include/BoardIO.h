#ifndef BOARDIO_H
#define BOARDIO_H

#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include "ChessMove.h"
#include "ChessBoard.h"


class BoardIO
{
	public:
		BoardIO();
		virtual ~BoardIO();

		void saveBoard(const ChessBoard &board, const char *filename);
		void saveBoards(vector<ChessBoard> &boards, const char *filename);
	protected:
		string getUnicode(int piece);
		void addHTMLHeader(ostream &output);
		void outputBoardAsHTMLTable(const ChessBoard &board, ostream &output, bool flip);
	private:
};

string positionString(Position position);
string moveString(ChessMove &chessMove);
char pieceSymbol(int piece);
int symbolToPiece(char pieceSymbol);
ChessBoard parseBoardStr(const string &boardStr);
void printToConsole(ChessBoard &board);
void printBoardStats(ChessBoard &board, bool showWhiteMoves, bool showBlackMoves);
ChessMove stringToMove(ChessBoard &board, const string &moveStr);

#endif // BOARDIO_H
