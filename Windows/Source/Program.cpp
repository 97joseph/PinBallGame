#include "Program.h"
#include "BowlingGame.h"

const char *Program::_appDirPath = 0;

Program::Program(const char *_appPath)
{
	_appDirPath = _appPath;
	_initialized = false;
	_running = false;
}

Program::~Program()
{
	if (_initialized)
		shutdown();		//ensure all data is freed 
}

int Program::initialize()
{
	if (_initialized)
		return 1;	//already initialized
	game.initialize();
	_initialized = true;
	return 1;		//successful initialization
}

int Program::runGame()
{
	if (!_initialized && !initialize())
		return 0;	//unable to initialize game
	if (_running)
		return 0;	//game already running

	_running = true;
	while (_running)
	{
		if (!game.update())
			_running = false;
	}
	_running = false;	//finish running the game
	return 1;
}

int Program::shutdown()
{
	if (_initialized)
	{
		_initialized = false;
		_running = false;
		return 1;	//successful shutdown
	}
	return 0;	//not initialized
}
