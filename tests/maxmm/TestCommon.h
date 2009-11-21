/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_TestCommon_h
#define maxmm_TestCommon_h

#include <sys/types.h> 
#include <signal.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <functional>

#include <maxmm/Time.h>

namespace maxmm
{
    namespace test
    {
                
        
        uint64_t fibonacci( uint8_t value );
        
        
                

        bool exec_until( boost::function0< bool > predicate , Time t );
    }
}//namespace maxmm


#endif
