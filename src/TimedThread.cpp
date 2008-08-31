/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <maxmm/TimedThread.h>
#include <maxmm/Time.h>

namespace maxmm
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
                // TODO
                // Add error recording to evaluate how much the thread was
                // actually behind the schedule.
                //
                // Add also an enum for the different behavior. ( return
                // immediately or keep on going ) 
                //
            }
            else
            {
                Time::sleep_until( until );
            }
            until  += _period;
            current = Time::now();
        }
    }
}
