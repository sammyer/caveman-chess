#include "SearchTree.h"

SearchTree::SearchTree()
{
	//ctor
}

SearchTree::~SearchTree()
{
	//dtor
}

ChessMove SearchTree::findBestMove(ChessBoard& board, int color, int depth) {
	vector<ChessMove> validMoves=board.getMoves(color);
	sort(validMoves.begin(),validMoves.end(),compareMovesHighestScoreFirst);
	float INFINITY=numeric_limits<float>::infinity();
	alphaBetaSearch(board,0,color,depth,-INFINITY,INFINITY,true);
}

float SearchTree::alphaBetaSearch(ChessBoard& board,float boardValue, int color, int depth, float alpha, float beta, bool isPlayer) {
	if (depth==0) return value;

	vector<ChessMove> validMoves=board.getMoves(isPlayer?color:getOpponent(color));
	if (validMoves.size()==0) return value;
	sort(validMoves.begin(),validMoves.end(),compareMovesHighestScoreFirst);

	float v;
	float newBoardValue;
	float INFINITY=numeric_limits<float>::infinity();
	if (isPlayer) {
		v=-INFINITY;
		for (vector<ChessMove>::iterator moveIter=validMoves.begin();moveIter!=validMoves.end();moveIter++) {
			ChessBoard newBoard=board;
			newBoard.applyMove(*moveIter);
			newBoardValue=boardValue+moveHeuristic(*moveIter);

			v=max(v,alphaBetaSearch(newBoard,newBoardValue,color,depth-1,alpha,beta,false))
			alpha=max(alpha,v);
			if (beta<=alpha) break;
		}
		return v;
	} else {
		v=INFINITY;
		for (vector<ChessMove>::iterator moveIter=validMoves.begin();moveIter!=validMoves.end();moveIter++) {
			ChessBoard newBoard=board;
			newBoard.applyMove(*moveIter);
			newBoardValue=boardValue-moveHeuristic(*moveIter);

			v=min(v,alphaBetaSearch(newBoard,newBoardValue,color,depth-1,alpha,beta,true))
			beta=min(beta,v);
			if (beta<=alpha) break;
			return v;
		}
	}
}

float pieceValue(int piece) {
	switch (pieceType(piece)) {
		case QUEEN: return 9;
		case ROOK: return 5;
		case KNIGHT: return 3;
		case BISHOP: return 3;
		case PAWN: return 1;
	}
	return 0;
}

float moveHeuristic(ChessMove &chessMove) {
    float score=pieceValue(chessMove.capture);
    if (chessMove.isCastlingMove()) score+=2;
    else if (chessMove.isCastlingPrevented()) score-=1;
    if (chessMove.isCheck) score+=0.4f;

	int yAdvance=chessMove.to.y-chessMove.from.y;
	if (getColor(chessMove.piece)==WHITE) yAdvance=-yAdvance;
	int xMovesTowardCenter=distanceToCenter(chessMove.from.x)-distanceToCenter(chessMove.to.x);
	score+=yAdvance*0.04f+xMovesTowardCenter*0.04f;
}

int distanceToCenter(int pos) {
	return pos>3?pos-4:3-pos;
}

bool compareMovesHighestScoreFirst(ChessMove &lhs, ChessMove &rhs) {
	//greater than = reverse sort
	return moveHeuristic(lhs)>moveHeuristic(rhs);
}

bool compareMovesLowestScoreFirst(ChessMove &lhs, ChessMove &rhs) {
	return moveHeuristic(lhs)<moveHeuristic(rhs);
}
