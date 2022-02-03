#ifndef PLAYER_H
#define PLAYER_H

#include "Score.h"
#include <vector>

using namespace std;

class Player
{
public:
	Player();
	~Player();

	const string getName() const { return _name; }
	void setName(string newName) { _name = newName; }
	const vector<Score> getScores() const { return _scores; }
	void addScore(int first, int second);

	bool bowlBall(int currentFrame);
	int calculateScore();
//Code
private:
	bool isValidScore(int score, int frame);
	Score _currentFrameScore;
	int _ballThisFrame;
	string _name;
	vector<Score> _scores;
};

#endif
