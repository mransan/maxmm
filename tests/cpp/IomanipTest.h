/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_IomanipTest_h
#define maxmm_test_IomanipTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class IomanipTest : CppUnit::TestFixture 
        {
            public:
                IomanipTest( void );
                ~IomanipTest( void );
           
                void setUp( void );
                void tearDown( void );

                void test_setbase( void );
                void test_setprecision( void );    
                static CppUnit::TestSuite* getSuite( void );

            private:
        };
    }
}



#endif
