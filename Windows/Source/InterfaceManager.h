#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "console.h"
#include <iostream>
#include <string>

using namespace con;
using namespace std;

class BowlingGame;
//Code
static class InterfaceManager
{
public:
	InterfaceManager();
	~InterfaceManager();

	int initialize(string windowName, int width = 84, int height = 30);
	void print(const char *format, ...)
	{
		char buffer[128];
		va_list vlist;
		va_start(vlist, format);
		_vsnprintf_s(buffer, 128, format, vlist);
		va_end(vlist);
		cout << buffer;
	}

	void setWindowSize(int width, int height, bool fixed = true);
	int getIntegerValue(const string &message);
	string getStringValue(const string &message);
	bool getBooleanValue(const string &message);

	void drawSplashScreen(const BowlingGame &game);
	void drawScoreTable(const BowlingGame &game);
	void drawGameOverScreen(const BowlingGame &game);
} Interface;

#endif
