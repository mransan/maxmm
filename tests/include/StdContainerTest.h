/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_StdContainerTest_h
#define maxmm_StdContainerTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace maxmm
{
    namespace test
    {
        class StdContainerTest :  public CppUnit::TestFixture 
        {
            public:
                void setUp( void );
                void tearDown( void );
                
                void test_vector_creation( void );
                static CppUnit::TestSuite* getSuite( void );

            private:

        };
    }
}

#endif
