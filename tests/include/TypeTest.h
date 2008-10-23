/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_test_TypeTest_h
#define maxmm_test_TypeTest_h


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace maxmm
{
    namespace test
    {
        class TypeTest : CppUnit::TestFixture 
        {
            public:
                TypeTest( void );
                ~TypeTest( void );
           
                void setUp( void );
                void tearDown( void );

                void test_static_cast( void );
                void test_reinterpret_cast( void );
                void test_primitive_type_cast( void );
                void test_const_cast( void );
                void test_const_ptr( void );
                
                //! \brief test function pointer syntax for both C and C++
                //!
                //! example were done using the following tutorial
                //! http://www.newty.de/fpt/index.html
                //!
                void test_function_ptr( void );

                static CppUnit::TestSuite* getSuite( void );

            private:
        };
    }
}



#endif
