/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <ThreadPoolTest.h>
#include <maxmm/ThreadPool.h>
#include <TestCommon.h>


#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <apr_atomic.h>
#include <assert.h>


namespace
{
    class BoolWork : public maxmm::IWork
    {
        public:
            BoolWork( uint32_t &flag)
            :   _flag( flag )
            { 
                _flag = 0;   
            }

            virtual ~BoolWork( void ) { }

            void execute( void )
            {
                assert( apr_atomic_cas32(&_flag , 1 , 0 ) == 0 ) ;
            }
        private:
            uint32_t &_flag;
    };
    
    
    static bool verify_vector( const std::vector< uint32_t > &vector )
    {
        for( std::vector< uint32_t >::const_iterator
            it  = vector.begin( );
            it != vector.end( );
            ++it )
        {
            if( *it == 0 )
            {
                return false;
            }
        }

        return true;
    }
}


namespace maxmm
{
    namespace test
    {
        void ThreadPoolTest::test_single_thread( void )
        {
            static const uint32_t NB_WORKS = 10;
            {
                // vector of value that will set to 1 in the bool job.
                std::vector< uint32_t > bool_val( NB_WORKS );

                // create the bool job.
                std::vector< BoolWork* > works;
                std::transform(
                    bool_val.begin( ) , 
                    bool_val.end( ),
                    std::back_inserter( works ),
                    boost::lambda::bind(
                        boost::lambda::new_ptr< BoolWork >( )  , 
                        boost::lambda::_1) );
                
                // create the pool.
                maxmm::ThreadPool pool( 1 );
                
                // append all the jobs.
                std::for_each(
                    works.begin( ) , 
                    works.end( ) , 
                    boost::bind(
                        &ThreadPool::append_work , 
                        &pool , 
                        _1 ) );
                CPPUNIT_ASSERT_EQUAL( std::size_t( NB_WORKS ) , pool._works.size( ) );
                
                // start the pool
                pool.start( );
                
                CPPUNIT_ASSERT(
                    maxmm::test::exec_until( 
                        boost::bind( 
                            &verify_vector , 
                            boost::cref( bool_val ) ) , 
                        2 ) );

                pool.stop( );
            }
            {
                // vector of value that will set to 1 in the bool job.
                std::vector< uint32_t > bool_val( NB_WORKS );

                // create the bool job.
                std::vector< BoolWork* > works;
                std::transform(
                    bool_val.begin( ) , 
                    bool_val.end( ),
                    std::back_inserter( works ),
                    boost::lambda::bind(
                        boost::lambda::new_ptr< BoolWork >( )  , 
                        boost::lambda::_1) );
                
                // create the pool.
                maxmm::ThreadPool pool( 1 );
           
                pool.start( );
                
                // append all the jobs.
                std::for_each(
                    works.begin( ) , 
                    works.end( ) , 
                    boost::bind(
                        &ThreadPool::append_work , 
                        &pool , 
                        _1 ) );
                CPPUNIT_ASSERT(
                    maxmm::test::exec_until( 
                        boost::bind( 
                            &verify_vector , 
                            boost::cref( bool_val ) ) , 
                        2 ) );
                pool.stop( );
            }
        }
        
        void ThreadPoolTest::test_multi_thread( void )
        {
            static const uint32_t NB_WORKS = 1000;
            {
                // vector of value that will set to 1 in the bool job.
                std::vector< uint32_t > bool_val( NB_WORKS );

                // create the bool job.
                std::vector< BoolWork* > works;
                std::transform(
                    bool_val.begin( ) , 
                    bool_val.end( ),
                    std::back_inserter( works ),
                    boost::lambda::bind(
                        boost::lambda::new_ptr< BoolWork >( )  , 
                        boost::lambda::_1) );
                
                // create the pool.
                maxmm::ThreadPool pool( 5 );
                
                // append all the jobs.
                std::for_each(
                    works.begin( ) , 
                    works.end( ) , 
                    boost::bind(
                        &ThreadPool::append_work , 
                        &pool , 
                        _1 ) );
                CPPUNIT_ASSERT_EQUAL( std::size_t( NB_WORKS ) , pool._works.size( ) );
                
                // start the pool
                pool.start( );
                
                CPPUNIT_ASSERT(
                    maxmm::test::exec_until( 
                        boost::bind( 
                            &verify_vector , 
                            boost::cref( bool_val ) ) , 
                       2) );

                pool.stop( );

                std::vector< uint64_t > stats = pool.execution_stats( );
                std::for_each(
                    stats.begin( ),  
                    stats.end( ),
                    std::cout << boost::lambda::_1 << ' ' );
                std::cout << std::endl;
            }
//            {
//                // vector of value that will set to 1 in the bool job.
//                std::vector< uint32_t > bool_val( NB_WORKS );
//
//                // create the bool job.
//                std::vector< BoolWork* > works;
//                std::transform(
//                    bool_val.begin( ) , 
//                    bool_val.end( ),
//                    std::back_inserter( works ),
//                    boost::lambda::bind(
//                        boost::lambda::new_ptr< BoolWork >( )  , 
//                        boost::lambda::_1) );
//                
//                // create the pool.
//                maxmm::ThreadPool pool( 5 );
//           
//                pool.start( );
//                
//                // append all the jobs.
//                std::for_each(
//                    works.begin( ) , 
//                    works.end( ) , 
//                    boost::bind(
//                        &ThreadPool::append_work , 
//                        &pool , 
//                        _1 ) );
//                CPPUNIT_ASSERT(
//                    maxmm::test::exec_until( 
//                        boost::bind( 
//                            &verify_vector , 
//                            boost::cref( bool_val ) ) , 
//                        2 ) );
//                pool.stop( );
//                std::vector< uint64_t > stats = pool.execution_stats( );
//                std::for_each(
//                    stats.begin( ),  
//                    stats.end( ),
//                    std::cout << boost::lambda::_1 << ' ' );
//                std::cout << std::endl;
//               
//            }
        }

        CppUnit::TestSuite* ThreadPoolTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<ThreadPoolTest>( 
                        "ThreadPoolTest::test_single_thread",
                        &ThreadPoolTest::test_single_thread ) );
            suite->addTest( new CppUnit::TestCaller<ThreadPoolTest>( 
                        "ThreadPoolTest::test_multi_thread",
                        &ThreadPoolTest::test_multi_thread ) );
           
            return suite;
           
        
        }
    }
}
