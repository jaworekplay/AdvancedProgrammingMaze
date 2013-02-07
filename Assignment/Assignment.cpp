// Assignment.cpp : Defines the entry point for the console application.
//Lukasz Banach BAN11118219 CO2451 | Assignment 1 "Maze"
#pragma once
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <iomanip>

#include "Game_Data.h"

#define NOT_TEST 0


int _tmain(int	argc, _TCHAR* argv[])
{
	//initialising varaibles used for the game
	int decision			= 0, 
		currentPosition[2]	= {0,0};
	int oldPosition[2]		= {0,0};
	int lastDecision		= 0;
	Vars maze[arraySize][arraySize];
	PlayerInformation player;
	vectors vec;
	int dec;
	//char century[100][365][24][60][60];
	//////////////////////////////////////////
	//createData(maze,player);
	//cout << maze[3][2].left[0] << maze[3][2].left[1] << maze[3][2].right[0] << maze[3][2].right[1] << endl;
	//getch();
	//learn(maze);
	//system("cls");
	cout << "Would you like the game to animate? Takes more time to display\ny or n\n";
	dec = getch();
	switch( dec )
	{
		case 121:
			ANIMATE = true;
			break;
		case 110:
			break;
		default:
			cout << "Invalid Input, proceeding without animation.\n";
			break;
	}
	system("cls");
	intro();
	while( gameState != EGameState::E_GS_QUIT )
	{
		switch( gameState )
		{
		case EGameState::E_GS_STARTING:
			createData(maze,player);
			displayMarkupGrid();
			userData(player);
			system("cls");
		break;
		case EGameState::E_GS_MENU :
			decision = 0;//reset the the fields into default values ---- could put it into function
			currentPosition[0]	= 0;
			currentPosition[1]	= 0;
			oldPosition[0]		= 0;
			oldPosition[1]		= 0;
			lastDecision		= 0;
			switch( menu(decision) )
			{
			case 49:
				while( gameState == EGameState::E_GS_GAME )
				{
					system("cls");
					displayMarkupGrid();
					inGame(maze, player, decision, currentPosition, oldPosition, lastDecision);
				}
				break;
			case 50:
				help();
				system("cls");
				break;
			case 51:
				learn(maze);
				break;
			case 52:
				displayGrid(maze);
				ANIMATE = false;
				Sleep(1000);
				system("cls");
				gameState = EGameState::E_GS_MENU;
				break;
			case 53:
				saveOrRetrieveHighscore(player,true);
				cin.get();
				break;
			case 54:
				gameState = EGameState::E_GS_QUIT;
				break;
			}
		break;
		case EGameState::E_GS_WIN:
		case EGameState::E_GS_LOSS:
			endScenario(player);
		break;
		}
	}
	cout << "Goodbye. Thanks for playing ...\n";
	Sleep(1000);
	return 0;
}

void intro(void)
{
	string msg;
	msg = "Welcome to the maze game - TRIAL VERSION\n\n";
	bool change = false;
	string::iterator it;
	int width = 0;
	while( msg.length() > 0 )
	{
		for( it = msg.begin(); it < msg.end();++it)
		{
			cout << setw(width) <<  *it;
			if( ANIMATE )
				Sleep(delay);
		}
		cout << endl;
		msg = "";
	}
}

