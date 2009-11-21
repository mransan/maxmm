/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_RandomUniformTest_h
#define maxmm_RandomUniformTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxmm/Random.h>

namespace maxmm
{
    namespace test
    {

        class RandomUniformTest : public CppUnit::TestFixture
        {
            public:
                void setUp( void ) ;
                void tearDown( void );

                void test_binary_generation( void );
                void test_10nb_generation( void );
               
                static CppUnit::TestSuite* getSuite( void );
        };
    }
}
#endif
