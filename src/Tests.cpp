#include "Tests.h"

Tests::Tests()
{
	//ctor
}

Tests::~Tests()
{
	//dtor
}

void abstractGame(ChessMove (*whitePlayer)(ChessBoard &,int),ChessMove (*blackPlayer)(ChessBoard &,int)) {
	ChessBoard board;
	board.setup();
	SearchTree ai;
	ChessMove selectedMove;
	unsigned int i;
	vector<ChessMove> moves;

	BoardIO boardWriter;
	vector<ChessBoard> boards;
	int color=WHITE;
	for (i=0;i<16;i++) {
		boards.push_back(board);
		if (color==WHITE) selectedMove=(*whitePlayer)(board,color);
		else selectedMove=(*blackPlayer)(board,color);
		board.getMoves(color,moves);
		board.applyMove(selectedMove);
		cout << moveString(selectedMove) << "\n";
		color=getOpponent(color);
	}
	boardWriter.saveBoards(boards, "C:\\Users\\sam\\Documents\\cpp\\chess.html");
}

ChessMove aiPlayer(ChessBoard &board, int color) {
	static SearchTree ai;
	return ai.findBestMove(board,color,2);
}
ChessMove randomPlayer(ChessBoard &board, int color) {
	vector<ChessMove> moves;
	board.getMoves(color,moves);
	int i=rand()%moves.size();
	return moves[i];
}
ChessMove consolePlayer(ChessBoard &board, int color) {
	string inputstr;
	ChessMove chessMove;
	printToConsole(board);
	while (true) {
		cout << '\n';
		cin >> inputstr;

		try {
			chessMove=stringToMove(board,inputstr);
			if (getColor(chessMove.piece)==color&&board.validateFromList(chessMove)) {
				return chessMove;
			} else cout << "Invalid move!\n";
		} catch (invalid_argument &ex) {
			cout << "Exception : " << ex.what() << '\n';
		}
	}
}

void consoleGame() {
	string inputstr;
	ChessMove chessMove;
	ChessBoard board;
	board.setup();
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
}

