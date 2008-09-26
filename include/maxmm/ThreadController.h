/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ThreadController_h
#define maxmm_ThreadController_h

#include <maxmm/Time.h>

namespace maxmm
{
    
    class NoWaitController
    {
         public:
            bool execute( void )
            {
               return true;    
            }
    };

    class OnceController
    {
        public:
            OnceController( void )
            :   _run( true )
            {
                    
            }

            bool execute( void )
            {
                bool ret = _run;
                _run = false;
                return ret;
            }
        private:
            bool _run;
    };

    class TimedController
    {
        public:
            TimedController( const Time &period )
            :   _period( period ),
                _tick( 0 )
            {
                
            }

            bool execute( void )
            {
                Time now = Time::now( );
                if( _tick == 0 )
                {
                    _tick = now +  _period;
                    return true;
                }
                Time::sleep_until( _tick );
                _tick += _period;
            }
        private:
            Time _period;
            Time _tick;
    };
}



#endif
