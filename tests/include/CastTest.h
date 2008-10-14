/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_CastTest_h
#define maxmm_test_CastTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class CastTest : CppUnit::TestFixture 
        {
            public:
                CastTest( void );
                ~CastTest( void );
           
                void setUp( void );
                void tearDown( void );

                void test_static_cast( void );
                void test_reinterpret_cast( void );
                void test_primitive_type_cast( void );
                void test_const_cast( void );
                static CppUnit::TestSuite* getSuite( void );

            private:
        };
    }
}



#endif