bool createData(Vars maze[arraySize][arraySize], PlayerInformation& player)//could declare Vars* maze
{
	string	line			= "";							//this variable will provide info about current position in the file
	char	toINT1[1]		= {' '},
			toINT2[1]		= {' '};						//These variables are one element arrays. Reason why used is because of atoi()
	int		i				= 0,							//
			j				= 0,							//
			k				= 0,							//
			l				= 0,							//
			mazeValue[2]	= {doNotDisplay,doNotDisplay};
	//---------initialising player members
	for( i = 0; i < arraySize; ++i )
	{
		for( j = 0; j < arraySize; ++j )
		{
			maze[i][j].left[0]		= 0;
			maze[i][j].left[1]		= 0;
			maze[i][j].right[0]		= 0;
			maze[i][j].right[1]		= 0;
			maze[i][j].visited		= false;
		}
	}
	i = j = k = l = 0;
	player.name				= "";
	player.score			= 0;
	ifstream file("maze.txt", std::ios::out || std::ios::in);//file which is going to be used to create the maze
	if( file.is_open() )						//opening the file
	{
		while( file.good() && i < 100)			//while file is open
		{
			std::getline( file, line );			//retrieve the line from file
			//cout << line << endl;
			if( line.size() > 1 && line == "-1" )//if value is -1, we need to check the size of the "line" cause if less than 2 causes debug assertion error
			{
				mazeValue[0]	= -1;
				mazeValue[1]	= doNotDisplay;	//this is going to be checked when displaying the grid.
			}
			else if( line.size() > 1 )			//if the size of the string is bigger than one
			{
				toINT1[0]		= line[0];				
				toINT2[0]		= line[1];		//assign values of the string to char variable
				mazeValue[0]	= atoi(toINT1);	//convert char into int
				mazeValue[1]	= atoi(toINT2);
			}
			else								//if the value is less than 2
			{
				if( line[0] == '0' )
				{
					mazeValue[0] = 0;
					mazeValue[1] = doNotDisplay;//this check is for zero zero the exit value
				}
				else if( line[0] >= 0 )
				{
					toINT1[0]	 = line.at(0);//First value will have the < 10 value and the second value will be doNotDisplay, this will make things easier in the future to display the grid.
					mazeValue[0] = toINT1[0] - 48;//- 48 because of ASCII values
				}
			}

			if( l == 0 )							//when zero
			{
				maze[j][k].left[0]  = mazeValue[0];//assign value to the left pointer
				maze[j][k].left[1]	= mazeValue[1];
			}
			else if( l == 1 )					//when one
			{
				maze[j][k].right[0] = mazeValue[0];//assign value to the right pointer
				maze[j][k].right[1] = mazeValue[1];
			}
			mazeValue[0]		 = doNotDisplay;//reset value of the field in maze || update: just to be on the safe side both parts of the array are set to this value
			mazeValue[1]		 = doNotDisplay;//in this program, only the second part of the array could be not filled with data, therefore enum set up to determine whether to display it or not.	Reason why enumeration is used is because I can access it from this part of program without passing additional arguments.
											//add one so we can switch between two fields
			++l;
			if( l == 2 )
			{
				++k;
				++i;								//another field in maze
				l = 0;
			}
			if( k == 10 )
			{
				++j;
				k = 0;
			}
		}//end of while
	}//end of if
	else									//if file read has failed
		return false;						//finish and return false
	return true;							//if everything went well return true
}//end of createData()

void displayGrid(const Vars maze[arraySize][arraySize] )
{
	__int8 lineEnd = 0;
	int temp = 0;
	for( int i = 0; i < arraySize; ++i)
	{
		for( int j = 0; j < arraySize; ++j )
		{
			if( maze[i][j].left[0] == -1 && maze[i][j].right[0] == -1 )//if both pointers are -1
			{
				cout << maze[i][j].left[0] << " " << maze[i][j].right[0] << /*this is a post display interval*/"  ";
			}
			else
			{
				//-------------LEFT POINTER------------------
				cout << maze[i][j].left[0];/*first element will never be -2 so we don't have to check it*/
				if( maze[i][j].left[1] != doNotDisplay )
						cout << maze[i][j].left[1];
				else if( maze[i][j].left[0] != -1 )
					cout << " ";
				/////////////////////////////////////////////
				//----INTER DISPLAY INTERVAL--
				cout << " ";
				////////////////////////
				//-------------RIGHT POINTER-----------------
				cout << maze[i][j].right[0];
				if( maze[i][j].right[1] != doNotDisplay )//-||-
					cout << maze[i][j].right[1];
				else if( maze[i][j].right[0] != -1 )
					cout << " ";
				/////////////////////////////////////////////
				//----POST DISPLAY INTERVAL---
				cout << "  ";
				//////////////////////////////
			}
			++lineEnd;
			if( lineEnd == 10 )//determine when line ends
			{
				cout << endl;
				lineEnd = 0;
			}
		}
	}
}

