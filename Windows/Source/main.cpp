#include <string>
#include "Program.h"

std::string extractAppPath(const char *fullPath)
{
#ifdef __APPLE__										//Mac
	std::string s(fullPath);
	for (int i = 0; i < 5; ++i)
		s = s.substr(0, s.rfind("/"));
	return s + "/";
#else
	const std::string s(fullPath);
	if (s.find("/") != std::string::npos)				//Linux
		return s.substr(0, s.rfind( "/" )) + "/";
	else if (s.find("\\") != std::string::npos)			//Windows
		return s.substr(0, s.rfind( "\\" )) + "\\";
	return s;
#endif
}
//Code
int main(int argc, char *argv[])
{
	std::string path = extractAppPath(argv[0]);
	Program prog(path.c_str());
	if (prog.initialize())
		prog.runGame();
	prog.shutdown();
	return 0;
}
