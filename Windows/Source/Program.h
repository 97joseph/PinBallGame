#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include "BowlingGame.h"

class Program
{
public:
	Program(const char *);
	~Program();

	int initialize();
	int runGame();
	int shutdown();

private:
	static const char *_appDirPath;
	bool _initialized;
	bool _running;
	BowlingGame game;
};
//Call ends
#endif
