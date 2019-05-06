#pragma once

#include <chrono>
#include <thread>

#include "PipelineStage.h"

class ProcessStage : public PipelineStage<std::string>
{
public:
	ProcessStage(int ID) : PipelineStage(ID)
	{
	}

	virtual void Run() override
	{
		BlockingQueue<std::string>& inqueue = GetInQueue();
		BlockingQueue<std::string>& outqueue = GetOutQueue();

		while (1)
		{
			if (!inqueue.empty())
			{
				std::unique_ptr<std::string> str_p = inqueue.Take();

				for (auto&& fn : operations)
				{
					fn(*str_p.get());
				}
				outqueue.Add(*str_p.get());
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
};