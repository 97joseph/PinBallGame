#include "BowlingGame.h"
#include "InterfaceManager.h"
//Game
BowlingGame::BowlingGame()
{
	_gameNumber = 1;
	_laneID = 1;
	_turn = 0;
	_frame = 0;
}

BowlingGame::~BowlingGame()
{
}

int BowlingGame::initialize()
{
	Interface.initialize("Mind Candy Bowling!");
	int playerCount = 0;
	do
	{
		Interface.drawSplashScreen(*this);
		playerCount = Interface.getIntegerValue(" Please enter the number of players (1-6): ");
	}
	while (playerCount < 1 || playerCount > 6);
	for (int i = 0; i < playerCount; i++)
		_players.push_back(Player());
	for (int i = 0; i < playerCount; i++)
	{
		Interface.drawSplashScreen(*this);
		char textBuffer[64];
		_snprintf_s(textBuffer, 64, " Please enter a name for player %d: ", i+1);
		string playerName = Interface.getStringValue(textBuffer);
		int valid = nameIsValid(playerName);
		while (valid != NAME_IS_VALID)
		{
			Interface.drawSplashScreen(*this);
			string msg;
			if (valid == NAME_LENGTH_INVALID)
				msg = " Name must be between 3 and 16 characters long.\n";
			else if (valid == NAME_ALREADY_TAKEN)
				msg = " The name provided already exists.\n";
			msg.append(textBuffer);
			playerName = Interface.getStringValue(msg);
			valid = nameIsValid(playerName);
		};
		_players[i].setName(playerName);
	}
	_turn = _players.size()-1;
	_frame = 0;
	return 1;
}

int BowlingGame::update()
{
	_turn = (_turn + 1) % _players.size();
	if (_turn == 0)
		_frame++;
	if (_frame > 10)	//end of game, all players have been through ten frames
	{
		Interface.drawGameOverScreen(*this);
		bool replay = Interface.getBooleanValue(" Would you like to play again? ");
		if (replay)
		{
			_gameNumber++;
			_players.clear();
			return initialize();
		}
		else
			return 0;	//quit the game
	}
	bool turnEnded = false;
	while (!turnEnded)
	{
		Interface.drawScoreTable(*this);
		turnEnded = _players[_turn].bowlBall(_frame);
	}
	return 1;
}

int BowlingGame::shutdown()
{
	_players.clear();
	return 1;
}

int BowlingGame::nameIsValid(const string &name)
{
	if (name.length() < 3 || name.length() > 16) return NAME_LENGTH_INVALID;
	for (int i = 0; i < (int)_players.size(); i++)
		if (name == _players[i].getName())
			return NAME_ALREADY_TAKEN;
	return NAME_IS_VALID;
}
