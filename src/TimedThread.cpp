/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <maxutils/TimedThread.h>
#include <maxutils/Logger.h>
#include <maxutils/Time.h>

using namespace maxutils;

TimedThread::TimedThread(double _period) 
: Thread::Thread(),
  m_period(_period)
{
    // No - Op
}

TimedThread::~TimedThread()
{
    // No - Op
}

void TimedThread::run()
{
    Time current = Time::now();
    Time until   = current + m_period;
    while (! shouldStop())
    {
        this->loop();        
        current = Time::now();
        if (current >= until)
        {
            LOG_WARNING << "thread cannot keep up with period of " 
                        << m_period
                        << " sec"
                        << std::endl;
        }
        Time::sleepUntil(until);
        until  += m_period;
        current = Time::now();
    }
}
