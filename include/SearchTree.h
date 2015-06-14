#ifndef SEARCHTREE_H
#define SEARCHTREE_H


class SearchTree
{
	public:
		SearchTree();
		virtual ~SearchTree();

		ChessMove findBestMove(ChessBoard &board);
	protected:
	private:
};

#endif // SEARCHTREE_H
