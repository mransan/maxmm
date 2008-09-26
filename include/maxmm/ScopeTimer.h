/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ScopeTimer_h
#define maxmm_ScopeTimer_h

#include <maxmm/Time.h>

namespace maxmm
{
    class ScopeTimer
    {
        public:

            
            class Ressource
            {
                public:
                    Ressource( maxmm::ScopeTimer &timer)
                    :   _timer( timer )
                    {
                        _timer.start( );
                    }
                    ~Ressource( void )
                    {
                        _timer.end( );
                    }
                private:
                    maxmm::ScopeTimer &_timer;
            };

            friend class Ressource;

            ScopeTimer( void )
            :   _start( 0 ) , 
                _end( 0 )
            {
                
            }
            Time elapsed( void )
            {
                return _end - _start; 
            }
            
            Ressource ressource( void )
            {
                return Ressource( *this );
            }
        private:
            
            void start( void )
            {
                _start = Time::now( );    
            }
            void end( void )
            {
                _end = Time::now( );
            }
            
            Time _start;
            Time _end;
        
        public:            
    };

}


#endif
