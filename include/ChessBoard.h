#include <vector>
#include <sstream>
#include <stdlib.h>
#include "BoardConstants.h"
#include "ChessMove.h"

#ifndef CHESSBOARD_H
#define CHESSBOARD_H



using namespace std;

//todo: promotion, en-passant, castling

class ChessBoard {
	public:
		ChessBoard();
		virtual ~ChessBoard();

		void setup();
		void clear();
		bool isCheck(int color);
		bool isCheckmate(int color);
		bool isStalemate(int color);
		void getMoves(int color, vector<ChessMove> &moves);
		float getScore(int color);
		int get(int x, int y) const;
		void setPiece(int x, int y, int piece);

		void applyMove(ChessMove &chessMove);
		void undoMove(ChessMove &chessMove);
		ChessMove getMove(int fromX, int fromY, int toX, int toY);

		bool isCastlingAllowed(int color, int side);

	protected:
		int pieces[8][8];

	public:
		Position whiteKingPos;
		Position blackKingPos;
		int disallowedCastling;
		Position pawnPassant;

		void add(int x, int y, int color, int type);
		void setupHelper(int x, int type);

		int searchPiece(int x, int y, int dx, int dy);

		void getMovesForPiece(int x, int y, vector<ChessMove> &moves);
		void addMove(vector<ChessMove> &moves,int fromX, int fromY, int toX, int toY);
		void addMovesInDirection(vector<ChessMove> &moves,int x, int y, int dx, int dy);

		//bool validateBasic(ChessMove &chessMove);
		bool validateFromList(ChessMove &chessMove);

		float getPieceScore(int type);
		void applyMove(ChessMove &chessMove, bool isTemp);
};



#endif // CHESSBOARD_H
