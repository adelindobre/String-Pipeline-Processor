#pragma once

#include <queue>
#include <vector>
#include <functional>

#include "BlockingQueue.h"

#define NUMMAXSTAGES 5

template<typename WorkT>
class PipelineStage
{
public:
	PipelineStage(int ID) : thread_ID(ID)
	{
	}

	void InitQueues(std::shared_ptr<BlockingQueue<WorkT>> in_queue,
					std::shared_ptr<BlockingQueue<WorkT>> out_queue)
	{
		m_in_queue = in_queue;
		m_out_queue = out_queue;
	}

	virtual void Run() = 0;

	BlockingQueue<WorkT>& GetInQueue() const { return *m_in_queue; }
	BlockingQueue<WorkT>& GetOutQueue() const { return *m_out_queue; }

	static void Done(bool val, int thread_id) { m_done[thread_id] = val; }
	static bool PreviousDone (int thread_id) { return m_done[thread_id - 1]; }
	
	int ThreadID() const { return thread_ID;  }

	template<typename Function>
	void push_back(Function&& fn)
	{
		operations.push_back(std::forward<Function>(fn));
	}

	void pop_front()
	{
		operations.erase(operations.begin());
	}

public:
	std::vector<std::function<void(WorkT&)>> operations;

private:
	std::shared_ptr<BlockingQueue<WorkT>> m_in_queue;
	std::shared_ptr<BlockingQueue<WorkT>> m_out_queue;

	static std::vector<bool> m_done;
	int thread_ID;
};

std::vector<bool> PipelineStage<std::string>::m_done(NUMMAXSTAGES, false);
