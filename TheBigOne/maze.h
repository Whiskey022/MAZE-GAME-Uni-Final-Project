#pragma once

#include <ctime>


class Maze{
public:
	//constructor loads maze and asks for maze level number from as an argument 
	Maze(string level);
	//printing maze to the screen
	void PrintMaze();
	//function to move player, returns false if its end game
	bool ChangePlayerPosition(int xChange, int yChange);

private:
	//function to get maze values from a text file
	void GetMaze();
	//function called when maze is finished
	//outputs congratulations and records time
	void GameWon();
	//asks for username for the score
	//and saves time to a text file with other scores
	void SaveTime();

protected:
	//vector to print the actual maze and player position
	vector<vector<char>> mazeValues;
	//integers for player position
	int xPosition;
	int yPosition;
	//values to record maze completion time
	std::clock_t start;
	double timeDuration;
	//selected maze level
	string mazeLevel;

};
