//********************************************************
//* Programmers: Holden Madriaga         *
//*                                                      *
//* Date       : 5 December 2022                         *
//* Function   : Create a program to decode and encode   *
//* using a code string, and a cascading ascii grid that *
//* shifts over by one each row. the ascii grid will use *
//* an ascii row of char 32 - 126 and locates encrypted  *
//* letter with a the same range on first row and column *
//* the column is selected with a code string, and the   *
//* row is selected with ascii value of character in what*
//* is to be coded or decoded.                           *
//* use input output files, loop program and clear screen*
//* every loop. allows the user to quit on main menu     *
//*                                                      *
//* Pseudocode:                                          *
//* Initiate ascii table for global use. then inside main*
//* initiate variables and set values if needed, as well *
//* as strings, and stream names.                        *
//* next is to build the ascii table in a series of loops*
//* ( currently looking into more efficient methods of   * 
//* this process. Begin user selection on what they would*
//* like to do. encode, decode or quit. begin loop until *
//* quit. when 1 or 2 request input file and attempt to  *
//* open, if failed increase counter by 1 and when 3,    *
//* quit program.                                        *
//* Get and open output file, then get codestring.       *
//* get total characters in file, then repeat codestring *
//* until it reaches ammount of characters.              *
//* run switch case to determine what function to use.   *
//* clear variables used, repeat until quit.             *
//********************************************************


#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;
char asciiTable[96][96];

void encodeRTN(string codestring, ifstream &openedFile, ofstream &outputFile);
//*******************************************************************************
//* assign local variables and string, then get first character of opened file. *
//* and while not eof and not a new line character(if new line char pass and get*
//* next character.) get code string first char value - 32. and then get first  *
//* of openedFile and its char value - 30. use those numbers to locate encrypted*
//* code in global ascii table. pass to output file, repeat until end of file.  *
//*******************************************************************************

