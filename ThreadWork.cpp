#include "ThreadWork.h"

using namespace std;

namespace SMJ {

    ThreadWork::ThreadWork(uint8_t threads):
        workStarted(false)
    {
        for(uint8_t i = 0; i < threads; ++i){
            workers.push_back(thread(&ThreadWork::Routine, this, i));
        }
    }

    ThreadWork::~ThreadWork() {
        for(auto thread = workers.begin(); thread != workers.end(); ++thread){
            if(thread->joinable()){
                thread->join();
            }
        }
        workers.clear();
    }

    void ThreadWork::WaitEndOfWork() {
        for(auto thread = workers.begin(); thread != workers.end(); ++thread){
            thread->join();
        }
    }

    void ThreadWork::Routine(int id) {
        unique_lock<mutex> lock(wait_mutex);
        condition.wait(lock, [this]{ return workStarted; });
        lock.unlock();
        tasks[id]();
    }

}

