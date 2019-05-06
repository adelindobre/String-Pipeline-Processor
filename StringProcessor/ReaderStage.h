#pragma once

#include "PipelineStage.h"
#include "Reader.h"

class ReaderStage : public PipelineStage<std::string>
{
public:
	ReaderStage(int ID, std::string filename) : PipelineStage(ID)
	{
		this->filename = filename;
	}

	virtual void Run() override
	{
		BlockingQueue<std::string>& out_queue = GetOutQueue();
		Reader<std::string>(out_queue, this->filename);
		this->Done(true, this->ThreadID());
	}

private:

	std::string filename;
};
