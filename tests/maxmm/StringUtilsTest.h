/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_StringUtilsTest_h
#define maxmm_StringUtilsTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class StringUtilsTest : public CppUnit::TestFixture
        {
            public:
            
                StringUtilsTest( void )
                { }

                ~StringUtilsTest( void )
                { }

                void setUp( void )
                { }

                void tearDown( void )
                { }

                void test_tokenise( void );
                
                static CppUnit::TestSuite* getSuite( void );

        };


    }
}



#endif
