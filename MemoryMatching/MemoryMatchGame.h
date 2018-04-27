
#pragma once

#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

class MemoryMatchGame
{
public:
	MemoryMatchGame();
	~MemoryMatchGame();

	void start();

private:
	void initGrid(int rows, int col);
	void printError(string error);
	void printGrid();
	bool getSelection(int* rowIndex, int* colIndex, string lastSelection, string* error);
	bool checkIfDone();

	//Numer of rows and columns stored in each grid pointer
	int gridRows, gridCols;
	//This grid is what will be displayed to the user.
	//First dimension is rows, second is columns
	string** displayGrid;
	//This grid is what contains all the answers, for reference.
	//First dimension is rows, second is columns
	string** answerGrid;

	int speedSelection;
	int gridSelection;
	string rowColSelection;

	string themeName = "PTBLE"; //Periodic Table
	string themeTerms[50] = 
	{
		"H    ", "He   ", "Li   ", "Be   ", "B    ", "C    ", "N    ", "O    ", "Mg   ", "Al   ",
		"Si   ", "P    ", "S    ", "Cl   ", "Ar   ", "K    ", "Ca   ", "Sc   ", "Ti   ", "V    ",
		"Cr   ", "Mn   ", "Fe   ", "Co   ", "Ni   ", "Cu   ", "Zn   ", "Ga   ", "Ge   ", "As   ",
		"Se   ", "Br   ", "Kr   ", "Rb   ", "Sr   ", "Ac   ", "Ag   ", "Am   ", "Ar   ", "At   ",
		"Au   ", "Ba   ", "Be   ", "Bh   ", "Bi   ", "Bk   ", "Cd   ", "Ce   ", "Cf   ", "Cm   " 
	};
};

