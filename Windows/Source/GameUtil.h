#ifndef GAME_MATH_UTIL_H
#define GAME_MATH_UTIL_H

#include <iostream>
//
class GameUtil
{
public:
	/*
		Compares two arrays of characters, returning true if they are
		equal to each other in length and contents
	*/
	static bool compareStrings(const char *lhs, const char *rhs);

	/*
		Attempts to parse an array of characters into a true/false
		statement. Accepts either "1" or "true" as true statements,
		and assumes any other input as false
	*/
	static bool stringToBoolean(const char *input);

	/*
		Converts an array of characters into a floating point number.
		This is calculated in base 10, up to an internal precision of
		64 bits (fixed point)
	*/
	static int stringToInteger(const char *input, bool strict = false);
};

#endif
