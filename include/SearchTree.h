#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <algorithm>
#include <vector>
#include <limits>
#include "ChessMove.h"
#include "ChessBoard.h"

struct SearchValue {
	ChessMove chessMove;
	float score;

	SearchValue(ChessMove _chessMove, float _score) : chessMove(_chessMove), score(_score) {};
};

class SearchTree {
	public:
		SearchTree();
		virtual ~SearchTree();

		ChessMove findBestMove(ChessBoard &board, int color, int depth);
		SearchValue alphaBetaSearch(ChessBoard &board, float boardValue, int color, int depth, float alpha, float beta, bool isPlayer);
	protected:
	private:
};

float moveHeuristic(ChessMove &chessMove);
int distanceToCenter(int pos);
bool compareMovesHighestScoreFirst(ChessMove &lhs, ChessMove &rhs);
bool compareMovesLowestScoreFirst(ChessMove &lhs, ChessMove &rhs);

#endif // SEARCHTREE_H
