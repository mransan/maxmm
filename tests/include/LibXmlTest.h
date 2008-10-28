/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_LibXmlTest_h
#define maxmm_test_LibXmlTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class LibXmlTest : CppUnit::TestFixture 
        {
            public:
                LibXmlTest( void );
                ~LibXmlTest( void );
           
                void setUp( void );
                void tearDown( void );

                void test_simple( void );
                
                static CppUnit::TestSuite* getSuite( void );

            private:
        };
    }
}

#endif
