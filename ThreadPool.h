#ifndef SORTMERGEJOIN_THREADPOOL_H
#define SORTMERGEJOIN_THREADPOOL_H

// https://gist.github.com/alagafonov
// https://github.com/nbsdx/ThreadPool/blob/master/ThreadPool.h

#include "SortMergeJoin.h"

namespace SMJ {
    class ThreadPool {
    public:

        // Constructor.
        ThreadPool(int threads);

        // Destructor.
        ~ThreadPool();

        // Adds task to a task queue.
        void Enqueue(std::function<void()> f);

        // Shut down the pool.
        void ShutDown();

        void Resize(uint8_t threads);

        void WaitEndOfWork();

    private:
        // Thread pool storage.
        std::vector<std::thread> threadPool;

        // Queue to keep track of incoming tasks.
        std::queue<std::function<void()>> tasks;

        // Task queue mutex.
        std::mutex tasksMutex;

        // Condition variable.
        std::condition_variable condition;

        // Number of tasks running
        std::atomic_uint running;

        // Condition variable for waiting end of all tasks
        std::condition_variable wait_var;

        // Waiting mutex
        std::mutex wait_mutex;

        // Indicates that pool needs to be shut down.
        bool terminate;

        // Indicates that pool has been terminated.
        bool stopped;

        // Function that will be invoked by our threads.
        void Invoke();
    };
}

#endif //SORTMERGEJOIN_THREADPOOL_H