void decodeRTN(string codestring, ifstream &openedFile, ofstream &outputFile);
//*******************************************************************************
//* assign local variables and string, then get first character of opened file. *
//* and while not eof and not a new line character(if new line char pass and get*
//* next character.) get code string first char value - 32. assign a temp char  *
//* using row of your code string char value, and column 0. then cycle through  *
//* every character in row, counting each, until a match is found. set code     *
//* next pass a value in the ascii table 0 for row number and the just          *
//* calculated number as the column number. repeat process until eof.           *
//*******************************************************************************
int main()
{
	int choice, incodeLength = 0;
	int dividestring, remainderstring;
	string inputFilePath;
	string outputFilePath;
	string codeString;
	string tempString;
	ifstream inputFile;
	ofstream outputFile;
	char tempchar = 0;
	int NDXColumn = 0;
	int NDXRow = 0;
	int attempts = 1;

	//Setting up Ascii table array the hard way.
	int tempNum = 0;
	for (NDXColumn = 1; NDXColumn < 96; NDXColumn++) {//First row ASCII
		asciiTable[0][NDXColumn] = char(32 + tempNum);
		tempNum++;
	}
	tempNum = 0;
	for (NDXColumn = 1; NDXColumn < 96; NDXColumn++) {//secound row ASCII
		asciiTable[1][NDXColumn] = char(32 + tempNum);
		tempNum++;
	}
	tempNum = 0;
	for (NDXRow = 1; NDXRow < 96; NDXRow++) { //first column ASCII
		asciiTable[NDXRow][0] = char(32 + tempNum);
		tempNum++;
	}
	for (NDXRow = 2; NDXRow < 96; NDXRow++) {// Build full array
		for (NDXColumn = 1; NDXColumn < 96; NDXColumn++) {
			asciiTable[NDXRow][NDXColumn] = asciiTable[NDXRow - 1][NDXColumn + 1];
		}
		asciiTable[NDXRow][95] = asciiTable[NDXRow - 1][0];
	}
	//finished setting up ascii table the hard way

	cout << "       MENU" << endl;
	cout << "1. Encode" << endl << "2. Decode" << endl << "3. Quit" << endl << "Enter your selection: ";
	cin >> choice;
	while ((choice < 3) && (choice > 0)) {
		cin.ignore();
		cout << "Please type an input file path: ";
		getline(cin, inputFilePath);
		inputFile.open(inputFilePath);

		if (!inputFile.is_open()) {
			while ((!inputFile.is_open()) && (attempts < 3)) {
				attempts++;
				cout << endl;
				cout << "Input file opening failed.\n";
				cout << "File path: " << inputFilePath << endl;
				cout << "Try right clicking file and copying file path, and ensure no double quotes are used." << endl;
				cout << "Please type an input file path: ";
				getline(cin, inputFilePath);
				inputFile.open(inputFilePath);
			}
		}

		if(attempts == 3) {
			exit(0);
		}
		system("cls");
		cout << "Please type an output file: ";
		getline(cin, outputFilePath);
		outputFile.open(outputFilePath);

		cout << "Please type the code string: ";
		getline(cin, codeString);
	
		while (getline(inputFile, tempString)) { // get length to setup repeating codestring
			incodeLength += tempString.length();
		}
		inputFile.clear();
		inputFile.seekg(0,ios::beg);
		tempString.clear();

		dividestring = incodeLength / codeString.length();
		remainderstring = (incodeLength % codeString.length());
		for (int currentChar = 0; currentChar < dividestring; currentChar++) {
			tempString.append(codeString);
		}
		for (int currentChar = 0; currentChar < remainderstring; currentChar++) {
			tempString.push_back(codeString[currentChar]);
		}

		switch (choice) {
		case 1:
			encodeRTN(tempString, inputFile, outputFile);
			inputFile.close();
			outputFile.close();
			break;
		case 2:
			decodeRTN(tempString, inputFile, outputFile);
			inputFile.close();
			outputFile.close();
			break;
		}
		incodeLength == 0;
		attempts == 4; //this allows the user not to worry about getting kicked out after one successful run.
		system("cls");
		cout << "(1) Encode" << endl << "(2) Decode" << endl << "(3) Quit" << endl << "Please enter numerical reference: ";
		cin >> choice;

	}
	cout << "Goodbye.";
}
void encodeRTN(string codestring, ifstream& openedFile, ofstream& outputFile) {
	string tempString;
	char currentCharFile;
	int codeStringIndex = 0;
	int column;
	int row;
	openedFile.get(currentCharFile);
	while (!openedFile.eof()) {
		if (currentCharFile == '\n') {
			outputFile << tempString;
			outputFile << '\n';
			tempString.clear();
			openedFile.get(currentCharFile);
			codeStringIndex = 0;
		}
		row = (int(codestring.at(codeStringIndex)) - 32);
		column = (int(currentCharFile - 30));
		tempString.push_back(asciiTable[row][column]);
		codeStringIndex++;
		openedFile.get(currentCharFile);
	}
	outputFile << tempString;
};
void decodeRTN(string codestring, ifstream& openedFile, ofstream& outputFile) {
	string tempString;
	char currentCharFile;
	char tempChar;
	int tempColumn = 0;
	int codeStringIndex = 0;
	int row;
	openedFile.get(currentCharFile);
	while (!openedFile.eof()) {
		if (currentCharFile == '\n') {
			outputFile << tempString;
			outputFile << '\n';
			tempString.clear();
			openedFile.get(currentCharFile);
			codeStringIndex = 0;
		}
		row = (int(codestring.at(codeStringIndex)) - 32);
		tempChar = (asciiTable[row][tempColumn]);
		while (currentCharFile != tempChar) {
			tempColumn++;
			tempChar = (asciiTable[row][tempColumn]);
		}
		tempString.push_back(asciiTable[0][tempColumn - 1]);
		codeStringIndex++;
		tempColumn = 0;
		openedFile.get(currentCharFile);
	}
	outputFile << tempString;
};