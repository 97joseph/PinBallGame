#include "GameUtil.h"

bool GameUtil::compareStrings(const char *lhs, const char *rhs)
{
	if (!lhs || !rhs) return (lhs == rhs);	//ensure strings exist
	int lengthA = 0, lengthB = 0;
	while(lhs[lengthA] != '\0') lengthA++;
	while(rhs[lengthB] != '\0') lengthB++;
	if (lengthA != lengthB) return false;	//different length strings
	for (int i = 0; i < lengthA; i++)
		if (lhs[i] != rhs[i]) return false;	//non-matching values
	return true;
}
//Code

bool GameUtil::stringToBoolean(const char *input)
{
	if (!input) return 0;
	int length = 0;
	while(input[length] != '\0') length++;	//Find length of input string

	if (length == 1 && (input[0] == 'y' || input[0] == 'Y'))
		return true;	//'y' accepted as true

	if (length != 3) return false;
	const char *trueString = "yes";		//Test case
	for (int i = 0; i < length; i++)
		if (input[i] != trueString[i] && (input[i]-' ') != trueString[i])
			return false;
	return true;
}

int GameUtil::stringToInteger(const char *input, bool strict)
{
	if (!input) return 0;
	unsigned int output = 0;

	int i = 0;
	char c = input[i];
	while (c != '\0')
	{
		if ((c < '0' || c > '9'))		//skip non-numbers
		{
			if (strict)
				return -1;	//disallow invalid input
			continue;		//ignore invalid input
		}
		int value = (int)(c - '0');	//convert from ASCII to integer
#ifdef _DEBUG
		if (value > UINT_MAX * 0.1f - value) break;	//UINT limit
#endif
		output = output * 10 + value;
		c = input[++i];
	}
	return output;
}
