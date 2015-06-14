#include "ChessBoard.h"

ChessBoard::ChessBoard() {
	clear();
}

ChessBoard::~ChessBoard() {
	//dtor
}
void ChessBoard::clear() {
	for (int i=0;i<8;i++) {
		for (int j=0;j<8;j++) pieces[i][j]=EMPTY;
	}
	whiteKingPos=blackKingPos=pawnPassant=Position(-1,-1);
	disallowedCastling=0xFF;
}

void ChessBoard::setup() {
	for (int i=0;i<8;i++) {
		add(i,1,BLACK,PAWN);
		add(i,6,WHITE,PAWN);
		for (int j=2;j<6;j++) pieces[i][j]=EMPTY;
	}
	setupHelper(0,ROOK);
	setupHelper(1,KNIGHT);
	setupHelper(2,BISHOP);
	add(3,0,BLACK,QUEEN);
	add(4,0,BLACK,KING);
	add(3,7,WHITE,QUEEN);
	add(4,7,WHITE,KING);
	disallowedCastling=0;
}

void ChessBoard::setupHelper(int x, int type) {
	add(x,0,BLACK,type);
	add(7-x,0,BLACK,type);
	add(x,7,WHITE,type);
	add(7-x,7,WHITE,type);
}

void ChessBoard::add(int x, int y, int color, int type) {
	pieces[x][y]=color+type;
	if (type==KING) {
		if (color==WHITE) whiteKingPos=Position(x,y);
		else blackKingPos=Position(x,y);
	}
}

int ChessBoard::get(int x, int y) const {
	if (x<0||y<0||x>=8||y>=8) return INVALID;
	else return pieces[x][y];
}

void ChessBoard::setPiece(int x, int y, int piece) {
	if (x<0||y<0||x>=8||y>=8) return;
	pieces[x][y]=piece;
	if (piece==WHITE+KING) whiteKingPos=Position(x,y);
	else if (piece==BLACK+KING) blackKingPos=Position(x,y);
}


//search for next piece starting at x,y, and moving in direction dx,dy
int ChessBoard::searchPiece(int x, int y, int dx, int dy) {
	int piece;
	do {
		x+=dx;
		y+=dy;
		piece=get(x,y);
	} while (piece==EMPTY);
	return piece;
}


bool ChessBoard::isCheck(int color) {
	Position king=color==WHITE?whiteKingPos:blackKingPos;
	//sanity check
	if (get(king.x,king.y)==INVALID||get(king.x,king.y)==EMPTY) return false;

	int opponent=color==WHITE?BLACK:WHITE;
	int x=king.x;
	int y=king.y;
	int piece;

	//rook/queen
	piece=searchPiece(x,y,1,0);
	if (piece==opponent+ROOK||piece==opponent+QUEEN) return true;
	piece=searchPiece(x,y,0,1);
	if (piece==opponent+ROOK||piece==opponent+QUEEN) return true;
	piece=searchPiece(x,y,-1,0);
	if (piece==opponent+ROOK||piece==opponent+QUEEN) return true;
	piece=searchPiece(x,y,0,-1);
	if (piece==opponent+ROOK||piece==opponent+QUEEN) return true;

	//bishop/queen
	piece=searchPiece(x,y,1,1);
	if (piece==opponent+BISHOP||piece==opponent+QUEEN) return true;
	piece=searchPiece(x,y,1,-1);
	if (piece==opponent+BISHOP||piece==opponent+QUEEN) return true;
	piece=searchPiece(x,y,-1,1);
	if (piece==opponent+BISHOP||piece==opponent+QUEEN) return true;
	piece=searchPiece(x,y,-1,-1);
	if (piece==opponent+BISHOP||piece==opponent+QUEEN) return true;

	//pawn
	int forwards=color==WHITE?-1:1;
	if (get(x-1,y+forwards)==opponent+PAWN) return true;
	if (get(x+1,y+forwards)==opponent+PAWN) return true;

	//knight
	for (int dx=-2;dx<=2;dx++) {
		if (dx==0) continue;
		int dy=(dx>0)?3-dx:3+dx;
		if (get(x+dx,y+dy)==opponent+KNIGHT) return true;
		if (get(x+dx,y-dy)==opponent+KNIGHT) return true;
	}

	return false;
}

