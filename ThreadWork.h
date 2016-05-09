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

        ~ThreadWork();

        inline void LaunchWork(){
            condition.notify_all();
        }

        inline void setTasks(std::vector<std::function<void()>>& functions){
            tasks = move(functions);
        }

        void WaitEndOfWork();

    private:
        std::vector<std::thread> workers;

        std::vector<std::function<void()>> tasks;

        std::condition_variable condition;

        std::mutex wait_mutex;

        void Routine(int id);
    };
}

#endif //SORTMERGEJOIN_THREADWORK_H
