// https://gist.github.com/alagafonov

#include "ThreadPool.h"

// Constructor.
ThreadPool::ThreadPool(int threads) :
        terminate(false),
        stopped(false), running(0)
{
    // Create number of required threads and add them to the thread pool vector.
    for(int i = 0; i < threads; i++)
    {
        threadPool.emplace_back(thread(&ThreadPool::Invoke, this));
    }
}

void ThreadPool::Enqueue(function<void()> f)
{
    // Scope based locking.
    {
        // Put unique lock on task mutex.
        unique_lock<mutex> lock(tasksMutex);

        // Push task into queue.
        tasks.push(f);
    }

    // Wake up one thread.
    condition.notify_one();
}

void ThreadPool::Invoke() {

    function<void()> task;
    while(true)
    {
        // Scope based locking.
        {
            // Put unique lock on task mutex.
            unique_lock<mutex> lock(tasksMutex);

            // Wait until queue is not empty or termination signal is sent.
            condition.wait(lock, [this]{ return !tasks.empty() || terminate; });

            // If termination signal received and queue is empty then exit else continue clearing the queue.
            if (terminate && tasks.empty())
            {
                return;
            }

            // Get next task in the queue.
            task = tasks.front();

            // Remove it from the queue.
            tasks.pop();
        }

        // Execute the task.
        task();
        --running;
    }
}

void ThreadPool::ShutDown()
{
    // Scope based locking.
    {
        // Put unique lock on task mutex.
        unique_lock<mutex> lock(tasksMutex);

        // Set termination flag to true.
        terminate = true;
    }

    // Wake up all threads.
    condition.notify_all();

    // Join all threads.
    for(auto thread = threadPool.begin(); thread != threadPool.end(); ++thread)
    {
        thread->join();
    }

    // Empty workers vector.
    threadPool.clear();

    // Indicate that the pool has been shut down.
    stopped = true;
}

// Destructor.
ThreadPool::~ThreadPool()
{
    if (!stopped)
    {
        ShutDown();
    }
}

void ThreadPool::Resize(uint8_t threads) {
    if(!stopped) {
        ShutDown();
    }

    for(int i = 0; i < threads; i++)
    {
        threadPool.emplace_back(thread(&ThreadPool::Invoke, this));
    }

    terminate = false;
    stopped = false;
    running = 0;
}

bool ThreadPool::IsWorkFinished() {
    return (running == 0) && tasks.empty();
}