void displayMarkupGrid()
{
	if( gameState == EGameState::E_GS_GAME )
		delay = 0;
	for( int i = 0; i < arraySize; ++i )
	{
		for( int j = 0; j < arraySize; ++j )
		{
			cout << i << j << " ";
			if( ANIMATE )
				Sleep(delay);

		}
		cout << endl;
	}
}

int  menu( int& decision )
{
	string menu = "MAIN MENU\n1. Start the game.\n2. Display Help.\n3. Learn the maze\n4. View the maze( numbers ) - limited time ;).\n5. View High scores.\n6. Exit\nIf you want to see the maze for unlimited time, buy FULL Version ;)\nPlease enter your choice: \n";
	string::iterator it;
	cout << setw(30);
	for( it = menu.begin(); it < menu.end(); ++it)
	{
		if( ANIMATE )
			Sleep(delay);
		cout << *it;
	}
	decision = _getch();
#ifdef TEST
	cout << decision << endl;
#endif
	switch( decision )
	{
	case 49://1
		gameState = EGameState::E_GS_GAME;
		return decision;
		break;
	case 50://2
		gameState = EGameState::E_GS_HELP;
		return decision;
		break;
	case 51://3
		gameState = EGameState::E_GS_LEARN;
		return decision;
		break;
	case 52://4
		gameState = EGameState::E_GS_GRID_DISPLAY;
		return decision;
		break;
	case 53://5
		gameState = EGameState::E_GS_HIGHSCORES_DISPLAY;
		return decision;
		break;
	case 54://6
		gameState = EGameState::E_GS_QUIT;
		return decision;
		break;
	case 27://Escape
		gameState = EGameState::E_GS_QUIT;
		return decision;
		break;
	}
	return NULL;
}

