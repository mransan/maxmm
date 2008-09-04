/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_TestCommon_h
#define maxmm_TestCommon_h

#include <sys/types.h> 
#include <signal.h>
namespace maxmm
{
    namespace test
    {
        class ProcessKiller
        {
            public:
                ProcessKiller( ::pid_t pid )
                {
                    _pid = pid ;
                }
                ~ProcessKiller( void )
                {
                    static_cast< void >( ::kill( _pid , SIGINT ) );
                }
            private:
                ::pid_t _pid;
        };
    }
}//namespace maxmm


#endif
