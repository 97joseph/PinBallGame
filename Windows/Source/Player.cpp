#include "Player.h"
#include "InterfaceManager.h"

Player::Player()
{
	_name = "";
	_scores.reserve(10);
	_ballThisFrame = 0;
}

Player::~Player()
{
}
//Code
bool Player::bowlBall(int currentFrame)
{
	int scoreThisBowl = -1;
	int cursorPosY = console.getCurPosY();
	do
	{
		console.setCurPosY(cursorPosY);
		console.clearwin(0, cursorPosY, 80, 30);
		scoreThisBowl = Interface.getIntegerValue(" How many pins were hit on this bowl? ");
	}
	while(!isValidScore(scoreThisBowl, currentFrame));
	
	for (int i = 0; i < (int)_scores.size(); i++)
		_scores[i].update(scoreThisBowl);
	_currentFrameScore.setScore(_ballThisFrame, scoreThisBowl);
	_ballThisFrame++;
	if (_ballThisFrame == 1 && !_currentFrameScore.isStrike())
		return false;
	if (currentFrame == 10)	//special case for final frame
	{
		if (_ballThisFrame == 1)
			return false;
		else if (_ballThisFrame == 2)
		{
			if (_currentFrameScore.isStrike() || _currentFrameScore.isSpare())
				return false;
			else if (!_currentFrameScore.isSpare())
				_currentFrameScore.setScore(Score::THIRD, 0);
		}
	}
	_scores.push_back(_currentFrameScore);
	_currentFrameScore = Score();
	_ballThisFrame = 0;
	return true;	//turn ended
}

int Player::calculateScore()
{
	if (_scores.empty()) return 0;
	int totalScore = 0;
	for (int i = 0; i < (int)_scores.size(); i++)
		totalScore += _scores[i].total;
	return totalScore;
}

bool Player::isValidScore(int score, int frame)
{
	if (score < 0 || score > 10)
		return false;
	if (frame != 10)
	{
		if (_ballThisFrame == 1 && _currentFrameScore.ball[Score::FIRST] + score > 10)
			return false;
		if (_ballThisFrame == 2)
			return false;
	}
	return true;
}
