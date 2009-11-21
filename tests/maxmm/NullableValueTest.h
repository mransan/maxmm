/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_NullableValueTest_h
#define maxmm_test_NullableValueTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class NullableValueTest : CppUnit::TestFixture 
        {
            public:
                NullableValueTest(void);
                ~NullableValueTest(void);
           
                void setUp(void);
                void tearDown(void);

                void test_simple(void); 
                static CppUnit::TestSuite* getSuite(void);

            private:
        };
    }
}

#endif
