// Connect4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Connect4.h"
#include <Query.h>
#include "Negamax.h"
#include <sstream>

#define ROWS 6
#define COLUMNS 7
#define DEPTH 2

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

//void Connect4::StartGame()
//{
//	Negamax boardSolver = Negamax();
//	string line;
//
//	for (int l = 1; getline(std::cin, line); l++) {
//		Position pos;
//		if (pos.PlacePiece(line) != line.size())
//		{
//			cerr << "Line << " << l << ": Invalid move " << (pos.GetTotalMoves() + 1) << " \"" << line << "\"" << endl;
//		}
//		else
//		{
//			LARGE_INTEGER frequency;
//			LARGE_INTEGER startTime, endTime;
//			double elapsedTime;
//
//			QueryPerformanceFrequency(&frequency);
//			QueryPerformanceCounter(&startTime);
//
//			int score = boardSolver.Solve(pos, -1, 1);
//			//int score = boardSolver.Solve(pos, -Position::WIDTH*Position::HEIGHT / 2, Position::WIDTH*Position::HEIGHT / 2);
//
//			
//			QueryPerformanceCounter(&endTime);
//			elapsedTime = ((endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart) / 1000.0;
//
//			cout << line << " " << score << " " << boardSolver.GetNodeCount() << " " << elapsedTime;
//		}
//		cout << endl;
//	}
//}

void Connect4::StartGame()
{
	Negamax boardSolver = Negamax();

	cout << "Welcome to Connect4!" << endl;
	bool validInput = false;
	for (string line; cout << "Would you like to go first? (y/n)" << endl && getline(cin, line); )
	{
		if (line == "y" || line == "Y" )
		{ 
			validInput = true; 
			aiPlayer = 2;
			humanPlayer = 1;
			break; 
		}
		else if (line == "n" || line == "N")
		{
			validInput = true;
			aiPlayer = 1;
			humanPlayer = 2;
			break;
		}
		cerr << "I'm sorry, that was not an option." << endl;
	}
	if (!validInput) 
	{ 
		cerr << "Premature end of input.\n"; 
	}

	//PrintBoard();

	string line;
	for (int l = 1; getline(std::cin, line); l++) {
		Position pos;
		if (pos.PlacePiece(line) != line.size())
		{
			cerr << "Line << " << l << ": Invalid move " << (pos.GetTotalMoves() + 1) << " \"" << line << "\"" << endl;
		}
		else
		{
			LARGE_INTEGER frequency;
			LARGE_INTEGER startTime, endTime;
			double elapsedTime;

			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&startTime);

			int score = boardSolver.Solve(pos, -1, 1);
			//int score = boardSolver.Solve(pos, -Position::WIDTH*Position::HEIGHT / 2, Position::WIDTH*Position::HEIGHT / 2);


			QueryPerformanceCounter(&endTime);
			elapsedTime = ((endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart) / 1000.0;

			cout << line << " " << score << " " << boardSolver.GetNodeCount() << " " << elapsedTime;
		}
		cout << endl;
	}
	//CreateMap();

	//TakeTurn();
}



void Connect4::PrintBoard()
{
	for (int i = Position::WIDTH - 1; i >= 0; --i)
	{
		for (int j = 0; j < Position::HEIGHT; ++j)
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
		PlacePiece(columnChoice);
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
	if (aiPlayer == 1)
	{
		return minimaxLogic.GetMove(DEPTH, gameBoard, true);
	}
	else
	{
		return minimaxLogic.GetMove(DEPTH, gameBoard, false);
	}
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

	if (winner != 0)
	{
		cout << "Game over! Player ";
		cout << winner;
		cout << " won!" << endl;
		cout << "Enter character to quit.";
		char pauseChar;
		cin >> pauseChar;
		exit(0);
	}
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
						return pieceOwner;
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
						return pieceOwner;
					}
				}
			}
		}
	}

	return 0;
}