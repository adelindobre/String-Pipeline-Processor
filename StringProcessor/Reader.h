#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "BlockingQueue.h"

bool Readline(std::ifstream& in, std::string& str_line)
{
	return !std::getline(in, str_line);
}

template<typename WorkT>
void Reader(BlockingQueue<WorkT>& queue, std::string filename)
{
	std::ifstream in_file(filename);
	if (in_file)
	{
		WorkT str_line;
		bool is_finished = Readline(in_file, str_line);
		while (!is_finished)
		{
			queue.Add(str_line);
			is_finished = Readline(in_file, str_line);
		}
	}
}