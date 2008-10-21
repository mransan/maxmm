/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <ThreadControllerTest.h>

#include <maxmm/ThreadController.h>
#include <maxmm/ScopeTimer.h>
#include <maxmm/Thread.h>

namespace 
{
    class CThread : public maxmm::Thread< maxmm::ConditionController >
    {
        public:
            CThread( maxmm::Condition & condition )
            :   maxmm::Thread< maxmm::ConditionController >( 
                    maxmm::ConditionController( condition ) ) ,
                _iter( 0 )
            { } 
            virtual ~CThread( void )
            { }
            
            void loop( void )
            {
                ++_iter ;
            }
            void init( void )
            { }
            void clean(  void )
            { }
            uint32_t iter( void )
            {
                return _iter;
            }
        private:
            uint32_t _iter ;
    };

}

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

        void ThreadControllerTest::composite_controller_test( void )
        {
            {
                CompositeController<
                    OnceController , 
                    NoWaitController > controller( 
                         ( OnceController( )  ), 
                         ( NoWaitController( ) ) );

                CPPUNIT_ASSERT_EQUAL( true ,  controller.execute( ) );
                CPPUNIT_ASSERT_EQUAL( false , controller.execute( ) );
            }
            
            {
                CompositeController<
                    OnceController , 
                    NoWaitController , 
                    std::logical_or< bool > > controller( 
                        static_cast< OnceController >( OnceController( ) ) , 
                        static_cast< NoWaitController >( NoWaitController( ) ) );
                
                CPPUNIT_ASSERT_EQUAL( true , controller.execute( ) );
                CPPUNIT_ASSERT_EQUAL( true , controller.execute( ) );
            }
        }
    
        void ThreadControllerTest::condition_controller_test( void )
        {
            Mutex mutex;
            {
                Condition condition( mutex );
                
                CThread thread( condition );

                CPPUNIT_ASSERT_EQUAL( uint32_t( 0 ) , thread.iter( ) );

                thread.start( );

                CPPUNIT_ASSERT_EQUAL( uint32_t( 0 ) , thread.iter( ) );

                condition.broadcast( );
                
                Time::sleep( 0.2 );

                CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , thread.iter( ) );
                CPPUNIT_ASSERT_EQUAL( false , thread.should_stop( ) );

                thread.stop( );
                
                CPPUNIT_ASSERT_EQUAL( uint32_t( 1 ) , thread.iter( ) );
                CPPUNIT_ASSERT_EQUAL( true , thread.should_stop( ) );
                
                condition.broadcast( );

                thread.join( );
            }
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
            
            suite->addTest( 
                new CppUnit::TestCaller<ThreadControllerTest>( 
                    "ThreadControllerTest::composite_controller_test", 
                    &ThreadControllerTest::composite_controller_test ) );
             
            suite->addTest( 
                new CppUnit::TestCaller<ThreadControllerTest>( 
                    "ThreadControllerTest::condition_controller_test", 
                    &ThreadControllerTest::condition_controller_test ) );
         
            return suite;
        }

    }
}
