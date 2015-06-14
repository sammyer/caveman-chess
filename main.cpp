#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/ChessBoard.h"
#include "include/BoardIO.h"

using namespace std;



int main() {
	ChessBoard board;

	//board=parseBoardStr("K/3R//////3k");
	board.setup();
	printToConsole(board);
	cout << "Srch : " << pieceSymbol(board.searchPiece(3,7,0,-1)) << "\n";
	printBoardStats(board,true,true);

	string inputstr;
	ChessMove chessMove;
	for (int i=0;i<15;i++) {
		cout << '\n';
		cin >> inputstr;
		if (inputstr=="q") break;
		try {
			chessMove=stringToMove(board,inputstr);
			if (board.validateFromList(chessMove)) {
				board.applyMove(chessMove);
				printToConsole(board);
				printBoardStats(board,true,true);
			} else cout << "Invalid move!\n";
		} catch (invalid_argument &ex) {
			cout << "Exception : " << ex.what() << '\n';
		}
	}

    return 0;
}


