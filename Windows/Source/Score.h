#ifndef SCORE_H
#define SCORE_H

#include <string>

using namespace std;

class Score
{
public:
	enum BALL_ID
	{
		FIRST,
		SECOND,
		THIRD
	};
//Code
	Score(int _f = 0, int _s = 0, int _t = 0)
	{
		total = _f + _s + _t;
		ball[FIRST] = _f;
		ball[SECOND] = _s;
		ball[THIRD] = (_f == 10) ? 2 : (_f+_s == 10) ? 1 : 0;
	}

	string getScore(int id) const
	{
		if (id < 0 || id > 2) return "";
		if (ball[id] == 0) return "-";
		if (ball[id] == 10) return "X";
		if (id == SECOND && ball[FIRST]+ball[SECOND] == 10) return "/";
		char buffer[4];
		_snprintf_s(buffer, 4, "%d", ball[id]);
		return buffer;
	}

	void setScore(int id, int score)
	{
		if (id < 0 || id > 2) return;
		ball[id] = score;
		if (id != THIRD)
		{
			total = ball[FIRST] + ball[SECOND];
			ball[THIRD] = isStrike() ? 2 : isSpare() ? 1 : 0;
		}
		else
			total = ball[FIRST] + ball[SECOND] + ball[THIRD];
	}

	void update(int pinsDown)
	{
		if (ball[THIRD] > 0)
		{
			total += pinsDown;
			ball[THIRD]--;
		}
	}

	bool isStrike() const { return (ball[FIRST] == 10); }
	bool isSpare() const { return (!isStrike() && ball[FIRST]+ball[SECOND] == 10); }
	bool isOpen() const { return (ball[THIRD] != 0); }

	int ball[3];
	int total;
};

#endif