bool ChessBoard::isCheckmate(int color) {
	bool inCheck=isCheck(color);
	if (!inCheck) return false;
	vector<ChessMove> moves;
	getMoves(color,moves);
	return (moves.size()==0);
}

bool ChessBoard::isStalemate(int color) {
	bool inCheck=isCheck(color);
	if (inCheck) return false;
	vector<ChessMove> moves;
	getMoves(color,moves);
	return (moves.size()==0);
}

//------

void ChessBoard::getMoves(int color, vector<ChessMove> &moves) {
	moves.clear();
	for (int x=0;x<8;x++) {
		for (int y=0;y<8;y++) {
			if (pieces[x][y]==EMPTY) continue;
			if (getColor(pieces[x][y])==color) {
				getMovesForPiece(x,y,moves);
			}
		}
	}
}

bool ChessBoard::isCastlingAllowed(int color, int side) {
	int y=color==WHITE?7:0;
	bool throughCheck;
	int bitmask;
	if (color==WHITE&&side==KING) bitmask=BIT_CASTLE_WK;
	if (color==WHITE&&side==QUEEN) bitmask=BIT_CASTLE_WQ;
	if (color==BLACK&&side==KING) bitmask=BIT_CASTLE_BK;
	if (color==BLACK&&side==QUEEN) bitmask=BIT_CASTLE_BQ;
	if ((disallowedCastling&bitmask)!=0) return false;
	if (side==KING) {
		//inbetween squares are empty
		if (pieces[5][y]!=EMPTY||pieces[6][y]!=EMPTY) return false;
		//start square not in check
		if (isCheck(color)) return false;
		//middle square not in check
		if (color==WHITE) {
			whiteKingPos.x++;
			throughCheck=isCheck(color);
			whiteKingPos.x--;
		} else {
			blackKingPos.x++;
			throughCheck=isCheck(color);
			blackKingPos.x--;
		}
		if (throughCheck) return false;
	} else if (side==QUEEN) {
		//inbetween squares are empty
		if (pieces[1][y]!=EMPTY||pieces[2][y]!=EMPTY||pieces[3][y]!=EMPTY) return false;
		//start square not in check
		if (isCheck(color)) return false;
		//middle square not in check
		if (color==WHITE) {
			whiteKingPos.x--;
			throughCheck=isCheck(color);
			whiteKingPos.x++;
		} else {
			blackKingPos.x--;
			throughCheck=isCheck(color);
			blackKingPos.x++;
		}
		if (throughCheck) return false;
	}
	return true;
}


void ChessBoard::getMovesForPiece(int x, int y,vector<ChessMove> &moves) {
	int type=pieceType(pieces[x][y]);
	int color=getColor(pieces[x][y]);
	int opponent=getOpponent(color);
	int piece;
	int dx,dy;

    if (type==KING) {
    	for (dx=-1;dx<=1;dx++) {
			for (dy=-1;dy<=1;dy++) {
				if (dx==0&&dy==0) continue;
				piece=get(x+dx,y+dy);
				if (piece!=INVALID&&(piece==EMPTY||getColor(piece)==opponent)) addMove(moves,x,y,x+dx,y+dy);
			}
    	}

    	if (isCastlingAllowed(color,KING)) addMove(moves,x,y,x+2,y);
    	if (isCastlingAllowed(color,QUEEN)) addMove(moves,x,y,x-2,y);
    }
    if (type==PAWN) {
		int direction=color==WHITE?-1:1;
		int homeRow=color==WHITE?6:1;

		if (get(x,y+direction)==EMPTY) addMove(moves,x,y,x,y+direction);
		//diagonal attack
		if (getColor(get(x+1,y+direction))==opponent) addMove(moves,x,y,x+1,y+direction);
		if (getColor(get(x-1,y+direction))==opponent) addMove(moves,x,y,x-1,y+direction);
		//double move
		if (y==homeRow&&get(x,y+direction*2)==EMPTY) addMove(moves,x,y,x,y+direction*2);
		//en-passant
		if (pawnPassant.x>0&&pawnPassant.y==y+direction) {
			if (pawnPassant.x==x+1) addMove(moves,x,y,x+1,y+direction);
			if (pawnPassant.x==x-1) addMove(moves,x,y,x-1,y+direction);
		}
    } else if (type==KNIGHT) {
		for (dx=-2;dx<=2;dx++) {
			if (dx==0) continue;
			dy=(dx>0)?3-dx:3+dx;
			piece=get(x+dx,y+dy);
			if (piece!=INVALID&&(piece==EMPTY||getColor(piece)==opponent)) addMove(moves,x,y,x+dx,y+dy);
			piece=get(x+dx,y-dy);
			if (piece!=INVALID&&(piece==EMPTY||getColor(piece)==opponent)) addMove(moves,x,y,x+dx,y-dy);
		}
    }
    if (type==BISHOP||type==QUEEN) {
		addMovesInDirection(moves,x,y,1,1);
		addMovesInDirection(moves,x,y,1,-1);
		addMovesInDirection(moves,x,y,-1,1);
		addMovesInDirection(moves,x,y,-1,-1);
    }
    if (type==ROOK||type==QUEEN) {
		addMovesInDirection(moves,x,y,1,0);
		addMovesInDirection(moves,x,y,-1,0);
		addMovesInDirection(moves,x,y,0,1);
		addMovesInDirection(moves,x,y,0,-1);
    }
}

