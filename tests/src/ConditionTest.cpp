#include "ConditionTest.h"
#include <maxmm/Time.h>

#include <cppunit/TestAssert.h>

namespace maxmm
{
    namespace test
    {

        ConditionTest::ThreadTest::ThreadTest( Condition & condition, int id ) 
        :_condition( condition ), _id( id ) , _iter( 0 )
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
        void ConditionTest::ThreadTest::run( void )
        {
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
            ScopeLock  sl(mutex);
            Condition  condition(sl);
        
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
        
        CppUnit::TestSuite* ConditionTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<ConditionTest>( "test_signaling", 
                                                              &ConditionTest::test_signaling 
                                                           ) 
                        );
            return suite;
        }
    }
}
