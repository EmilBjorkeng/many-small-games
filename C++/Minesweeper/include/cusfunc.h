#include <stdio.h>					// Standard
#include <stdlib.h>                 // Standard library
#include <fstream>					// Read/Write to files
#include <map>						// Map (For random int)
#include <random>					// Random Number Generator
#include <string>					// String
#include <cstring>					// CString
#include <iostream>					// iostream (cout)

// Rectangle
struct Rect
{
	float x, y;
	float width, height;
};

// Vector 2
struct Vec2
{
	int x, y;
};

// Json
//--------------------------------------------------------------------------------------
struct JsonInt
{
	char* name;
	int value;
};
JsonInt jsonInt;

struct JsonBool
{
	char* name;
	bool value;
};
JsonBool jsonBool;

struct JsonStr
{
	char* name;
	char* value;
};
JsonStr jsonStr;
//--------------------------------------------------------------------------------------

// Int to char
char* itc(int i)
{
	std::string s = std::to_string(i);
	char const *str = s.c_str();
	return (char*)str;
}

// Bool to char
char* btc(bool b)
{
	char* str;
	if (b) str = "true";
	else str = "false";
	return str;
}

// Random int
int random(int start, int end)
{
	std::random_device rd;
	std::map<int, int> hist;
	std::uniform_int_distribution<int> dist(start, end);

	int random = dist(rd);
	return random;
}

// Random Double
double randomD(int start, int end, int decimals) // Max 9 decimals
{
	unsigned long long int n = 10;
	for (int i = 0; i < decimals - 1; i++) n *= 10;

	std::random_device rd;
	std::map<int, int> hist;
	std::uniform_int_distribution<int> dist(start * n, end * n);

	int random = dist(rd);
	return (double) random / n;
}

int fileSize(char* path)
{
	std::ifstream file( path, std::ios::binary | std::ios::ate);
	return file.tellg();
}

int JsonLength(char* path)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		char ch;
		int i = 0;

		while (!file.eof())
		{
			file >> ch;
			if (ch == ':')
			{
				i++;
			}
		}
		file.close();
		return i;
	}
	else std::cerr << "No File" << std::endl;
	return -1;
}

void JsonClear(char* path)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		file << "{}";
	}
	else std::cerr << "No File" << std::endl;
	return;
}

// Write to json
//--------------------------------------------------------------------------------------
void wJsonAppendInt(char* path, char* name, int value)
{
	char s[50] = {};
	strcat(s, itc(value));

	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		char* str = new char [fileSize(path) + strlen(name) + strlen(s) + 10];
		char ch;
		int i = 0;

		while (!file.eof())
		{
			file >> ch;
			str[i] = ch;
			i++;
		}
		str[i - 2] = '\0';
		file.close();

		std::ofstream file(path);
		if (file.is_open())
		{
			file << str;
			if (!strcmp(str, "{") == 0) file.write(",", 1);
			file.write("\"", 1);
			file.write(name, strlen(name));
			file.write("\":", 2);
			file.write(s, strlen(s));
			file.write("}", 1);
		}
		else std::cerr << "No File While Writing" << std::endl;
		delete[] str;
		return;
	}
	else std::cerr << "No File" << std::endl;
	return;
}

void wJsonAppendBool(char* path, char* name, bool value)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		char* str = new char [fileSize(path) + strlen(name) + strlen(btc(value)) + 10];
		char ch;
		int i = 0;

		while (!file.eof())
		{
			file >> ch;
			str[i] = ch;
			i++;
		}
		str[i - 2] = '\0';
		file.close();

		std::ofstream file(path);
		if (file.is_open())
		{
			file << str;
			if (!strcmp(str, "{") == 0) file.write(",", 1);
			file.write("\"", 1);
			file.write(name, strlen(name));
			file.write("\":", 2);
			file.write(btc(value), strlen(btc(value)));
			file.write("}", 1);
		}
		else std::cerr << "No File While Writing" << std::endl;
		delete[] str;
		return;
	}
	else std::cerr << "No File" << std::endl;
	return;
}

void wJsonAppendStr(char* path, char* name, char* value)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		char* str = new char [fileSize(path) + strlen(name) + strlen(value) + 10];
		char ch;
		int i = 0;

		while (!file.eof())
		{
			file >> ch;
			str[i] = ch;
			i++;
		}
		str[i - 2] = '\0';
		file.close();

		std::ofstream file(path);
		if (file.is_open())
		{
			file << str;
			if (!strcmp(str, "{") == 0) file.write(",", 1);
			file.write("\"", 1);
			file.write(name, strlen(name));
			file.write("\":\"", 3);
			file.write(value, strlen(value));
			file.write("\"}", 2);
		}
		else std::cerr << "No File While Writing" << std::endl;
		delete[] str;
		return;
	}
	else std::cerr << "No File" << std::endl;
	return;
}
//--------------------------------------------------------------------------------------

// Read from json
//--------------------------------------------------------------------------------------
char*** rJson(char* path)
{
	std::ifstream file;
	file.open(path);

	if (file.is_open())
	{
		char strArr[50][50][50];
		char*** strArr = new char** [fileSize(path)];
		char** group = new char* [fileSize(path)];
		char* name = new char [fileSize(path) / JsonLength(path)];
		char* value = new char [fileSize(path) / JsonLength(path)];
		char ch;
		int i, n = 0;

		while (!file.eof())
		{
			file >> ch;
			if (ch == '"')
			{
				// Name
				file >> ch;
				i = 0;
				while (ch != '"')
				{
					name[i] = ch;
					file >> ch;
					i++;
				}
				// Value
				file >> ch;
				name[i] = '\0';
				i = 0;
				while (ch != ',' && ch != '}')
				{
					file >> ch;
					value[i] = ch;
					i++;
				}
				value[i - 1] = '\0';
				group[0] = name;
				group[1] = value;
				strArr[n] = group;
				n++;
			}
		}
		file.close();
		return strArr;
	}
	else std::cerr << "No File" << std::endl;
	char** Arr[1];
	return Arr;
}
//--------------------------------------------------------------------------------------