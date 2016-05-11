//
// Created by Alexis bayle on 09/05/2016.
//

#ifndef SORTMERGEJOIN_THREADWORK_H
#define SORTMERGEJOIN_THREADWORK_H

#include "ThreadPool.h"

namespace SMJ {
    class ThreadWork {
    public:
        ThreadWork(uint8_t threads);
        ThreadWork(const ThreadWork &obj);

        ~ThreadWork();

        inline void LaunchWork(){
            std::lock_guard<std::mutex> lock(wait_mutex);
            workStarted = true;
            condition.notify_all();
        }

        inline void AddTask(std::function<void()> task){
            tasks.push_back(task);
        }

        inline void WaitEndOfWork(){
            for(auto thread = workers.begin(); thread != workers.end(); ++thread){
                thread->join();
            }
        }

    private:
        std::vector<std::thread> workers;

        std::vector<std::function<void()>> tasks;

        std::condition_variable condition;

        std::mutex wait_mutex;

        bool workStarted;

        long numberOfProcessors;

        void Routine(int id);
    };
}

#endif //SORTMERGEJOIN_THREADWORK_H
