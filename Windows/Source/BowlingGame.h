#ifndef BOWLING_GAME_H
#define BOWLING_GAME_H

#include <string>
#include <vector>
#include "Player.h"
//Games
using namespace std;

class BowlingGame
{
public:
	BowlingGame();
	~BowlingGame();

	int initialize();
	int update();
	int shutdown();

	const vector<Player> getPlayers() const { return _players; }
	int getCurrentTurn() const { return _turn; }
	int getGameNumber() const { return _gameNumber; }
	int getLaneID() const { return _laneID; }

private:
	enum NAME_VALIDATION
	{
		NAME_LENGTH_INVALID,
		NAME_ALREADY_TAKEN,
		NAME_IS_VALID
	};
	int nameIsValid(const string &name);
	int _gameNumber;
	int _laneID;
	vector<Player> _players;
	int _turn;
	int _frame;
};

#endif