bool inGame( const Vars maze[arraySize][arraySize], PlayerInformation& player,vectors& vec, int& decision, int newPosition[], int oldPosition[], int& lastDecision )
{
	if( oldPosition[0] == 0 && oldPosition[1] == 0 )
	{
		cout << "Your current position is START.";
		cout << setw(20) << "Player: " << player.name << endl << endl;
		cout << "Current node options: ";
		//-------------------------------LEFT POINTER
		cout << maze[oldPosition[0]][oldPosition[1]].left[0];//first element
		if( maze[oldPosition[0]][oldPosition[1]].left[1] != doNotDisplay )
			cout << maze[oldPosition[0]][oldPosition[1]].left[1];//second element
		//-------------------------------INTER DISPLAY INTERVAL
		cout << " ";
		//-------------------------------RIGHT POINTER
		cout << maze[oldPosition[0]][oldPosition[1]].right[0];//first element
		if( maze[oldPosition[0]][oldPosition[1]].right[1] != doNotDisplay )
			cout << maze[oldPosition[0]][oldPosition[1]].right[1];//second element
	}
	else if( maze[oldPosition[0]][oldPosition[1]].left[0] == 0 && maze[oldPosition[0]][oldPosition[1]].right[0] == 0 )
	{
		cout << "Congratulations " << player.name ;
		gameState = EGameState::E_GS_WIN;
		return true;
	}
	else if( maze[oldPosition[0]][oldPosition[1]].left[0] == -1 && maze[oldPosition[0]][oldPosition[1]].right[0] == -1 )
	{
		cout << "No more move!\n";
			gameState = EGameState::E_GS_LOSS;
			return false;
	}
	else
	{
		cout << "Your current position is X:" << oldPosition[0]<< " Y: ";
		if( oldPosition[1] != doNotDisplay )
			cout << oldPosition[1];
		else
			cout << 0;
		cout << setw(20) << "Player: " << player.name << endl;
		if( lastDecision == 0 )
			cout << "Last VALID choice was LEFT.\n";
		else
			cout << "Last VALID choice was RIGHT\n";
		cout << "Current node options: ";
		//-------------------------------LEFT POINTER
		cout << maze[oldPosition[0]][oldPosition[1]].left[0];//first element
		if( maze[oldPosition[0]][oldPosition[1]].left[1] != doNotDisplay )
			cout << maze[oldPosition[0]][oldPosition[1]].left[1];//second element
		//-------------------------------INTER DISPLAY INTERVAL
		cout << " ";
		//-------------------------------RIGHT POINTER
		cout << maze[oldPosition[0]][oldPosition[1]].right[0];//first element
		if( maze[oldPosition[0]][oldPosition[1]].right[1] != doNotDisplay )
			cout << maze[oldPosition[0]][oldPosition[1]].right[1];//second element
	}
	cout << "\nWhich way now ?\n";
	decision = _getch();
	switch( _getch() )//used twice because arrow keys hold 2 values :/
	{
	case 75://left
		cout << "LEFT\n";
		if( maze[oldPosition[0]][oldPosition[1]].left[0] != -1 )
		{
			if( maze[oldPosition[0]][oldPosition[1]].left[1] != doNotDisplay )
			{
				newPosition[0] = maze[oldPosition[0]][oldPosition[1]].left[0];
				newPosition[1] = maze[oldPosition[0]][oldPosition[1]].left[1];
				vec.playerDecisions.push_back(newPosition[0]);
				vec.playerDecisions.push_back(newPosition[1]);
			}
			else
			{
				newPosition[0] = 0;
				newPosition[1] = maze[oldPosition[0]][oldPosition[1]].left[0];//because first element will hold the value of the column in this case
				vec.playerDecisions.push_back(newPosition[0]);
				vec.playerDecisions.push_back(newPosition[1]);
			}
		}
		else
		{
			cout << "Yo can't go this way !\n";
			lastDecision		= 0;
			break;
		}
		oldPosition[0]			= newPosition[0];
		oldPosition[1]			= newPosition[1];
		lastDecision			= 0;
		break;
	case 77://-----------right
		cout << "RIGHT\n";
		if( maze[oldPosition[0]][oldPosition[1]].right[0] != -1 )
		{
			if( maze[oldPosition[0]][oldPosition[1]].right[1] != doNotDisplay )
			{
				newPosition[0] = maze[oldPosition[0]][oldPosition[1]].right[0];
				newPosition[1] = maze[oldPosition[0]][oldPosition[1]].right[1];
				vec.playerDecisions.push_back(newPosition[0]);
				vec.playerDecisions.push_back(newPosition[1]);
			}
			else
			{
				newPosition[0] = 0;
				newPosition[1] = maze[oldPosition[0]][oldPosition[1]].right[0];//because first element will hold the value of the column in this case
				vec.playerDecisions.push_back(newPosition[0]);
				vec.playerDecisions.push_back(newPosition[1]);
			}
		}
		else
		{
			cout << "Yo can't go this way !\n";
			lastDecision		= 1;
			break;
		}
		oldPosition[0]			= newPosition[0];
		oldPosition[1]			= newPosition[1];
		lastDecision			= 1;
		break;
	case 72://up
		cout << "You have chosen to go UP?\n";
		break;
	case 80://down
		cout << "You have chosen to go DOWN?\n";
		break;
	case 27://escape
		cout << "Exiting ....\n";
		gameState = EGameState::E_GS_QUIT;
		break;
	default:
		break;
	}
	Sleep(700);
	return true;
}

bool userData( PlayerInformation& player)
{
	cout << "Please enter your name: ";
	cin >> player.name;
	gameState = EGameState::E_GS_MENU;
	return true;
}

