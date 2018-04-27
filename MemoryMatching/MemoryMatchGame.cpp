
#include "stdafx.h"
#include "MemoryMatchGame.h"

MemoryMatchGame::MemoryMatchGame() { }
MemoryMatchGame::~MemoryMatchGame() { }

void MemoryMatchGame::start()
{
	while (true)
	{
		system("CLS");
		cout << endl;
		cout << "How good is your memory? Select a difficulty and find out!" << endl << endl;

		//Grid size input loop
		while (true)
		{
			cout << "Please choose your grid: " << endl;
			cout << "1. 4 x 4 grid (Easy)" << endl;
			cout << "2. 6 x 6 grid (Medium)" << endl;
			cout << "3. 8 x 8 grid (Hard)" << endl << endl;
			cin >> gridSelection;

			//Set the grid size
			switch (gridSelection)
			{
			case 1:
				initGrid(4, 4);
				break;
			case 2:
				initGrid(6, 6);
				break;
			case 3:
				initGrid(8, 8);
				break;
			default:
				//Not a correct grid size value
				printError("Not a proper grid size selection.");
				continue;
			}
			break;
		}

		//Speed input loop
		while (true)
		{
			cout << "Please choose your speed: " << endl << endl;
			cout << "1. 6 Second Intervals (Easy)" << endl;
			cout << "2. 4 Second Intervals (Medium)" << endl;
			cout << "3. 2 Second Intervals (Hard)" << endl << endl;
			cin >> speedSelection;

			//Is this a correct speed value?
			if (speedSelection < 1 || speedSelection > 3)
			{
				//Nope.
				printError("Not a proper speed selection.");
			}
			else
			{
				//Change input to actual interval for convenience, as seconds
				//(4 - 1) * 2 = (3 * 2) = 6
				//(4 - 2) * 2 = (2 * 2) = 4
				//(4 - 3) * 2 = (1 * 2) = 2
				speedSelection = (4 - speedSelection) * 2;
				break;
			}
		}

		//Start game timer
		time_t startTime = time(0);

		//Main game loop
		while (true)
		{
			//Player's grid selection indices
			int colIndex1, rowIndex1;
			string errorMessage;

			//Initial grid print
			printGrid();

			//Input loop 1
			while (true)
			{
				if (getSelection(&rowIndex1, &colIndex1, "", &errorMessage))
				{
					//No problems occurred, end input loop
					break;
				}
				else
				{
					printGrid();
					if (!errorMessage.empty())
						cout << errorMessage << endl;
				}
			}

			displayGrid[rowIndex1][colIndex1] = answerGrid[rowIndex1][colIndex1];
			printGrid();

			int colIndex2, rowIndex2;

			//Input loop 2
			while (true)
			{
				if (getSelection(&rowIndex2, &colIndex2, answerGrid[rowIndex1][colIndex1], &errorMessage))
				{
					//No problems occurred, end input loop
					break;
				}
				else
				{
					printGrid();
					if (!errorMessage.empty())
						cout << errorMessage << endl;
				}
			}

			displayGrid[rowIndex2][colIndex2] = answerGrid[rowIndex2][colIndex2];
			printGrid();

			if (displayGrid[rowIndex1][colIndex1].compare(displayGrid[rowIndex2][colIndex2]) != 0)
			{
				cout << "No match.";
				for (int i = 0; i < speedSelection; ++i)
				{
					_sleep(1000);
					cout << ".";
				}
				cout << endl;
				displayGrid[rowIndex1][colIndex1] = themeName;
				displayGrid[rowIndex2][colIndex2] = themeName;

				//Grid will be printed again on next loop
			}
			else
			{
				cout << "It's a match!" << endl;
				_sleep(2000); //Sleep for 2 seconds so the user can read the message
				if (checkIfDone())
					break;
			}
		}

		cout << "You win!" << endl;
		double elapsedSeconds = difftime(time(0), startTime);
		cout << "Game took " << elapsedSeconds << " seconds." << endl;
		cout << "Play again? (Y/N)" << endl;

		char playAgain;
		cin >> playAgain;

		if (playAgain != 'Y' && playAgain != 'y') //Assume no for any other input.
			break;
	}
}

