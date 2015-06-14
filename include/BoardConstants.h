#ifndef BOARDCONSTANTS_H
#define BOARDCONSTANTS_H

#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define BLACK 32
#define WHITE 16

#define MASK_COLOR 48
#define MASK_TYPE 7
#define EMPTY 0
#define INVALID -1

#define BIT_CASTLE_WK 1
#define BIT_CASTLE_WQ 2
#define BIT_CASTLE_BK 4
#define BIT_CASTLE_BQ 8


struct Position {
	int x;
	int y;

	Position() : x(0), y(0) {};
	Position(int _x, int _y) : x(_x), y(_y) {};
	bool operator==(const Position& rhs) {
		return x==rhs.x&&y==rhs.y;
	}
};

inline int getOpponent(int color) {
	return color==WHITE?BLACK:WHITE;
}

inline int getColor(int piece) {
	return (piece&MASK_COLOR);
}

inline int pieceType(int piece) {
	return (piece&MASK_TYPE);
}


#endif // BOARDCONSTANTS_H
