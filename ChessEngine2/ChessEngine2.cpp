#include <iostream>
#include <vector>

struct Piece
{
	int8_t coordX;
	int8_t coordY;

	//int Pvalue;//Value of piece
	int Pname;//Name of piece

	Piece(int x, int y, int name) : coordX(x), coordY(y), Pname(name) {}

	virtual ~Piece() {}

	virtual std::pair<int8_t, int8_t> BestMove(std::vector<std::vector<int8_t>> v)
	{
		int8_t mxV = INT8_MIN;
		std::pair<int8_t, int8_t> res;

		for (const std::vector<int8_t> i : v)
		{
			if (mxV < i[2])
			{
				res = { i[0], i[1] };
			}
		}
		return res;
	}

	virtual std::vector<std::vector<int8_t>> PossibleMoves() {}
};

struct Board
{
	//По расценке фигур 10 - Король, 9 - Дама, 5 - Ладья, 4 - Слон, 3 - Конь, 1 - Пешка черные также, но с отр значениями
	int8_t board[8][8] = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
	};
	int8_t valueBoard[8][8] = {
		{0, 0, 0, 0,  0, 0, 0, 0},
		{6, 6, 5, 5,  5, 5, 6, 6},
		{0, 0, 6, 6,  6, 6, 0, 0},
		{0, 0, 0, 10, 10, 0, 0, 0},

		{0, 0, 0, 8,   8, 0, 0, 0},
		{3, 0, 0, -5, -5, 0, 0, 3},	
		{3, 4, 4, -8, -8, 4, 4, 3},
		{0, 0, 0, 0,   0, 0, 0, 0},
	};

	void PrintBoard()
	{
		std::cout << "   a  b  c  d  e  f  g  h\n";
		std::cout << "  + - - - - - - - - - - - +\n";

		for (int i = 0; i <= 7; ++i)
		{
			std::cout << i + 1 << " |";
			for (int j = 0; j < 8; ++j)
			{
				if (board[i][j] >= 0) std::cout << " ";
				std::cout << static_cast<int>(board[i][j]) << " ";
			}
			std::cout << '\n';
		}

		std::cout << "  + - - - - - - - - - - - +\n";
		std::cout << "   a  b  c  d  e  f  g  h\n";
	}
};

struct King: Piece
{
private:
	Board& gameBoard;
	bool isBlack;
public:
	const int moves[8][2] = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1},          {0, 1},
		{1, -1},  {1, 0},  {1, 1}
	};

	King(bool isB, Board& b) : Piece(4, isB ? 7 : 0, isB ? -10 : 10), gameBoard(b), isBlack(isB) {}
	King(bool isB, int x, int y, Board& b) : Piece(x, y, isB ? -10 : 10), gameBoard(b), isBlack(isB)
	{
		gameBoard.board[x][y] = 10;
	}

	/*void MakeMove()
	{
		std::pair<int, int> tmp = BestMove(PossibleMoves());
		gameBoard.board[coordX][coordY] = 0;
		coordX = tmp.first; coordY = tmp.second;
		gameBoard.board[coordX][coordY] = 10;
	}*/

	//НУЖНО ПЕРЕДЕЛАТЬ ПОД ВИРТУАЛЬНУЮ ФУНКЦИЮ
	/*std::vector<std::pair<std::pair<int8_t, int8_t>, int8_t>> PossibleMoves()
	{
		std::vector<std::pair<std::pair<int8_t, int8_t>, int8_t>> v;
		for (int i = 0; i < 8; ++i)
		{
			int8_t newX = this->coordX + moves[i][0];
			int8_t newY = this->coordY + moves[i][1];
			if ((newX > -1 && newX < 8) && (newY > -1 && newY < 8))
			{
				if(gameBoard.board[newX][newY] == 0 || (gameBoard.board[coordX][coordY] * gameBoard.board[newX][newY] < 0))
					v.push_back({ { newX, newY }, 0 });
			}
		}
		return v;
	}*/

	std::pair<int8_t, int8_t> BestMove(std::vector<std::pair<std::pair<int8_t, int8_t>, int8_t>> posM)
	{
		int8_t mx = INT8_MIN;
		std::pair<int8_t, int8_t> res;
		for(int8_t i = 0; i < posM.size(); ++i)
		{
			posM[i].second += gameBoard.board[posM[i].first.first][posM[i].first.second] + gameBoard.valueBoard[posM[i].first.first][posM[i].first.second];
			if (mx <= std::max(mx, posM[i].second))
			{
				res = posM[i].first;
				mx = posM[i].second;
			}
				
		}
		return res;
	}

};

struct Pawn: Piece
{
private:
	Board& gameBoard;
	bool isBlack;
public:
	//2 это название пешки, которая не ходила
	Pawn(bool isBlack, int8_t coordX, int8_t coordY, Board b) : Piece(coordX, coordY, isBlack ? 2 : -2), gameBoard(b), isBlack(isBlack) {}

	int8_t moves[2][2] = { {2, 0}, {1, 0} };
	int8_t takes[2][2] = { {1, -1}, {1, 1} };

	std::vector<std::vector<int8_t>> PossibleMoves()
	{
		std::vector<std::vector<int8_t>> res;//В общем делаем тип данных для пары координат и оценки хода
		for (const int8_t* i : moves)
		{
			int8_t newX = coordX + isBlack ? i[0] : -i[0];
			int8_t newY = coordY + i[1];
			if (gameBoard.board[newX][newY] == 0)
			{
				res.push_back({newX, newY, gameBoard.valueBoard[newX][newY]});
			}
		}

		for (const int8_t* i : takes)
		{
			int8_t newX = coordX + isBlack ? i[0] : -i[0];
			int8_t newY = coordY + i[1];
			if ((isBlack && gameBoard.board[newX][newY] < 0) || (!isBlack && gameBoard.board[newX][newY] > 0))
			{
				res.push_back({newX, newY, gameBoard.valueBoard[newX][newY]});
			}
		}
	}
};

struct Bishop : Piece
{
private:
	Board& gameBoard;
	bool isBlack;

	struct Node
	{
		Node* right;
		Node* left;
	};
public:
	Bishop(bool isBlack, int8_t coordX, int8_t coordY, Board b) : Piece(coordX, coordY, isBlack ? 2 : -2), gameBoard(b), isBlack(isBlack) {}

	int8_t moves[4][2] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };

	std::vector<std::vector<int8_t>> PossibleMoves(int x, int y)
	{
		std::vector<std::vector<int8_t>> res;
		for (int i = 0; i < 4; ++ i)
		{
			int8_t newX = x + moves[i][0];
			int8_t newY = y + moves[i][1];
			while (gameBoard.board[moves[i][0] + newX][moves[i][1] + newY] == 0)
			{
				res.push_back({ newX, newY, gameBoard.valueBoard[newX][moves[i][1] + newY] });
				newX += moves[i][0]; newY += moves[i][1];
			}
			if ((gameBoard.board[newX][newY] <= 0 && isBlack) //Если преграждает путь своя фигура
				|| (gameBoard.board[newX][newY] >= 0 && !isBlack))
				continue;


			if ((gameBoard.board[newX][newY] > 0 && isBlack) || (gameBoard.board[newX][newY] < 0 && !isBlack))
			{

				res.push_back({ newX, newY, 0 });
			}
		}
	}

	Node Deep(int8_t res)
	{

	}
};

int main()
{
	Board b;
	King k(0, 0, 3, b);
}