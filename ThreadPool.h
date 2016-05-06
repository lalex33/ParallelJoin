#ifndef SORTMERGEJOIN_THREADPOOL_H
#define SORTMERGEJOIN_THREADPOOL_H

// https://gist.github.com/alagafonov

#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <unistd.h>

using namespace std;

class ThreadPool
{
public:

    // Constructor.
    ThreadPool(int threads);

    // Destructor.
    ~ThreadPool();

    // Adds task to a task queue.
    void Enqueue(function<void()> f);

    // Shut down the pool.
    void ShutDown();

    void Resize(uint8_t threads);

    bool IsWorkFinished();

private:
    // Thread pool storage.
    vector<thread> threadPool;

    // Queue to keep track of incoming tasks.
    queue<function<void()>> tasks;

    // Task queue mutex.
    mutex tasksMutex;

    // Number of tasks running
    uint running;

    // Condition variable.
    condition_variable condition;

    // Indicates that pool needs to be shut down.
    bool terminate;

    // Indicates that pool has been terminated.
    bool stopped;

    // Function that will be invoked by our threads.
    void Invoke();
};


#endif //SORTMERGEJOIN_THREADPOOL_H
