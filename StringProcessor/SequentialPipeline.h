#pragma once

#include <vector>

#include "BlockingQueue.h"
#include "PipelineStage.h"
#include "Reader.h"

template<typename WorkT>
class SequentialPipeline
{
public:
	void AddStage(std::shared_ptr< PipelineStage<WorkT> > stage)
	{
		m_stages.push_back(stage);
		size_t num_stage = m_stages.size();

		m_queues.resize(num_stage);

		m_queues[num_stage - 1] =
			std::shared_ptr<BlockingQueue<WorkT>>(
				new BlockingQueue<WorkT>()
				);

		if (num_stage == 1)
			m_stages[num_stage - 1]->InitQueues(nullptr, m_queues[num_stage - 1]);
		else if(num_stage == NUMMAXSTAGES)
			m_stages[num_stage - 1]->InitQueues(m_queues[num_stage - 2], nullptr);
		else
			m_stages[num_stage - 1]->InitQueues(m_queues[num_stage - 2], m_queues[num_stage - 1]);
	}

	void Start()
	{
		for (size_t i = 0; i < m_stages.size(); ++i)
		{
			m_threads.push_back(
				std::shared_ptr<std::thread>(new std::thread(
						&SequentialPipeline<WorkT>::StartStage, this, i))
			);
		}
	}

	template<typename Function>
	void AddOperationOnStage(int index, Function&& fn)
	{
		m_stages[index]->push_back(fn);
	}

	void RemoveOperationOnStage(int index)
	{
		m_stages[index]->pop_front();
	}

	void Join()
	{
		for (size_t i = 0; i < m_stages.size(); ++i)
		{
			m_threads[i]->join();
		}
	}

	void PrintResult()
	{
		std::cout << "Number of stages: " 
					<< m_stages.size() << std::endl;
		std::cout << "Number of queues: " 
					<< m_queues.size() << std::endl;

		for(size_t i = m_queues.size(); i > 0; i--)
		{
			std::cout << "Number of elements in queue "
					<< m_queues.size() - i  
					<< ": " << m_queues[m_queues.size() - i]->GetCurrentSize() 
					<< std::endl;
		}

		std::cout << std::endl << "Processed file: "
					<< std::endl << std::endl;

		while (!m_queues[m_queues.size() - 1]->empty())
		{
			std::cout << *(m_queues[m_queues.size() - 1]->Take().get()) << std::endl;
		}
	}

private:

	void StartStage(size_t index)
	{
		m_stages[index]->Run();
	}

private:
	std::vector < std::shared_ptr<PipelineStage<WorkT>> > m_stages;
	std::vector < std::shared_ptr<BlockingQueue<WorkT>> > m_queues;
	std::vector < std::shared_ptr<std::thread> > m_threads;
};