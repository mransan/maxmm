/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_TestCommon_h
#define maxmm_TestCommon_h

#include <sys/types.h> 
#include <signal.h>

#include <boost/bind.hpp>
#include <algorithm>
#include <functional>

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
        
        template< typename CONTAINER >
                static bool do_not_contain( 
                        const CONTAINER & c , 
                        typename CONTAINER::value_type v  )
        {
            typename CONTAINER::difference_type 
                ret = std::count_if(
                            c.begin( ) , 
                            c.end( ) , 
                            boost::bind(
                                std::not_equal_to< typename CONTAINER::value_type >( ) ,
                                _1 ,  
                                v )  );
            return ( ret == typename CONTAINER::difference_type( 0 ) );
        }
       
    }
}//namespace maxmm


#endif
