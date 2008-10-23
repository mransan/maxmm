/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <iomanip>
#include <iostream>

#include <IomanipTest.h>

namespace
{
    struct FlagReseter
    {
            FlagReseter( const std::string& str )
            :   _flags( std::cout.flags( ) )
            { 
                std::cout << str << std::endl;
            }

            ~FlagReseter( void )
            {
                std::cout.setf(
                    static_cast< std::ios_base::fmtflags >( _flags ) , 
                    static_cast< std::ios_base::fmtflags >( 0xFFFFFFFF ) );
                std::cout << std::endl;
            }
        private:
            
            std::ios_base::fmtflags _flags;
    };
}
namespace maxmm
{
    namespace test
    {
        
        IomanipTest::IomanipTest( void )
        { }

        IomanipTest::~IomanipTest( void )
        { }
    
        void IomanipTest::setUp( void )
        { }

        void IomanipTest::tearDown( void )
        { }

        void IomanipTest::test_setbase( void )
        {
            std::cout << std::endl;
            //
            // This shows how to manipulate base using constant.
            //
            // REMEMBER: stream manipulators are in the std namespace.
            //
            {
                FlagReseter reseter( "base manipulation using manipulator" );
                uint16_t i = 20;
            
                std::cout   << std::dec 
                            << std::showbase 
                            << i << std::endl;
                
                std::cout   << std::hex  
                            << std::showbase
                            << i << std::endl;
                
                // the std::hex and std::showbase are persistent.
                std::cout   << i << std::endl;
                
                std::cout.unsetf( std::ios::hex );
                std::cout << i << std::endl;
                
                std::cout   << std::oct 
                            << std::showbase
                            << i << std::endl;
            } 
            
            //
            // Same example using stream operator.
            //
            // Remember 
            //  * stream operator are in the std namespace.
            //  * stream flags are in the std::ios namespace.
            // 
            {
                FlagReseter reseter( "base manipulation using parametrized manipulator" );
                
                uint16_t i = 20;
            
                std::cout   << std::setiosflags( std::ios::dec )
                            << std::setiosflags( std::ios::showbase )
                            << i << std::endl;
                
                std::cout   << std::resetiosflags( std::ios::dec )
                            << std::setiosflags( std::ios::hex )
                            << i << std::endl;
                
                std::cout   << i << std::endl;
                
                std::cout   << std::resetiosflags( std::ios::hex ) 
                            << std::setiosflags( std::ios::oct )
                            << i << std::endl;
            } 
            
            //
            // Same example using member functions..
            // 
            {
                FlagReseter reseter( "base manipulation using stream member function" );
                
                uint16_t i = 20;
                
                std::cout.setf( std::ios::dec | std::ios::showbase );
                std::cout   << i << std::endl;
                
                std::cout.unsetf( std::ios::dec );
                std::cout.setf( std::ios::hex );
                std::cout   << i << std::endl;
                
                std::cout   << i << std::endl;
                
                std::cout.unsetf( std::ios::hex );
                std::cout.setf( std::ios::oct );
                std::cout   << i << std::endl;
            } 
        }
            
        void IomanipTest::test_setprecision( void )
        {
            std::cout << std::endl;

            {
                FlagReseter reseter( "set precision with parametrized manipulator" );

                double d = 1.1111;
                
                std::cout << std::setprecision( 2 ) << d << std::endl;
            }
            {
                FlagReseter reseter( "set precision with stream member function" );

                double d = 1.1111;

                std::cout.precision( 2 ) ;
                std::cout << d << std::endl;
            }    

        }

    
        CppUnit::TestSuite *IomanipTest::getSuite( void )
        {
             CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
             suite->addTest(
                new CppUnit::TestCaller<IomanipTest>(
                    "IomanipTest::test_setbase",
                    &IomanipTest::test_setbase) );
             suite->addTest(
                new CppUnit::TestCaller<IomanipTest>(
                    "IomanipTest::test_setprecision",
                    &IomanipTest::test_setprecision) );
            
             return suite; 
        }
    
    }
}
