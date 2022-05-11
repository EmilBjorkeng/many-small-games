#include <ctime>                    // CTime (For Randomiser)
#include <stdlib.h>                 // Standard library
#include <string>                   // String
#include <cstring>                  // CString
#include <sstream>                  // SStream

bool randomSeedSet = false;

// Randomiser
int random(int start, int end) {
	if (!randomSeedSet)
	{
		srand(time(0));
		randomSeedSet = true;
	}
	int random = start + rand() % (end - start + 1);
	return random;
}

// Random between 2 numbers
int rb2n(int num1, int num2) {
	if (!randomSeedSet)
	{
		srand(time(0));
		randomSeedSet = true;
	}
	int random = rand() % 2;

    if (random) return num1;
    else return num2;
}

// Convert String to Integer
int sti(std::string str)
{
	char buffer [50];
	strcpy(buffer, str.c_str());

	return atoi(buffer);
}

// Convert Integer to String
std::string its(int integer)
{
	std::stringstream ss;
	ss << integer;
	std::string str = ss.str();

	return str;
}