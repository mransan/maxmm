/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <LockFreeQueueTest.h>
#include <TestCommon.h>

#include <maxmm/LockFreeQueue.h>
#include <maxmm/ScopeTimer.h>
#include <maxmm/Mutex.h>
#include <maxmm/Thread.h>
#include <maxmm/ThreadController.h>
#include <maxmm/Random.h>
#include <maxmm/Time.h>

#include <queue>
#include <vector>
#include <list>
#include <iomanip>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

namespace 
{
    
    
    static const uint32_t NB_TOTAL_OPERATIONS = 1000000;
    static const uint8_t  NB_THREADS = 2;

    class ThreadBase : public maxmm::Thread< maxmm::NoWaitController >
    {
        public:
            ThreadBase( void )
            :   maxmm::Thread< maxmm::NoWaitController >( maxmm::NoWaitController( ) ),
                _nb_generator( 10  , 13 ),
                _value( 0 )
            { }

            virtual ~ThreadBase( void )
            { }
            
            void do_work( void )
            {
                static_cast< void >( maxmm::test::fibonacci( _nb_generator( ) ) );
            }
        protected:
            maxmm::random::Uniform< uint8_t > _nb_generator;
            int _value;        
    };
    
    
    class LockFreeThread : public ThreadBase
    {
        public:
            LockFreeThread( maxmm::LockFreeQueue< int > &shared_queue )
            :   ThreadBase( ),
                _shared_queue( shared_queue ),
                _iter( NB_TOTAL_OPERATIONS / NB_THREADS ) 
            { }
            
            virtual ~LockFreeThread( void ) 
            { }
            void init( void ) 
            { }
            void clean( void ) 
            { }
            void loop( void ) 
            {
                _shared_queue.push_mt( --_iter );
                this->do_work( );
                _shared_queue.pop_mt( _value );
                this->do_work( );
                if( _iter == 0 )
                {
                    this->stop( );
                }
            }
        private:
           maxmm::LockFreeQueue< int > &_shared_queue; 
           int _iter;
    };
    
    class MutexThread : public ThreadBase
    {
        public:
            MutexThread( std::list< int > &shared_queue , maxmm::Mutex &shared_mtx )
            :   ThreadBase( ),
                _shared_queue( shared_queue ),
                _shared_mtx( shared_mtx ),
                _iter( NB_TOTAL_OPERATIONS / NB_THREADS )
            {
             
            }
            
            virtual ~MutexThread( void ) 
            { }
            void init( void ) 
            { }
            void clean( void ) 
            { }
            void loop( void ) 
            {
                {
                    maxmm::ScopeLock lock( _shared_mtx );
                    _shared_queue.push_back( --_iter );
                }
                this->do_work( );
                {
                    maxmm::ScopeLock lock( _shared_mtx );
                    _value = _shared_queue.front( );
                    _shared_queue.pop_front( );
                }
                this->do_work( );
                if( _iter == 0 )
                {
                    this->stop( );
                }
            }
        private:
           std::list< int > &_shared_queue;
           maxmm::Mutex &_shared_mtx;
           int _iter;
    };
    
    class LockNoLockThread : public ThreadBase
    {
        public:
            LockNoLockThread( maxmm::LockFreeQueue< int > &shared_queue , maxmm::Mutex &shared_mtx )
            :   ThreadBase( ),
                _shared_queue( shared_queue ),
                _shared_mtx( shared_mtx ),
                _iter( NB_TOTAL_OPERATIONS / NB_THREADS )
            { }
            
