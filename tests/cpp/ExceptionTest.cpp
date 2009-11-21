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

#include <boost/bind.hpp>

namespace
{
    struct NoThrow
    {
        void nothrow_test( void ) throw( std::bad_exception )
        {
            throw std::string( "boom" );
        }
        
        void unexpected_test( void ) throw( std::bad_exception , int )
        {
            throw std::string( "boom" );
        }
        static void on_unexpected( void )
        {
            ++unexpected_called ;
            throw;
        }

        static uint8_t unexpected_called;
    };

    uint8_t NoThrow::unexpected_called = 0;

    struct UnexpectedReseter
    {
            UnexpectedReseter(
                void(* on_unexpected)( void ) )
            :   _previous_unexpected( std::set_unexpected( on_unexpected ) )
            { }
            
            ~UnexpectedReseter( void )
            {
                std::set_unexpected( _previous_unexpected ); 
            }
        private:
            void(* _previous_unexpected)( void );
    };
}

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
        
        void ExceptionTest::test_nothrow( void )
        {
            
            NoThrow object;
            std::set_unexpected( &NoThrow::on_unexpected );
            uint8_t calls = NoThrow::unexpected_called;
            try
            {
                object.nothrow_test( );
            }
            catch( std::bad_exception& )
            {
                CPPUNIT_ASSERT( true );
            }
            catch( std::exception& )
            {
                CPPUNIT_ASSERT( false );
            }
            CPPUNIT_ASSERT_EQUAL( uint8_t( calls + 1) , NoThrow::unexpected_called );

        }

        void ExceptionTest::test_unexpected( void )
        {   
            NoThrow object;
            uint8_t calls = NoThrow::unexpected_called;
            UnexpectedReseter reseter( &NoThrow::on_unexpected );
            try
            {
                object.unexpected_test( );
            }
            catch( std::bad_exception& )
            {
                CPPUNIT_ASSERT( true );
            }
            catch( std::exception& )
            {
                CPPUNIT_ASSERT( false );
            }
            CPPUNIT_ASSERT_EQUAL( uint8_t( calls + 1) , NoThrow::unexpected_called );
        }
        CppUnit::TestSuite *ExceptionTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<ExceptionTest>(
                    "ExceptionTest::test_inheritance",
                    &ExceptionTest::test_inheritance) );
             suite->addTest(
               new CppUnit::TestCaller<ExceptionTest>(
                   "ExceptionTest::test_nothrow",
                   &ExceptionTest::test_nothrow) );
             suite->addTest(
                new CppUnit::TestCaller<ExceptionTest>(
                    "ExceptionTest::test_unexpected",
                    &ExceptionTest::test_unexpected) );
         
             return suite; 
        }
    }
}