void MemoryMatchGame::initGrid(int rows, int cols)
{
	gridRows = rows;
	gridCols = cols;

	//Init rand with new seed based on current time
	srand(time(0));

	//Shuffle terms so they are in a random new order
	random_shuffle(&themeTerms[0], &themeTerms[50]);

	int gridTotal = rows * cols;
	int termCount = gridTotal / 2;
	int termIndex = 0;

	//Create a random remap table so term locations aren't predictable
	int* termRemap = new int[gridTotal];
	int x = 0;
	for (int i = 0; i < termCount; i++)
	{
		termRemap[x++] = i;
		termRemap[x++] = i;
	}
	random_shuffle(&termRemap[0], &termRemap[gridTotal]);

	displayGrid = new string*[rows];
	answerGrid = new string*[rows];
	for (int r = 0; r < rows; r++)
	{
		displayGrid[r] = new string[cols];
		answerGrid[r] = new string[cols];
		for (int c = 0; c < cols; c++)
		{
			displayGrid[r][c] = themeName;
			answerGrid[r][c] = themeTerms[termRemap[termIndex++]];
		}
	}
}

void MemoryMatchGame::printError(string error)
{
	//Clear the screen
	system("CLS");
	//Print error message
	cout << error << endl << endl;
	//Reset cin
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

void MemoryMatchGame::printGrid()
{
	system("CLS");
	cout << endl;
	for (int r = 0; r <= gridRows; r++)
	{
		for (int c = 0; c <= gridCols; c++)
		{
			if (r == 0)
			{
				if (c != 0)
				{
					cout << " " << string(1, ('A' + (c - 1))) << "     "; //7 chars total width
				}
				else
				{
					cout << "   "; //3 chars total width
				}
			}
			else
			{
				if (c == 0)
				{
					cout << " " << string(1, ('1' + (r - 1))) << " "; //3 chars total width
				}
				else
				{
					cout << " " << displayGrid[r - 1][c - 1] << " "; //7 chars total width
				}
			}
		}

		//End row with newline
		cout << endl;
	}

	//End grid with newline padding
	cout << endl;
}

bool MemoryMatchGame::getSelection(int* rowIndex, int* colIndex, string lastSelection, string* error)
{
	cout << "Please select a row (#) and column (letter). Example: 2A" << endl;

	if (!lastSelection.empty())
		cout << "Last pick: " << lastSelection << endl;

	cin >> rowColSelection;

	if (rowColSelection.length() < 2)
	{
		*error = "Input not long enough.";
		return false;
	}

	//1 in ascii is 0x31
	*rowIndex = (int)rowColSelection[0] - 0x31;
	if (*rowIndex < 0 || *rowIndex >= gridRows)
	{
		*error = "Not a valid row index.";
		return false;
	}

	//A in ascii is 0x41, a is 0x61
	*colIndex = (int)rowColSelection[1];
	if (*colIndex >= 0x61)
		*colIndex -= 0x61;
	else
		*colIndex -= 0x41;
	if (*colIndex < 0 || *colIndex >= gridCols)
	{
		*error = "Not a valid column index.";
		return false;
	}

	if (displayGrid[*rowIndex][*colIndex].compare(themeName) != 0)
	{
		*error = "Grid selection is already visible.";
		return false;
	}

	return true;
}

bool MemoryMatchGame::checkIfDone()
{
	for (int r = 0; r < gridRows; r++)
	{
		for (int c = 0; c < gridCols; c++)
		{
			if (displayGrid[r][c].compare(themeName) == 0)
			{
				return false;
			}
		}
	}
	return true;
}
