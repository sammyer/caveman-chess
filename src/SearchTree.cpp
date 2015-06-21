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
	float INFINITY=numeric_limits<float>::infinity();
	return alphaBetaSearch(board,0,color,depth,-INFINITY,INFINITY,true).chessMove;
}

SearchValue SearchTree::alphaBetaSearch(ChessBoard& board,float boardValue, int color, int depth, float alpha, float beta, bool isPlayer) {
	if (depth==0) return SearchValue(ChessMove(),boardValue);

	vector<ChessMove> validMoves;
	board.getMoves(isPlayer?color:getOpponent(color),validMoves);
	if (validMoves.size()==0) return SearchValue(ChessMove(),boardValue);
	sort(validMoves.begin(),validMoves.end(),compareMovesHighestScoreFirst);

	float score=0;
	float newBoardValue;
	ChessMove *chosenMove=NULL;
	bool isFirst=true;
	if (isPlayer) {
		for (vector<ChessMove>::iterator moveIter=validMoves.begin();moveIter!=validMoves.end();moveIter++) {
			ChessBoard newBoard=board;
			newBoard.applyMove(*moveIter);
			newBoardValue=boardValue+moveHeuristic(*moveIter);

			float childScore=alphaBetaSearch(newBoard,newBoardValue,color,depth-1,alpha,beta,false).score;
			if (isFirst||childScore>score) {
				score=childScore;
				chosenMove=&(*moveIter);
				isFirst=false;
			}
			alpha=max(alpha,score);
			if (beta<=alpha) break;
		}
	} else {
		for (vector<ChessMove>::iterator moveIter=validMoves.begin();moveIter!=validMoves.end();moveIter++) {
			ChessBoard newBoard=board;
			newBoard.applyMove(*moveIter);
			newBoardValue=boardValue-moveHeuristic(*moveIter);

			float childScore=alphaBetaSearch(newBoard,newBoardValue,color,depth-1,alpha,beta,true).score;
			if (isFirst||childScore<score) {
				score=childScore;
				chosenMove=&(*moveIter);
				isFirst=false;
			}
			beta=min(beta,score);
			if (beta<=alpha) break;
		}
	}
	return SearchValue(*chosenMove,score);
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

	return score;
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
