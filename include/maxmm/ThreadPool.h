/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ThreadPool_h
#define maxmm_ThreadPool_h

#include <boost/function.hpp>

#include <maxmm/Thread.h>
#include <maxmm/ThreadController.h>
#include <maxmm/Condition.h>


namespace maxmm { namespace test { class ThreadPoolTest; } }
namespace maxmm { class Mutex; }
namespace maxmm { class ScopeLock; }

namespace maxmm
{
    
class IWork
{
public:
    virtual ~IWork(void) { };
    virtual void execute(void) = 0 ;
};

class ThreadPool
{
friend class maxmm::test::ThreadPoolTest;
public:
    
    ThreadPool(int nb_thread);
    ~ThreadPool(void);

    bool append_work(IWork* work);

    IWork *get_next_work(void);

    void stop(void);

    void start(void);

    std::vector< uint64_t > execution_stats(void) const ;
private:
    class Thread : public maxmm::Thread< ConditionController >
    { 
        public :
            Thread(Condition &condition , ThreadPool &thread_pool);
            virtual ~Thread(void);
            virtual void loop(void);
            virtual void init(void);
            virtual void clean(void);
            
            uint64_t nb_works(void) const ;
        private:
            ThreadPool &_thread_pool;
            uint64_t _work_counter;
    };

    std::vector< ThreadPool::Thread* > _threads;
    Mutex _mutex;
    Condition _condition;

    Mutex _works_mtx;
    std::list< IWork* > _works;

    bool _stoped;
};
}


#endif