void ChessBoard::addMovesInDirection(vector<ChessMove> &moves, int x, int y, int dx, int dy) {
	int piece;
	int opponent=getOpponent(getColor(pieces[x][y]));
	int px=x;
	int py=y;
	do {
		px+=dx;
		py+=dy;
		piece=get(px,py);
		if (piece==EMPTY||getColor(piece)==opponent) addMove(moves,x,y,px,py);
	} while (piece==EMPTY);
}

void ChessBoard::addMove(vector<ChessMove> &moves, int fromX, int fromY, int toX, int toY) {
	ChessMove chessMove=getMove(fromX,fromY,toX,toY);
	if (chessMove.isValid) moves.push_back(chessMove);
}

/*
//doesn't do complete validation
bool validateBasic(ChessMove &chessMove) {
	if (chessMove.from.x<0||chessMove.from.x>7||chessMove.from.y<0||chessMove.from.y>7||
		chessMove.to.x<0||chessMove.to.x>7||chessMove.to.y<0||chessMove.to.y>7) return false;
	int dx=chessMove.to.x-chessMove.from.x;
	int dy=chessMove.to.y-chessMove.from.y;

	if (piece==ROOK&&dx!=0&&dy!=0) return false;
	//TODO
	return true;
}
*/

bool ChessBoard::validateFromList(ChessMove &chessMove) {
	if (chessMove.piece==EMPTY) return false;
	vector<ChessMove> validMoves;
	getMoves(getColor(chessMove.piece),validMoves);
	for (vector<ChessMove>::iterator moveIter=validMoves.begin();moveIter!=validMoves.end();moveIter++) {
        if (moveIter->from==chessMove.from&&moveIter->to==chessMove.to) return true;
	}
	return false;
}

ChessMove ChessBoard::getMove(int fromX, int fromY, int toX, int toY) {
	int piece=pieces[fromX][fromY];
	ChessMove chessMove=ChessMove(Position(fromX,fromY),Position(toX,toY),piece,pieces[toX][toY]);

	//en-passant move
	if (toX==pawnPassant.x&&toY==pawnPassant.y) {
		chessMove.isEnPassant=true;
		chessMove.capture=pieces[toX][fromY];
	}
	//make sure move doesn't cause check
	int color=getColor(piece);
	applyMove(chessMove,true);
	if (isCheck(color)) chessMove.isValid=false;
	else if (isCheck(getOpponent(color))) chessMove.isCheck=true;
	undoMove(chessMove);

	int newCastling=disallowedCastling;
	if (piece==WHITE+KING) newCastling|=(BIT_CASTLE_WK+BIT_CASTLE_WQ);
	else if (piece==BLACK+KING) newCastling|=(BIT_CASTLE_BK+BIT_CASTLE_BQ);
	else if (piece==WHITE+ROOK&&fromX==0) newCastling|=BIT_CASTLE_WQ;
	else if (piece==WHITE+ROOK&&fromX==7) newCastling|=BIT_CASTLE_WK;
	else if (piece==BLACK+ROOK&&fromX==0) newCastling|=BIT_CASTLE_BQ;
	else if (piece==BLACK+ROOK&&fromX==7) newCastling|=BIT_CASTLE_BK;

	chessMove.allowedCastlingDiff=newCastling-disallowedCastling;

	return chessMove;
}


