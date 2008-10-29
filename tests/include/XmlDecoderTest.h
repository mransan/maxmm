/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_XmlDecoderTest_h
#define maxmm_test_XmlDecoderTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class XmlDecoderTest : CppUnit::TestFixture 
        {
            public:
                XmlDecoderTest( void );
                ~XmlDecoderTest( void );
           
                void setUp( void );
                void tearDown( void );

                void test_primitive( void );
                void test_classlike( void );
                void test_rootnode( void );
                void test_sequence( void );

                static CppUnit::TestSuite* getSuite( void );

            private:
        };
    }
}

#endif
