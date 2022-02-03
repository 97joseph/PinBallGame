#include "InterfaceManager.h"
#include "GameUtil.h"
#include "BowlingGame.h"
#include "Player.h"

InterfaceManager::InterfaceManager()
{
}

InterfaceManager::~InterfaceManager()
{
}

int InterfaceManager::initialize(string windowName, int width, int height)
{
	console.setTitle(windowName);
	console.setSize(width, height, true);
	return 1;
}
//Manager
void InterfaceManager::setWindowSize(int width, int height, bool fixed)
{
	console.setSize(width, height, fixed);
}

int InterfaceManager::getIntegerValue(const string &message)
{
	cout << message;
	string input;
	cin >> input;
	return GameUtil::stringToInteger(input.c_str(), true);	//strict input
}

string InterfaceManager::getStringValue(const string &message)
{
	cout << message;
	string input;
	cin >> input;
	return input;
}

bool InterfaceManager::getBooleanValue(const string &message)
{
	cout << message;
	string input;
	cin >> input;
	return GameUtil::stringToBoolean(input.c_str());
}

void InterfaceManager::drawSplashScreen(const BowlingGame &game)
{
	vector<Player> players = game.getPlayers();
	console.setSize(84, 28 + (players.size() * 3), true);
	console.clear();
	cout << "               .-.\n";
	cout << "               \\ /      .-.\n";
	cout << "               |_|  .-. \\ /\n";
	cout << "               |=|  \\ / |_|\n";
	cout << "              /   \\ |_| |=|\n";
	cout << "             | (@) ||=|/   \\\n";
	cout << "        ____ |     /   \\@)  |\n";
	cout << "      .'    '.    | (@) |   |\n";
	cout << "     / #      \\   |     |   |\n";
	cout << "    |          |  |     |   |\n";
	cout << "    |    o o   |='|     |  /\n";
	cout << "     \\     o  /    \\   /'='\n";
	cout << "      '.____.'      '='\n\n";
	
	for (int i = 0; i < (int)players.size(); i++)
	{
		cout << " Player " << i+1 << ": " << players[i].getName() << "\n";
	}
	cout << "\n";
}

void InterfaceManager::drawScoreTable(const BowlingGame &game)
{
	vector<Player> players = game.getPlayers();
	int currentTurn = game.getCurrentTurn();
	console.clear();
	print(" _______________________________________________________________________________ \n");
	print("| Lane: %d, Game: %d |__1__|__2__|__3__|__4__|__5__|__6__|__7__|__8__|__9__|__10__|\n",
		game.getLaneID(), game.getGameNumber());
	int cursorPosY = console.getCurPosY();
	for (int i = 0; i < (int)players.size(); i++)
	{
		print("|                  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |   |\n");
		print("|                  |     |     |     |     |     |     |     |     |     |      |\n");
		if (i < (int)players.size()-1)
			print("|------------------|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|\n");
		else
			print("`------------------'-----'-----'-----'-----'-----'-----'-----'-----'-----'------'\n");
	}
	for (int i = 0; i < (int)players.size(); i++)
	{
		if (currentTurn == i)
		{
			console.save();
			console.setFgColour(_yellow);
		}
		console.setCurPosY(cursorPosY);
		Player player = players[i];
		vector<Score> scores = player.getScores();
		for (int j = 0; j < (int)scores.size(); j++)
		{
			console.setCurPosX(21 + (j*6));
			print(scores[j].getScore(Score::FIRST).c_str());
			console.setCurPosX(23 + (j*6));
			print(scores[j].getScore(Score::SECOND).c_str());
			if (j == 9)
			{
				console.setCurPosX(25 + (j*6));
				print(scores[j].getScore(Score::THIRD).c_str());
			}
		}
		console.setCurPosY(cursorPosY+1);
		int space = 18 - player.getName().length();
		console.setCurPosX(space);
		print(player.getName().c_str());
		int totalScore = 0;
		for (int j = 0; j < (int)scores.size(); j++)
		{
			if (!scores[j].isOpen() || j == 9)
			{
				totalScore += scores[j].total;
				char buffer[4];
				_snprintf_s(buffer, 4, "%d", totalScore);
				console.setCurPosX(21 + (j*6));
				print(buffer);
			}
		}
		cursorPosY += 3;
		if (currentTurn == i)
		{
			console.restore();
		}
	}
	console.setCurPosXY(0, 3 + players.size() * 3);
	print(" Current player's turn: %s\n\n", players[currentTurn].getName().c_str());
}

void InterfaceManager::drawGameOverScreen(const BowlingGame &game)
{
	drawScoreTable(game);
	console.setFgColour(_yellow);
	cout << "\n\n        .-..-\"\"``\"\"-..-.\n";
	cout << "        |(`\\`'----'`/`)|\n";
	cout << "         \\\\ ;:.    ; //\n";
	cout << "          \\\\|%.    |//\n";
	cout << "           )|%:    |(\n";
	cout << "         ((,|%.    |,))\n";
	cout << "          '-\\::.   /-'\n";
	cout << "             '::..'\n";
	cout << "               }{\n";
	cout << "              {__}\n";
	cout << "             /    \\\n";
	cout << "            |`----'|\n";
	cout << "            | [#1] |\n";
	cout << "            '.____.'\n\n";
	console.setFgColour(_grey);
	
	vector<Player> players = game.getPlayers();
	int topScore = 0, totalScore = 0;
	vector<Player> topPlayers;
	for (int i = 0; i < (int)players.size(); i++)
	{
		int score = players[i].calculateScore();
		totalScore += score;
		if (score > topScore)
		{
			topPlayers.clear();
			topScore = score;
			topPlayers.push_back(players[i]);
		}
		else if (score == topScore)
		{
			topPlayers.push_back(players[i]);
		}
	}
	if (topPlayers.size() == 1)
		cout << " The winner for this game is " << topPlayers[0].getName();
	else
	{
		cout << " The game resulted in a tie between ";
		for (int i = 0; i < (int)topPlayers.size() - 1; i++)
		{
			cout << topPlayers[i].getName();
			if (i < (int)topPlayers.size() - 2)
				cout << ", ";
		}
		cout << " and " << topPlayers.back().getName();
	}
	cout << " with " << topScore << " points!\n";
	cout << " The team scored a total of " << totalScore << " points.\n\n";
}
