/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <ThreadControllerTest.h>

#include <maxmm/ThreadController.h>
#include <maxmm/ScopeTimer.h>

namespace maxmm
{
    namespace test
    {
        void ThreadControllerTest::setUp( void )
        {
            // No - Op
        }

        void ThreadControllerTest::tearDown( void )
        {
            // No - Op.
        }

        void ThreadControllerTest::timed_controller_test( void )
        {
            maxmm::TimedController controller( 1 );
            
            ScopeTimer timer;
            {
                ScopeTimer::Ressource res = timer.ressource( );
                controller.execute( );
            }
            CPPUNIT_ASSERT( timer.elapsed( ) < 0.1 );
            {
                ScopeTimer::Ressource res = timer.ressource( );
                controller.execute( );
            }
            
            CPPUNIT_ASSERT( timer.elapsed( ) > 0.9 );
            CPPUNIT_ASSERT( timer.elapsed( ) < 1.1 );
            
            {
                ScopeTimer::Ressource res = timer.ressource( );
                controller.execute( );
            }
            CPPUNIT_ASSERT( timer.elapsed( ) > 0.9 );
            CPPUNIT_ASSERT( timer.elapsed( ) < 1.1 );

            return ;    
        }
        
        void ThreadControllerTest::once_controller_test( void )
        {
            OnceController controller;
            CPPUNIT_ASSERT_EQUAL( true , controller.execute( ) );
            CPPUNIT_ASSERT_EQUAL( false , controller.execute( ) );
            
            return;     
        }

        CppUnit::TestSuite* ThreadControllerTest::getSuite( void )
        {
            
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( 
                new CppUnit::TestCaller<ThreadControllerTest>( 
                    "ThreadControllerTest::timed_controller_test", 
                    &ThreadControllerTest::timed_controller_test ) );
            suite->addTest( 
                new CppUnit::TestCaller<ThreadControllerTest>( 
                    "ThreadControllerTest::once_controller_test", 
                    &ThreadControllerTest::once_controller_test ) );
           
            return suite;
        }

    }
}
