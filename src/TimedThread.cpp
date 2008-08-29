/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <maxutils/TimedThread.h>
#include <maxutils/Logger.h>
#include <maxutils/Time.h>

namespace maxutils
{

    TimedThread::TimedThread( double  period ) 
    : Thread::Thread( ),
      _period( period )
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
        Time until   = current + _period;
        
        while( ! should_stop( ) )
        {
            this->loop( );        
            current = Time::now( );
            if( current >= until )
            {
                LOG_WARNING << "thread cannot keep up with period of " 
                            << _period
                            << " sec"
                            << std::endl;
            }
            Time::sleep_until( until );
            until  += _period;
            current = Time::now();
        }
    }
}
