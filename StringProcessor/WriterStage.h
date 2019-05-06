#pragma once

#include <iostream>
#include <fstream>

#include "PipelineStage.h"

class WriterStage : public PipelineStage<std::string>
{
public:
	WriterStage(int ID, std::string filename) : PipelineStage(ID)
	{
		this->filename = filename;
	}

	virtual void Run() override
	{
		BlockingQueue<std::string>& inqueue = GetInQueue();
		std::ofstream os(filename);

		while (1)
		{
			if (!inqueue.empty())
			{
				std::unique_ptr<std::string> str_p = inqueue.Take();
				if (os)
				{
					os << *str_p.get() << std::endl;
				}
			}
			else
			{
				if (!PipelineStage::PreviousDone(this->ThreadID()))
					continue;
				else
					break;
			}
		}
		this->Done(true, this->ThreadID());
	}

private:
	std::string filename;
};
