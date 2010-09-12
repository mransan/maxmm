/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_ThreadPoolTest_h
#define maxmm_ThreadPoolTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        
        class ThreadPoolTest : public CppUnit::TestFixture
        {
            public:

                void setup( void ) { }
                void tearDown( void ) { }

                void test_single_thread( void );
                void test_multi_thread( void );

                static CppUnit::TestSuite* getSuite( void );
            private:

        };
    
    }
}



#endif