//------

float ChessBoard::getScore(int color) {
	float score=0;
	int piece;
	for (int x=0;x<8;x++) {
		for (int y=0;y<8;y++) {
			piece=pieces[x][y];
			if (piece==EMPTY) continue;
			if (getColor(piece)==color) score+=getPieceScore(pieceType(piece));
			else score-=getPieceScore(pieceType(piece));
		}
	}
	return score;
}

float ChessBoard::getPieceScore(int type) {
	switch (type) {
		case PAWN: return 1;
		case KNIGHT: case BISHOP: return 3;
		case ROOK: return 5;
		case QUEEN: return 9;
		default: return 0;
	}
}

//----------------------------

void ChessBoard::applyMove(ChessMove &chessMove) {
	applyMove(chessMove,false);
}

void ChessBoard::applyMove(ChessMove &chessMove, bool isTemp) {
	int piece=chessMove.piece;
	if (piece==WHITE+KING) whiteKingPos=chessMove.to;
	else if (piece==BLACK+KING) blackKingPos=chessMove.to;
	//promotion
	else if (piece==WHITE+PAWN&&chessMove.to.y==0) piece=WHITE+QUEEN;
	else if (piece==BLACK+PAWN&&chessMove.to.y==7) piece=BLACK+QUEEN;

	int castle=chessMove.isCastlingMove();
	int y=chessMove.from.y;
	if (castle==2) {
		pieces[5][y]=pieces[7][y];
		pieces[7][y]=EMPTY;
	} else if (castle==3) {
		pieces[3][y]=pieces[0][y];
		pieces[0][y]=EMPTY;
	}

	//update en-passant capture position -- warning: cannot be undone so don't do this for temp moves when chechking for check
	if (!isTemp) {
		if (pieceType(piece)==PAWN&&abs(chessMove.from.y-chessMove.to.y)==2) {
			pawnPassant.x=chessMove.from.x;
			pawnPassant.y=(chessMove.from.y+chessMove.to.y)/2;
		} else {
			pawnPassant.x=-1;
			pawnPassant.y=-1;
		}
	}
	//en-passant attack
	if (chessMove.isEnPassant) {
		pieces[chessMove.to.x][chessMove.from.y]=EMPTY;
	}

	pieces[chessMove.from.x][chessMove.from.y]=EMPTY;
	pieces[chessMove.to.x][chessMove.to.y]=piece;
	disallowedCastling+=chessMove.allowedCastlingDiff;
}

void ChessBoard::undoMove(ChessMove &chessMove) {
	if (chessMove.piece==WHITE+KING) whiteKingPos=chessMove.from;
	else if (chessMove.piece==BLACK+KING) blackKingPos=chessMove.from;

	int castle=chessMove.isCastlingMove();
	int y=chessMove.from.y;
	if (castle==2) {
		pieces[7][y]=pieces[5][y];
		pieces[5][y]=EMPTY;
	} else if (castle==3) {
		pieces[0][y]=pieces[3][y];
		pieces[3][y]=EMPTY;
	}

	//promotion will be automatically undone
	pieces[chessMove.to.x][chessMove.to.y]=chessMove.capture;
	pieces[chessMove.from.x][chessMove.from.y]=chessMove.piece;
	disallowedCastling-=chessMove.allowedCastlingDiff;

	if (chessMove.isEnPassant) {
		pieces[chessMove.to.x][chessMove.to.y]=EMPTY;
		pieces[chessMove.to.x][chessMove.from.y]=chessMove.capture;
	}
}
