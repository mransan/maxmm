#include "ThreadTest.h"
#include <maxmm/Time.h>
#include <maxmm/Thread.h>
#include <cppunit/TestAssert.h>

namespace maxmm
{
    namespace test
    {

        //
        // Nested Class TThread.
        // --------------------
        //
        
        ThreadTest::TThread::TThread(int i)
        :   _iter( i ),
            Thread<OnceController>( OnceController( ) )
        {
            // No - Op
        }
        
        ThreadTest::TThread::~TThread( void )
        {
            // No - Op
        }
        
        int ThreadTest::TThread::get_iter( void )
        {
            return _iter;
        }
        
        void ThreadTest::TThread::loop( void )
        {
            ++_iter;
        }


        //
        // Test Suite.
        // ---------- 
        //

        void ThreadTest::test_thread_run( void )
        {
            TThread tt(23);
            tt.start();
            tt.join();
            
            CPPUNIT_ASSERT_EQUAL_MESSAGE("thread result", 24, tt.get_iter( ) );
        }
        
       
        void ThreadTest::setUp( void )
        {
            // No - Op.
        }

        void ThreadTest::tearDown( void )
        {
            // No - Op.
        }

        CppUnit::TestSuite* ThreadTest::getSuite()
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<ThreadTest>( 
                        "testSignal", 
                        &ThreadTest::test_thread_run ) );
            return suite;
        }
    }
}
