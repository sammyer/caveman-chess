#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <algorithm>
#include <vector>

struct SearchNode {
	ChessMove chessMove;
	float score;
	vector<SearchNode> children;
};

class SearchTree {
	public:
		SearchTree();
		virtual ~SearchTree();

		ChessMove findBestMove(ChessBoard &board, int color, int depth);
		float alphaBetaSearch(ChessBoard &board, int color, int depth, int alpha, int beta, bool isPlayer);
	protected:
	private:
};

float moveHeuristic(ChessMove &chessMove);
int distanceToCenter(int pos);
bool compareMovesHighestScoreFirst(ChessMove &lhs, ChessMove &rhs;
bool compareMovesLowestScoreFirst(ChessMove &lhs, ChessMove &rhs;

#endif // SEARCHTREE_H
