/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_Condition_h
#define maxmm_Condition_h

#include <boost/thread/condition.hpp>
#include <boost/utility.hpp>

#include <maxmm/Mutex.h>

namespace maxmm
{
//! \brief encapsulate Condition for thread synchronization.
//!
class Condition : boost::noncopyable
{
private:
    boost::condition _condition;
    Mutex&       _mutex;

public:
    //! \brief constructor.
    //!
    //! \param lock the lock that the condition should work with.
    //!
    Condition(Mutex& mutex);
    
    
    //! \brief destructor.
    //!
    ~Condition(void);   
    
    //! \brief block until the condition is raised.
    //!
    void wait(void);
    
    //! \brief raise the condition to every thread waiting.
    //!
    void broadcast(void);

    void notify_one(void) ;
    
    //! \brief return  the lock that the condition is bind to.
    //!
    //! \return the lock that the condition is bind to.
    //!
    Mutex& mutex(void);
};

}//namespace utils
#endif
