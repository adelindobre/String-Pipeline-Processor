#pragma once

#include <deque>
#include <limits>
#include <mutex>
#include <condition_variable>

template<typename WorkT>
class BlockingQueue
{
public: 
	enum { NoMaxSizeRestriction = 0};

	BlockingQueue()
		: m_maxSize(NoMaxSizeRestriction)
	{
	}

	BlockingQueue(const BlockingQueue& other)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_deque = other.m_deque;
	}

	std::unique_ptr<WorkT> Take()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_data_cond.wait(lock, [this] { return !m_deque.empty(); });
		std::unique_ptr<WorkT> value = std::move(m_deque.back());
		m_deque.pop_back();

		return value;
	}

	bool Add(WorkT const& value)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		if ((m_deque.size() >= m_maxSize) &&
			(NoMaxSizeRestriction != m_maxSize))
		{
			return false;
		}

		m_deque.push_front(std::make_unique<WorkT>(value));
		m_data_cond.notify_one();

		return true;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_deque.empty();
	}

	void SetMaxSize(uint64_t value)
	{
		m_maxSize = value;
	}

	uint64_t GetMaxSize() const
	{
		return m_maxSize;
	}

	size_t GetCurrentSize() const
	{
		return m_deque.size();
	}


private:
	uint64_t m_maxSize;
	std::deque<std::unique_ptr<WorkT>> m_deque;
	mutable std::mutex m_mutex;
	std::condition_variable m_data_cond;
};