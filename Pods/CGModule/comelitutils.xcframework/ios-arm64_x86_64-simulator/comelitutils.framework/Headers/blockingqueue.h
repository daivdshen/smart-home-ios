#ifndef UTILS_BLOCKINGQUEUE_H
#define UTILS_BLOCKINGQUEUE_H

#include <mutex>
#include <queue>
#include "utils.h"

namespace Comelit {
namespace Utils   {

template <typename T>
/**
 * @brief The BlockingQueue class
 * handle queued data exchange between threads
 * on thread enqueue data, another thread process data
 */
class BlockingQueue
{
public:
	/**
	 * @brief remove element from the queue
	 * if the queue is empty, blocks until the next push
	 * @return
	 */
	T pop()
	{
		std::unique_lock<std::mutex> mlock(mutex);

		while (queue.empty())
		{
			cond.wait(mlock);
		}

		auto item = std::move(queue.front());
		queue.pop();
		return item;
	}

	/**
	 * @brief remove element from the queue
	 * if the queue is empty, blocks until the next push, or until the specified timeout
	 * it the queue is still empty afetr the timeout, timeoutVal is returned
	 * @param milliseconds timeout to waint in milliseconds
	 * @param timeoutVal value to return in case of timeout
	 * @return
	 */
	T pop(unsigned int milliseconds, T &&timeoutVal)
	{
		std::unique_lock<std::mutex> mlock(mutex);

		if (queue.empty())
		{
			cond.wait_for(mlock, std::chrono::milliseconds(milliseconds));
		}

		if (queue.empty())
		{
			return std::move(timeoutVal);
		}

		auto item = std::move(queue.front());
		queue.pop();
		return item;
	}

	/**
	 * @brief insert element into the queue
	 * @param item const reference
	 */
	void push(const T &item)
	{
		std::unique_lock<std::mutex> mlock(mutex);
		queue.push(item);
		mlock.unlock();
		cond.notify_one();
	}
	/**
	 * @brief insert element into the queue
	 * @param item rvalue reference
	 */
	void push(T &&item)
	{
		std::unique_lock<std::mutex> mlock(mutex);
		queue.push(std::move(item));
		mlock.unlock();
		cond.notify_one();
	}
	/**
	 * @brief empty
	 * @return true if the queue is empty
	 */
	bool empty()
	{
		std::lock_guard<std::mutex> mlock(mutex);
		return queue.empty();
	}
	/**
	 * @brief size
	 * @return the queue size
	 */
	size_t size()
	{
		std::lock_guard<std::mutex> mlock(mutex);
		return queue.size();
	}

private:
	std::queue<T> queue;
	std::mutex mutex;
	Comelit::Utils::steady_condition_variable cond;
};

} // Utils NS
} // Comelit  NS

#endif // UTILS_BLOCKINGQUEUE_H
