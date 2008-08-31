/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_TestTime_h
#define maxmm_TestTime_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class TimeTest : public CppUnit::TestFixture
        {
        
        public:
            void setUp()
            {}
            void tearDown()
            {}
            void testEquality();
            void testSign();
            static CppUnit::TestSuite* getSuite();
        };
    }
}


#endif

