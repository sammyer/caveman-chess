#include "ChessMove.h"

ChessMove::ChessMove(Position _from, Position _to, int _piece, int _capture) :
		from(_from),to(_to),piece(_piece),capture(_capture),
		allowedCastlingDiff(0), isEnPassant(false), isValid(true) {};

ChessMove::ChessMove():
	from(Position(0,0)),to(Position(0,0)),piece(0),capture(0),
		allowedCastlingDiff(0), isEnPassant(false), isValid(true) {};

ChessMove::~ChessMove() {
	//dtor
}

//returns 2 for king-side, 3 for queen-side, 0 otherwise
int ChessMove::isCastlingMove() {
	if (from.x==4&&pieceType(piece)==KING) {
		if (to.x==6) return 2;
		if (to.x==2) return 3;
	}
	return 0;
}
