#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/ChessBoard.h"
#include "include/BoardIO.h"
#include "include/Tests.h"

using namespace std;



int main() {
	ChessBoard board;

	//board=parseBoardStr("K/3R//////3k");
	board.setup();
	printToConsole(board);
	cout << "Srch : " << pieceSymbol(board.searchPiece(3,7,0,-1)) << "\n";
	printBoardStats(board,true,true);

	//aiGame();
	abstractGame(consolePlayer,aiPlayer);

    return 0;
}


