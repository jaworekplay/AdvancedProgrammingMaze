#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

#pragma once //this ensures that header files included in this header file are included only once. NOTE: if header files are included more than once in ypur application it causes error of redefinition
#include <string>//this allows us to use the string variables
#include <iostream>//basic input output operations
#include <vector>//vector is used to hold the right path to win the game

using namespace std;

bool					 ANIMATE		= false;
short unsigned int		 delay			= 20U;
const short unsigned int arraySize		= 10U;
const int				 doNotDisplay	= -2;
//maze structure
struct Vars
{
	int			left[2],		//pointer to the left node - reason why an array is because we need to use column and row values
				right[2];		//pointer to the right node
	bool		visited;		//status of the node
};

//------------------------player's structure
struct PlayerInformation
{
	string		name;		//name of the player
	unsigned	score;		//player's score
};

struct temp
{
	bool left,right;
};
struct vectors
{
	vector<int> playerDecisions;
	vector<int> learnPath;
};

enum EGameState //this enumeration will provide information for our current game state.
{
	E_GS_STARTING = 0,
	E_GS_MENU,
	E_GS_HELP,
	E_GS_GAME,
	E_GS_LEARN,
	E_GS_GRID_DISPLAY,
	E_GS_HIGHSCORES_DISPLAY,
	E_GS_WIN,
	E_GS_LOSS,
	E_GS_QUIT
} gameState;


void			intro(					void);
bool			createData(				Vars[arraySize][arraySize],PlayerInformation& );
void			displayGrid(			const Vars[arraySize][arraySize]);
void			displayMarkupGrid(		void );
int				menu(					int& );
bool			userData(				PlayerInformation& );
bool			inGame(					const Vars[arraySize][arraySize], PlayerInformation&, vectors&, int&, int [], int [], int& );
bool			learn(					const Vars[arraySize][arraySize] , vectors&);
void			help(					void);
void			score(					PlayerInformation&, vectors&);
void			saveOrRetrieveHighscore(PlayerInformation&, bool retrieve = false );
void			endScenario(			PlayerInformation& );
#endif