bool learn(const Vars maze[arraySize][arraySize], vectors& vec)
{
	bool found = false;
	int x	= 0,
		y	= 0,
		px	= 0,
		py	= 0;

	temp t[arraySize][arraySize];
	vec.learnPath.push_back(0);
	vec.learnPath.push_back(0);//start position
	while( !found )
	{
		if( maze[x][y].left[0] != -1 && !t[x][y].left)//check if it is dead end
		{
			if( maze[x][y].left[0] > 0 && maze[x][y].left[1] == doNotDisplay )//check if it is a single value
			{
				px = 0;//assign the values to the temp vars
				py = maze[x][y].left[0];
				t[x][y].left = true;//tick that it has been visited
				vec.learnPath.push_back(px);
				vec.learnPath.push_back(py);
				if( maze[x][y].left[0] && maze[x][y].right )//before we try to assign it check if we have found the end of maze
				{
					found = true;
					break;
				}
				x = px;
				y = py;
			}
			else
			{
				px = maze[x][y].left[0];
				py = maze[x][y].left[1];
				t[x][y].left = true;
				vec.learnPath.push_back(px);
				vec.learnPath.push_back(py);
				if( maze[x][y].left[0] && maze[x][y].right )
				{
					found = true;
					break;
				}
				x = px;
				y = py;
			}
		}
		else if( maze[x][y].right[0] != -1 && !t[x][y].right)//check if it is dead end
		{
			if( maze[x][y].right[0] > 0 && maze[x][y].right[1] == doNotDisplay )//check if it is a single value
			{
				px = 0;
				py = maze[x][y].right[0];
				t[x][y].right = true;
				vec.learnPath.push_back(px);
				vec.learnPath.push_back(py);
				if( maze[x][y].left[0] && maze[x][y].right )
				{
					found = true;
					break;
				}
				x = px;
				y = py;
			}
			else
			{
				px = maze[x][y].right[0];
				py = maze[x][y].right[1];
				t[x][y].right = true;
				vec.learnPath.push_back(px);
				vec.learnPath.push_back(py);
				if( maze[x][y].left[0] && maze[x][y].right )
				{
					found = true;
					break;
				}
				x = px;
				y = py;
			}
		}
		else if( t[x][y].left && t[x][y].right )//if both fields were visited then go back one
		{
			if( path.back() == 0 && path.size() < 2 )
				return false;
			vec.learnPath.pop_back();//remove last position
			vec.learnPath.pop_back();
			x = path.back() - 1;//assign last positions to the x and y
			y = path.back();
		}
	}
	gameState = EGameState::E_GS_MENU;
	return true;
}

void help(void)
{
	cout << "\nAim of the game is to find your way through the maze.\nYou have two options to choose from when in game,left or right.\n If you find the position with values \"0\" \"0\" then you win.\n But if you don't you lose :(\n";
	cout << "Press any key to continue ....\n";
	gameState = EGameState::E_GS_MENU;
	_getch();
	return;
}

void saveOrRetrieveHighscore(PlayerInformation& player, bool retrieve )
{
	fstream file("highscores.txt", std::ios::in | std::ios::out | std::ios::app);
	if( !retrieve )
	{
		player.name.push_back('|');
		if( file.is_open() )
		{
			file << player.name;
			file << player.score;
		}
		return;
	}
	else
	{
		string line;
		if( file.is_open() )
		{
			while( file.good() )
			{
				getline(file,line);
				if( line.length() < 1 )//check if there are any results
				{
					cout << "\n\nNo High scores in the database!\n";
					Sleep( 500 );
					system("cls");
					gameState = EGameState::E_GS_MENU;
					return;				//if not return
				}
				cout << "Name: ";
				for( int i = 0; i < line.length(); ++i )
				{
					if( line[i] == '|' )
					{
						cout << " ";
						cout << "Score: ";
					}
					else
						cout << line[i];
				}
				cout << endl;
			}
		}
	}
	gameState = EGameState::E_GS_MENU;
	return;
}

void endScenario( PlayerInformation& player )
{
	if( gameState == EGameState::E_GS_WIN )
	{
		cout << "\nYou have won !!!\n What would you like to do now ?\n";
		cout << "1. Go to the main menu.\n";
		cout << "2. Exit to the desktop.\n";
		score(player);
		saveOrRetrieveHighscore(player);
		switch( getch() )
		{
		case 49:
			gameState = EGameState::E_GS_MENU;
			break;
		case 50:
			gameState = EGameState::E_GS_QUIT;
		default:
			break;
		}
	}
	else
	{
		cout << "Oh well maybe other time :)\n";
		gameState = EGameState::E_GS_LOSS;
	}
}

void score(PlayerInformation& player, vectors& vec)
{
	for( int i = 0; i < vec.learnPath.size(); ++i )
	{
		if(vec.playerDecisions[i] == vec.learnPath[i] )
			player.score +=10;
	}
}
