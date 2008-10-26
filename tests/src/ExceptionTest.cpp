/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <iomanip>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <ExceptionTest.h>


namespace maxmm
{
    namespace test
    {
        
        ExceptionTest::ExceptionTest( void )
        { }

        ExceptionTest::~ExceptionTest( void )
        { }
    
        void ExceptionTest::setUp( void )
        { }

        void ExceptionTest::tearDown( void )
        { }
        
        void ExceptionTest::test_inheritance( void )
        {
            {
                int i = 0;

                try
                { 
                    throw std::range_error("error");
                }
                catch( std::runtime_error& )
                {
                    i = 1;
                }
                catch( std::exception&  )
                {
                    i = 2;
                }

                CPPUNIT_ASSERT_EQUAL( int( 1 ) , i );
            }
            {
                int i = 0;

                try
                { 
                    throw std::range_error("error");
                }
                catch( std::exception e )
                {
                    i = 1;
                }
                catch( std::runtime_error e  )
                {
                    i = 2;
                }

                CPPUNIT_ASSERT_EQUAL( int( 1 ) , i );
            }
            {
                int i = 0;

                try
                { 
                    throw new std::range_error("error");
                }
                catch( std::runtime_error * )
                {
                    i = 1;
                }
                catch( std::exception *  )
                {
                    i = 2;
                }

                CPPUNIT_ASSERT_EQUAL( int( 1 ) , i );
            }
            {
                int i = 0;

                try
                { 
                    throw std::string("error");
                }
                catch( std::runtime_error& )
                {
                    i = 1;
                }
                catch( std::exception&  )
                {
                    i = 2;
                }
                catch( std::string & )
                {
                    i = 3 ;
                }
                CPPUNIT_ASSERT_EQUAL( int( 3 ) , i );
            }
        }
        
        CppUnit::TestSuite *ExceptionTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<ExceptionTest>(
                    "ExceptionTest::test_inheritance",
                    &ExceptionTest::test_inheritance) );
           
             return suite; 
        }
    }
}
