/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <TestCommon.h>
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
    }
}