            virtual ~LockNoLockThread( void ) 
            { }
            void init( void ) 
            { }
            void clean( void ) 
            { }
            void loop( void ) 
            {
                {
                    maxmm::ScopeLock lock( _shared_mtx );
                    _shared_queue.push_st( --_iter );
                }
                this->do_work( );
                {
                    maxmm::ScopeLock lock( _shared_mtx );
                    _shared_queue.pop_st( _value );
                }
                this->do_work( );

                if( _iter == 0 )
                {
                    this->stop( );
                }
            }
        private:
           maxmm::LockFreeQueue< int > &_shared_queue;
           maxmm::Mutex &_shared_mtx;
           int _iter;
    };
}
namespace maxmm
{
    namespace test
    {
        void LockFreeQueueTest::test_push( void )
        {
            {
                LockFreeQueue<int> queue;
            
                CPPUNIT_ASSERT( queue._tail == queue._head ); 
                CPPUNIT_ASSERT( queue._head->_next == 0 ); 
                CPPUNIT_ASSERT( queue.empty( ) );
                
                queue.push_mt( 2 );
                
                CPPUNIT_ASSERT( queue._tail != queue._head ); 
                CPPUNIT_ASSERT( queue._head->_next != 0 ); 
                CPPUNIT_ASSERT( queue._head->_next == queue._tail );
            }
            {
                LockFreeQueue<int> queue;
            
                CPPUNIT_ASSERT( queue._tail == queue._head ); 
                CPPUNIT_ASSERT( queue._head->_next == 0 ); 
                CPPUNIT_ASSERT( queue.empty( ) );
                
                queue.push_st( 2 );
                
                CPPUNIT_ASSERT( queue._tail != queue._head ); 
                CPPUNIT_ASSERT( queue._head->_next != 0 ); 
                CPPUNIT_ASSERT( queue._head->_next == queue._tail );
            }
        }
        
        void LockFreeQueueTest::test_pop( void )
        {
            {
                LockFreeQueue< int > queue;

                queue.push_mt( 2 );
                int value = 0 ;
                
                CPPUNIT_ASSERT( queue.pop_mt( value ) );
                CPPUNIT_ASSERT_EQUAL( 2 , value );
                CPPUNIT_ASSERT( queue.empty( ) ); 
            }
            {
                LockFreeQueue< int > queue;

                queue.push_st( 2 );
                int value = 0 ;
                
                CPPUNIT_ASSERT( queue.pop_st( value ) );
                CPPUNIT_ASSERT_EQUAL( 2 , value );
                CPPUNIT_ASSERT( queue.empty( ) ); 
            }
        }

        void LockFreeQueueTest::test_perf( void )
        {
            static const int NB_INSERT = 15000000;
            {
                std::list< int > list;
                for( int i=0 ; i < NB_INSERT ; ++i )
                {
                    list.push_front( i );
                }
            }
            
            ScopeTimer timer;
            {
                ScopeTimer::Ressource res = timer.ressource( );
                LockFreeQueue<int> queue;
                for( int i = 0 ; i < NB_INSERT ; ++i )
                {
                    queue.push_mt( i );
                }
            }
            std::cout << "Lock free Queue MT: " << timer.elapsed( ) << std::endl; 
            
            {
                ScopeTimer::Ressource res = timer.ressource( );
                LockFreeQueue<int> queue;
                for( int i = 0 ; i < NB_INSERT ; ++i )
                {
                    queue.push_st( i );
                }
            }
            std::cout << "Lock free Queue ST: " << timer.elapsed( ) << std::endl; 
           
            {
                ScopeTimer::Ressource res = timer.ressource( );
                std::queue< int > queue;
                Mutex mutex;
                for( int i = 0 ; i < NB_INSERT ; ++i )
                {
                    ScopeLock lock( mutex );
                    queue.push( i );
                }
            }
            std::cout << "Std Queue with Mutex: " << timer.elapsed( ) << std::endl; 
        }
        
