#include "BoardConstants.h"

#ifndef CHESSMOVE_H
#define CHESSMOVE_H

class ChessMove
{
	public:
		ChessMove(Position _from, Position _to, int _piece, int _capture);
		ChessMove();
		virtual ~ChessMove();

		Position from;
		Position to;
		int piece;
		int capture;
		int allowedCastlingDiff;
		bool isEnPassant;
		bool isCheck;
		bool isValid;

		int isCastlingMove();

	protected:
	private:
};

#endif // CHESSMOVE_H
