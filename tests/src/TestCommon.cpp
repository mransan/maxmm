/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <TestCommon.h>
#include <maxmm/Timer.h>
#include <iostream>
namespace maxmm
{
    namespace test
    {
        uint64_t fibonacci( uint8_t value )
        {
            if( ( value == 0 ) || ( value  == 1 ) )
            {
                return static_cast< uint64_t >( value );
            }
            return fibonacci( value - 1 ) + fibonacci( value - 2);
        }
        
        bool exec_until( boost::function0< bool > predicate , Time t )
        {
            bool success = predicate( );
            if( success == true ){ return true; }
            Timer timer;
            while( ( timer.elapsed( ) ) < t )
            {
                success = predicate( );
                if( success == true )
                {
                    return true;
                }
                Time::sleep( 0.01 );
            }
            return false;
        }
       
    }
}