        void LockFreeQueueTest::test_mt_perf( void )
        {
            const uint8_t WIDTH = 40;

            std::cout << std::endl;
            ScopeTimer timer;
            {
                std::list< int > queue;
                maxmm::Mutex mtx;
                std::vector< IThread * > threads;
                std::generate_n(
                    std::back_inserter( threads ) , 
                    NB_THREADS , 
                    boost::lambda::bind(
                        boost::lambda::new_ptr< MutexThread >( ) , 
                        boost::ref( queue ) , 
                        boost::ref( mtx ) ) );

                {
                    ScopeTimer::Ressource res = timer.ressource( );
                    std::for_each( 
                        threads.begin( ) ,
                        threads.end( ) , 
                        boost::bind(
                            &IThread::start , 
                            _1 ) );
                    std::for_each( 
                        threads.begin( ) ,
                        threads.end( ) , 
                        boost::bind(
                            &IThread::join , 
                            _1 ) );
                }
                std::for_each( 
                    threads.begin( ) , 
                    threads.end( ) , 
                    boost::lambda::bind(
                        boost::lambda::delete_ptr( ) ,
                        boost::lambda::_1 ) );
                std::cout   << std::setw( WIDTH ) 
                            << "Mutex Thread duration:" 
                            << timer.elapsed( ) 
                            << std::endl;
            }
            {
                maxmm::LockFreeQueue< int > queue;
                maxmm::Mutex mtx;
                std::vector< IThread * > threads;
                std::generate_n(
                    std::back_inserter( threads ) , 
                    NB_THREADS, 
                    boost::lambda::bind(
                        boost::lambda::new_ptr< LockNoLockThread >( ) , 
                        boost::ref( queue ) , 
                        boost::ref( mtx ) ) );

                {
                    ScopeTimer::Ressource res = timer.ressource( );
                    std::for_each( 
                        threads.begin( ) ,
                        threads.end( ) , 
                        boost::bind(
                            &IThread::start , 
                            _1 ) );
                    std::for_each( 
                        threads.begin( ) ,
                        threads.end( ) , 
                        boost::bind(
                            &IThread::join , 
                            _1 ) );
                }
                std::for_each( 
                    threads.begin( ) , 
                    threads.end( ) , 
                    boost::lambda::bind(
                        boost::lambda::delete_ptr( ) ,
                        boost::lambda::_1 ) );
                std::cout   << std::setw( WIDTH ) 
                            << "Lock Free No Lock Thread duration:" 
                            << timer.elapsed( ) 
                            << std::endl;
            }
            {
                LockFreeQueue< int > queue;
                std::vector< IThread * > threads;
                std::generate_n(
                    std::back_inserter( threads ) , 
                    NB_THREADS , 
                    boost::lambda::bind(
                        boost::lambda::new_ptr< LockFreeThread >( ) , 
                        boost::ref(queue) ) );

                {
                    ScopeTimer::Ressource res = timer.ressource( );
                    std::for_each( 
                        threads.begin( ) ,
                        threads.end( ) , 
                        boost::bind(
                            &IThread::start , 
                            _1 ) );
                    std::for_each( 
                        threads.begin( ) ,
                        threads.end( ) , 
                        boost::bind(
                            &IThread::join , 
                            _1 ) );
                }

                std::for_each( 
                    threads.begin( ) , 
                    threads.end( ) , 
                    boost::lambda::bind(
                        boost::lambda::delete_ptr( ) ,
                        boost::lambda::_1 ) );
                std::cout   << std::setw( WIDTH ) 
                            << "LockFreeThread duration:" 
                            << timer.elapsed( ) 
                            << std::endl;
            }
        }
        CppUnit::TestSuite* LockFreeQueueTest::getSuite( void )
        {
            CppUnit::TestSuite *suite = new CppUnit::TestSuite();
            
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeQueueTest>( 
                        "LockFreeQueueTest::test_push", 
                        &LockFreeQueueTest::test_push ) );
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeQueueTest>( 
                        "LockFreeQueueTest::test_pop", 
                        &LockFreeQueueTest::test_pop ) );
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeQueueTest>( 
                        "LockFreeQueueTest::test_perf", 
                        &LockFreeQueueTest::test_perf ) );
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeQueueTest>( 
                        "LockFreeQueueTest::test_mt_perf", 
                        &LockFreeQueueTest::test_mt_perf ) );

            return suite;
        }
    
    }
}
