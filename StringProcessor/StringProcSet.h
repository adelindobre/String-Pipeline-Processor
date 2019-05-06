#pragma once

#include <string>
#include <vector>
#include <algorithm>

class StringProcSet
{
public:
	static void Lowercase(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	static void Uppercase(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}

	static void SortAscending(std::string& str)
	{
		std::sort(str.begin(), str.end());
	}

	static void RemoveSpaces(std::string& str)
	{
		std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
		str.erase(end_pos, str.end());
	}

	static void Reverse(std::string& str)
	{
		std::reverse(str.begin(), str.end());
	}

	static void InvertCase(std::string& str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			if (islower(str[i]))
				str[i] = toupper(str[i]);
			if (isupper(str[i]))
				str[i] = tolower(str[i]);
		}
	}
};