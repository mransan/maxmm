/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ThreadControllerTest_h
#define maxmm_ThreadControllerTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace maxmm
{
    namespace test
    {
        class ThreadControllerTest : public CppUnit::TestFixture
        {
            public:

                void setUp( void );
                void tearDown( void );
                
                void timed_controller_test( void );
                void once_controller_test( void );
                void composite_controller_test( void );
                void condition_controller_test( void );

                static CppUnit::TestSuite* getSuite( void );
        };
    }
}   


#endif
