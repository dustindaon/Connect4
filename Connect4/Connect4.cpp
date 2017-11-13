// Connect4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Connect4.h"

#define ROWS 6
#define COLUMNS 7

using namespace std;

int main(int ars, char* argv[])
{
	Connect4 newGame = Connect4();

    return 0;
}

Connect4::Connect4()
{
	StartGame();
}

Connect4::~Connect4()
{

}

void Connect4::CreateMap()
{
	gameBoard = vector<vector<int>>(COLUMNS, vector<int>(ROWS, 0));
}

void Connect4::StartGame()
{
	CreateMap();
	cout << "Welcome to Connect4!" << endl;
	cout << "Would you like to go first? (y/n)" << endl;

	// TODO: Fix this.
	char turnSelect = ' ';
	cin >> turnSelect;
	while (turnSelect != 'y' && turnSelect != 'n' && turnSelect != 'Y' && turnSelect != 'N')
	{
		cout << "I'm sorry, that was not an option." << endl;
		cout << "Would you like to go first? (y/n)" << endl;
		cin >> turnSelect;
	}

	PrintBoard();
	if (turnSelect == 'y' || turnSelect == 'Y')
	{
		aiPlayer = 2;
		humanPlayer = 1;
	}
	else if (turnSelect == 'n' || turnSelect == 'N')
	{
		aiPlayer = 1;
		humanPlayer = 2;
	}

	TakeTurn();
}

void Connect4::PrintBoard()
{
	for (int i = ROWS - 1; i >= 0; --i)
	{
		for (int j = 0; j < COLUMNS; ++j)
		{
			int playerPiece = gameBoard[j][i];
			char* output = "|   ";
			if (playerPiece == 1)
			{
				output = "| X ";
			}
			else if (playerPiece == 2)
			{
				output = "| O ";
			}

			cout << output;
		}

		cout << "|" << endl;
	}

	cout << endl << endl;
}

void Connect4::TakeTurn()
{
	int columnChoice = 0;
	if (currentPlayer == humanPlayer)
	{
		cout << "Select which column to place your piece in: " << endl;
		cin >> columnChoice;

		// TODO: Loop through this until they make the right choice.
		bool validChoice = false;

		if (columnChoice >= COLUMNS || columnChoice < 0)
		{
			cout << "That column doesn't exist. Please select which column to place your piece in: " << endl;
			cin >> columnChoice;
		}
		else if (gameBoard[columnChoice][ROWS - 1] != 0)
		{
			cout << "That row is full. Please select which column to place your piece in: " << endl;
			cin >> columnChoice;
		}

		PlacePiece(columnChoice);
	}
	else
	{
		columnChoice = TakeAITurn();
		cout << "           AI Move" << endl;
		//PlacePiece(columnChoice);
	}

	EndTurn();
}

void Connect4::PlacePiece(int column)
{
	for (int i = 0; i < ROWS; ++i)
	{
		if (gameBoard[column][i] == 0)
		{
			gameBoard[column][i] = currentPlayer;
			return;
		}
	}
}

int Connect4::TakeAITurn()
{
	return 0;
}

void Connect4::EndTurn()
{
	if (currentPlayer == 1)
	{
		currentPlayer = 2;
	}
	else
	{
		currentPlayer = 1;
	}

	PrintBoard();
	int winner = CheckIfGameOver();

	cout << winner << endl;
	TakeTurn();
}

int Connect4::CheckIfGameOver()
{
	int pieceOwner = 0;
	int piecesInARow = 0;
	for (int i = 0; i < COLUMNS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			if (gameBoard[i][j] != 0)
			{
				pieceOwner = gameBoard[i][j];

				piecesInARow = 0;
				// Check for diagonal win (bottom left to top right)
				int y = j - 3;
				for (int x = i-3; x < i+3; ++x, ++y)
				{
					if (x < 0 || y < 0 || x > COLUMNS - 1 || y > ROWS - 1)
					{
						continue;
					}
					if (gameBoard[x][y] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						return pieceOwner;
					}
				}

				piecesInARow = 0;
				// Check for diagonal win (top left to bottom right)
				y = j + 3;
				for (int x = i - 3; x < i + 3; ++x, --y)
				{
					if (x < 0 || y < 0 || x > COLUMNS - 1 || y > ROWS - 1)
					{
						continue;
					}
					if (gameBoard[x][y] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						return pieceOwner;
					}
				}

				piecesInARow = 0;
				// Check for vertical win
				for (int x = j - 3; x < j + 3; ++x)
				{
					if (x < 0 || x > ROWS - 1)
					{
						continue;
					}
					if (gameBoard[i][x] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						//return pieceOwner;
					}
				}

				piecesInARow = 0;
				// Check for horizontal win
				for (int x = i - 3; x < i + 3; ++x)
				{
					if (x < 0  || x > COLUMNS - 1)
					{
						continue;
					}
					if (gameBoard[x][j] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						//return pieceOwner;
					}
				}
			}
		}
	}

	return 0;
}