#include "ThreadWork.h"

using namespace std;

namespace SMJ {

    ThreadWork::ThreadWork(uint8_t threads):
        workStarted(false)
    {
        numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);

        #ifdef __linux__
            cpu_set_t cpuset;
        #endif

        for(uint8_t i = 0; i < threads; ++i){
            workers.push_back(thread(&ThreadWork::Routine, this, i));

            #ifdef __linux__
                CPU_ZERO(&cpuset);
                CPU_SET( (i+1) % numberOfProcessors, &cpuset);
                pthread_setaffinity_np(workers[i].native_handle(), sizeof(cpu_set_t), &cpuset);
            #endif
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

    void ThreadWork::Routine(int id) {
        double start = sec();
        #ifdef __linux__
            //cout << "ID : " << pthread_self() << ", CPU : " << sched_getcpu() << endl;
        #endif
        unique_lock<mutex> lock(wait_mutex);
        condition.wait(lock, [this]{ return workStarted; });
        //cout << "       " << this_thread::get_id() << " wait : " << (sec() - start) << endl;
        lock.unlock();
        tasks[id]();
    }

}

