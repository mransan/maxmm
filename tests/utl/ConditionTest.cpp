#include "ConditionTest.h"
#include <maxmm/Time.h>

#include <cppunit/TestAssert.h>

namespace maxmm
{
    namespace test
    {

        ConditionTest::ThreadTest::ThreadTest( Condition & condition, int id ) 
        :   _condition( condition ), 
            _id( id ) , 
            _iter( 0 ), 
            Thread<OnceController>( OnceController( ) )
        {
        
        }
        ConditionTest::ThreadTest::~ThreadTest( void )
        {
        
        }
        
        int ConditionTest::ThreadTest::iter( void )
        {
            //
            // TODO this should clearly be protected.
            //
            return _iter;
        }
        void ConditionTest::ThreadTest::loop( void )
        {
            std::cout << "condition loop " << _id << std::endl;
            if( _id == 1 )
            {
                Time::sleep( 2 );
                _condition.broadcast( );
            }
            else
            {
                _condition.wait( );
                ++_iter;
            }
        }
       
        //
        // Test Suite.
        // ----------
        //

        ConditionTest::ConditionTest( void )
        {
            // No - Op.
        }
    
        ConditionTest::~ConditionTest( void )
        {
            // No - Op.
        }

        void ConditionTest::setUp( void )
        {
            // No - Op.
        }

        void ConditionTest::tearDown( void )
        {
            // No - Op.
        }

        void ConditionTest::test_signaling( void )
        {
            Mutex      mutex;
            Condition  condition(mutex);
        
            //
            // We need to use delete since if the Condition class does not work
            // then the thread 2 will never finish. The join( ) call on this
            // thread would be blocking. By deleting the thread we insure that
            // the test suite does not block.
            //
            
            ThreadTest *tt1 = new ThreadTest(condition, 1);
            ThreadTest *tt2 = new ThreadTest(condition, 2);
        
            tt1->start( );
            tt2->start( );
            
            //
            // TODO do not use ::sleep but a templated loop with the predicate. 
            //
            
            ::sleep( 3 );
                
            int tt1_iter = tt1->iter( );
            int tt2_iter = tt2->iter( );

            delete tt1 ;
            delete tt2 ;
        
            CPPUNIT_ASSERT_EQUAL( 1 , tt2_iter );
            CPPUNIT_ASSERT_EQUAL( 0 , tt1_iter );
        }
        
        void ConditionTest::test_scope( void )
        { 
            Mutex mtx;
            std::auto_ptr< boost::thread > thread( 0 );
            {
                Condition condition( mtx );
                thread.reset( new boost::thread(
                    boost::bind(
                        &Condition::wait, 
                        &condition ) ) );
                // You must broadcast before detroying the condition
                // Otherwise the condition destructor will fail while trying to
                // destroy the mutex.
                // 
                // In pthread this is due to a failing call to :
                // pthread_mutex_destroy( )
                // see
                // http://www.opengroup.org/onlinepubs/000095399/functions/pthread_mutex_init.html
                // for more info.
                condition.broadcast( );
                Time::sleep( 0.3 );
            }
        }
        
        CppUnit::TestSuite* ConditionTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( 
                new CppUnit::TestCaller<ConditionTest>( 
                    "ConditionTest::test_signaling", 
                    &ConditionTest::test_signaling ) );
            suite->addTest(
                new CppUnit::TestCaller<ConditionTest>(
                    "ConditionTest::test_scope",
                    &ConditionTest::test_scope ) );

            return suite;
        }
    }
}
