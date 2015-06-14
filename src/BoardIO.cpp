#include "BoardIO.h"

BoardIO::BoardIO()
{
	//ctor
}

BoardIO::~BoardIO()
{
	//dtor
}

void BoardIO::saveBoard(const ChessBoard &board, const char *filename) {
	ofstream outfile;
	outfile.open(filename);
	outfile << "<html>\n";
	addHTMLHeader(outfile);
	outfile << "<body>\n";
	outputBoardAsHTMLTable(board,outfile,false);
	outfile << "\n</body>\n</html>";
	outfile.close();
}

void BoardIO::saveBoards(vector<ChessBoard> &boards, const char *filename) {
	ofstream outfile;
	outfile.open(filename);
	outfile << "<html>\n";
	addHTMLHeader(outfile);
	outfile << "<body>\n";
	for (unsigned int i=0;i<boards.size();i++) outputBoardAsHTMLTable(boards[i],outfile,false);
	outfile << "\n</body>\n</html>";
	outfile.close();
}

void BoardIO::addHTMLHeader(ostream& outfile) {
	outfile << "<head><style>\n";
	outfile << "table {text-align:center;border-spacing:0pt;font-family:'Arial Unicode MS'; display:inline-block; margin-bottom:15px;}\n";
	outfile << ".whitesq {width:40; height: 40; background-color:#eeddbb; border: 1px solid #422B16; font-size: 160%;}\n";
	outfile << ".blacksq {width:40; height: 40; background-color:#cc8844; border: 1px solid #422B16; font-size: 160%;}\n";
	outfile << "</style></head>\n";
}


string BoardIO::getUnicode(int piece) {
	switch (piece) {
		case WHITE+KING: return "&#9812;";
		case WHITE+QUEEN: return "&#9813;";
		case WHITE+ROOK: return "&#9814;";
		case WHITE+BISHOP: return "&#9815;";
		case WHITE+KNIGHT: return "&#9816;";
		case WHITE+PAWN: return "&#9817;";
		case BLACK+KING: return "&#9818;";
		case BLACK+QUEEN: return "&#9819;";
		case BLACK+ROOK: return "&#9820;";
		case BLACK+BISHOP: return "&#9821;";
		case BLACK+KNIGHT: return "&#9822;";
		case BLACK+PAWN: return "&#9823;";
		default: return "";
	}
}

void BoardIO::outputBoardAsHTMLTable(const ChessBoard &board, ostream &output, bool flip) {
	output << "<table>";
	bool isWhite;
	int piece;

	for (int y=0;y<8;y++) {
		output << "<tr><td>"<<(8-y)<<"</td>";
		for (int x=0;x<8;x++) {
			isWhite=(x+y)%2==0;
			if (flip) piece=board.get(7-x,7-y);
			else piece=board.get(x,y);
			output << "<td class=\"" << (isWhite?"whitesq":"blacksq") << "\">" << getUnicode(piece) << "</td>";
		}
		output << "</tr>\n";
	}
	output << "<tr><td></td>";
	for (char c='a';c<='h';c++) output << "<td>" << c << "</td>";
	output << "</tr>\n</table>";
}

string positionString(Position position) {
	ostringstream ss;
	ss << ((char)('a'+position.x)) << (8-position.y);
	return ss.str();
}

string moveString(ChessMove& chessMove) {
	int castlingMove=chessMove.isCastlingMove();
	if (castlingMove==2) return "O-O";
	else if (castlingMove==3) return "O-O-O";
	else {
		ostringstream ss;
		ss << pieceSymbol(chessMove.piece) << positionString(chessMove.from);
		if (chessMove.capture==EMPTY) ss << '-' << positionString(chessMove.to);
		else ss << 'x' << pieceSymbol(chessMove.capture) <<  positionString(chessMove.to);
		if (chessMove.isCheck) ss<< '+';
		return ss.str();
	}
}

char pieceSymbol(int piece) {
	switch (piece) {
		case WHITE+KING: return 'k';
		case WHITE+QUEEN: return 'q';
		case WHITE+ROOK: return 'r';
		case WHITE+BISHOP: return 'b';
		case WHITE+KNIGHT: return 'n';
		case WHITE+PAWN: return 'p';
		case BLACK+KING: return 'K';
		case BLACK+QUEEN: return 'Q';
		case BLACK+ROOK: return 'R';
		case BLACK+BISHOP: return 'B';
		case BLACK+KNIGHT: return 'N';
		case BLACK+PAWN: return 'P';
		default: return ' ';
	}
}

