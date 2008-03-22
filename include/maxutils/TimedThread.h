/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_TIMED_THREAD_H
#define MAXUTILS_TIMED_THREAD_H

#include <maxutils/Thread.h>
namespace maxutils
{


    class TimedThread : public Thread
    {
    protected:
        double        m_period;
        
        virtual void  loop() = 0;
    public: 
        TimedThread(double _period);
        ~TimedThread();
        virtual void run(); 
    };


}//namespace maxutils



#endif

