/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_ExceptionTest_h
#define maxmm_test_ExceptionTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class ExceptionTest : CppUnit::TestFixture 
        {
            public:
                ExceptionTest( void );
                ~ExceptionTest( void );
           
                void setUp( void );
                void tearDown( void );

                void test_inheritance( void );
                static CppUnit::TestSuite* getSuite( void );

            private:
        };
    }
}

#endif