int symbolToPiece(char symbol) {
	switch (symbol) {
		case 'k': return WHITE+KING;
		case 'q': return WHITE+QUEEN;
		case 'r': return WHITE+ROOK;
		case 'b': return WHITE+BISHOP;
		case 'n': return WHITE+KNIGHT;
		case 'p': return WHITE+PAWN;
		case 'K': return BLACK+KING;
		case 'Q': return BLACK+QUEEN;
		case 'R': return BLACK+ROOK;
		case 'B': return BLACK+BISHOP;
		case 'N': return BLACK+KNIGHT;
		case 'P': return BLACK+PAWN;
		default: return EMPTY;
	}
}

ChessBoard parseBoardStr(const string &boardStr) {
	ChessBoard board;

	int x=0;
	int y=0;
	unsigned int i;
	int j;
	char c;
	for (i = 0; i < boardStr.size(); i++) {
		c=boardStr[i];
		if (c=='/') {
			while (x<8) {
				board.setPiece(x,y,EMPTY);
				x++;
			}
			y++;
			x=0;
		} else if (c>='1'&&c<='8') {
			int n=c-'0';
			for (j=0;j<n;j++) {
				board.setPiece(x,y,EMPTY);
				x++;
			}
		} else {
			board.setPiece(x,y,symbolToPiece(c));
			x++;
		}
	}
	return board;
}

void printToConsole(ChessBoard &board) {
	for (int y=0;y<8;y++) {
		for (int x=0;x<8;x++) {
			char c=pieceSymbol(board.get(x,y));
			if  (c==' ') cout<<'.';
			else cout << c;
		}
		cout << '\n';
	}
}

void printBoardStats(ChessBoard &board, bool showWhiteMoves, bool showBlackMoves) {
	if (board.isCheckmate(WHITE)) cout << "Checkmate white";
	else if (board.isCheckmate(BLACK)) cout << "Checkmate black";
	else if (board.isStalemate(WHITE)) cout << "Stalemate white";
	else if (board.isStalemate(BLACK)) cout << "Stalemate black";
	else if (board.isCheck(WHITE)) cout << "Check white";
	else if (board.isCheck(BLACK)) cout << "Check black";
	else cout << "No check";

	const char *castleString[]={"can't castle","can castle queen-side","can castle king-side","can castle both"};
	int castleStringId=board.isCastlingAllowed(WHITE,KING)?2:0+board.isCastlingAllowed(WHITE,QUEEN)?1:0;
	cout << ", White " << castleString[castleStringId];
	castleStringId=board.isCastlingAllowed(BLACK,KING)?2:0+board.isCastlingAllowed(BLACK,QUEEN)?1:0;
	cout << ", Black " << castleString[castleStringId] << "\n";

	vector<ChessMove> moves;
	vector<ChessMove>::iterator moveIter;
	if (showWhiteMoves) {
		board.getMoves(WHITE,moves);
		cout << "White has " << moves.size() << "moves : ";
		for (moveIter=moves.begin();moveIter!=moves.end();moveIter++) {
			cout << moveString(*moveIter) << ", ";
		}
		cout << "\n";
	}

	if (showBlackMoves) {
		board.getMoves(BLACK,moves);
		cout << "Black has " << moves.size() << "moves : ";
		for (moveIter=moves.begin();moveIter!=moves.end();moveIter++) {
			cout << moveString(*moveIter) << ", ";
		}
		cout << "\n";
	}
}

ChessMove stringToMove(ChessBoard &board, const string &moveStr) {
	if (moveStr.size()<5) throw invalid_argument("Invalid move string.");
	int fromX=moveStr[0]-'a';
	int fromY=8-(moveStr[1]-'0');
	int toX=moveStr[3]-'a';
	int toY=8-(moveStr[4]-'0');
	if (fromX<0||fromY<0||toX<0||toY<0||fromX>7||fromY>7||toX>7||toY>7) throw invalid_argument("Invalid move string.");
	return board.getMove(fromX,fromY,toX,toY);
}
