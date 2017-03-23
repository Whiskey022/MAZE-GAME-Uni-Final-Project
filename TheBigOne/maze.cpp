#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>

using namespace std;

#include "maze.h"
#include "timestruct.h"


//constructor loads maze and asks for maze level number from as an argument 
Maze::Maze(string level) {
	xPosition = 0;
	yPosition = 0;
	mazeLevel = level;
	GetMaze();

	//maze timer started
	start = clock();
}
//printing maze to the screen
void Maze::PrintMaze() {

	//clears console screen
	system("cls");


	//loop for 2d vector (i for rows, j for collumns)
	for (int i = 0; i < mazeValues.size(); i++) {
		for (int j = 0; j < mazeValues[i].size(); j++) {

			//if maze value is 0 or 2, print whitespace
			if (mazeValues[i][j] == '0' || mazeValues[i][j]=='2')
				cout << ' ';

			//if maze value is, print a wall
			else if (mazeValues[i][j] == '1') {
				cout << char(177);
			}

			//if maze value is 3, print a player
			else if (mazeValues[i][j] == '3') {
				cout << char(254);
				yPosition = i;
				xPosition = j;
			}
		}
		cout << endl;

		}

	if (xPosition == 0 && yPosition == 0) {
		cout << "Error, player position not found" << endl;
		getch();
	}
}

//function to move player
bool Maze::ChangePlayerPosition(int xChange, int yChange) {
	//if changed position wont be at a wall, move
	if (xPosition + xChange > 0 && yPosition + yChange > 0) {
		//if position is end game, move and print victory output
		if (mazeValues[yPosition + yChange][xPosition + xChange] == '2') {
			GameWon();
			return false;
		}
		//if move its not end game or a wall, move and print out the maze again
		else if (mazeValues[yPosition + yChange][xPosition + xChange] != '1') {

			//previous position turned into whitespace value
			mazeValues[yPosition].at(xPosition) = '0';
			//new position turned into a player value
			mazeValues[yPosition + yChange][xPosition + xChange] = '3';
			//maze is printed
			PrintMaze();
			return true;
		}
	}
	else
		return true;

}

//function to get maze values from a text file
void Maze::GetMaze() {

	ifstream fileInput;

	string level = "maze_" + mazeLevel + ".txt";

	//try finding file to read from
	fileInput.open(level, ios::in);

	fileInput.clear();
	fileInput.seekg(0, ios::beg);

	//while not the end of the file, read values and to the mazeValues vector
	for (int i = 0; !fileInput.eof();i++) {
		//temporary vector to add rows to our mazeValues vector
		//and temporary string value to read a row
		vector<char> rowVector;
		string stringRow;

		//reading row, and adding string's characters to a temporary vector
		getline(fileInput, stringRow);
		for (int j = 0; j < stringRow.size(); j++) {
			rowVector.push_back(stringRow[j]);
		}

		//temporary vector added to the main one
		mazeValues.push_back(rowVector);

		
	}
	fileInput.close();
}
//print when finish a maze
void Maze::GameWon() {
	//maze time recorded
	timeDuration = (clock() - start) / (double)CLOCKS_PER_SEC;

	system("cls");
	
	//gets victory congratulations from a text file, prints it on screen
	ifstream victoryInput("victory.txt");
	string temp;
	while (!victoryInput.eof()) {
		getline(victoryInput, temp);
		cout << temp << endl;
	}
	getch();

	SaveTime();
}


//asks for username for the score
//and saves time to a text file with other scores
void Maze::SaveTime() {
	//asking to provide a user name for the recorded time
	system("cls");
	cout << "Your time was: " << timeDuration << " seconds." << endl;
	cout << "Provide a user name: " << endl;
	cout << "> ";
	string userName;
	cin >> userName;

	//filestream to read already saved time scores
	//data will be put into a TimeStruct vector
	string file = "times_" + mazeLevel + ".txt";
	ifstream timeFileInput(file);
	vector<TimeStruct> timeStruct;

	//reading data loop
	TimeStruct tempTime;
	for (int i = 0; !timeFileInput.eof(); i++) {
		char temp;
		tempTime.userName = "";
		timeFileInput >> tempTime.position;
		timeFileInput >> temp;
		getline(timeFileInput, tempTime.userName, '|');
		timeFileInput >> tempTime.time;
		//added only if succesfully read data
		if (tempTime.userName != "") 
			timeStruct.push_back(tempTime);
		if (timeFileInput.eof())
			break;
	}
	timeFileInput.close();

	//new time score
	tempTime.userName = userName;
	tempTime.time = timeDuration;

	//loop to insert new score into appropriate position
	bool timeInserted = false;
	for (int i = 0; i < timeStruct.size(); i++) {
		if (timeStruct[i].time > tempTime.time) {
			tempTime.position = i+1;
			timeStruct.insert(timeStruct.begin()+i, tempTime);
			timeInserted = true;
			//when new time inserted, all further scores changes position by 1
			for (int j = i + 1; j < timeStruct.size(); j++) {
				timeStruct[j].position++;
			}
			break;
		}
	}
	//if time was not inserted yet (slower times not found), insert into last position
	if (!timeInserted) {
		tempTime.position = timeStruct.size()+1;
		timeStruct.push_back(tempTime);
	}

	//rewriting times.txt with new scores
	ofstream timeFileOutput(file);
	timeFileOutput.clear();
	for (int i = 0; i < timeStruct.size(); i++) {
		timeFileOutput << timeStruct[i].position << "|" << timeStruct[i].userName << "|" << timeStruct[i].time << endl;
	}
	timeFileOutput.close();
	
}