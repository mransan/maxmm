/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <ThreadPoolTest.h>
#include <maxmm/ThreadPool.h>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>



namespace
{
    class BoolWork : public maxmm::IWork
    {
        public:
            BoolWork( uint8_t &flag)
            :   _flag( flag )
            { 
                _flag = 0;   
            }

            virtual ~BoolWork( void ) { }

            void execute( void )
            {
                _flag = 1 ;
            }
        private:
            uint8_t &_flag;
    };
}


namespace maxmm
{
    namespace test
    {
        void ThreadPoolTest::test_single_thread( void )
        {
            
            {
                // vector of value that will set to 1 in the bool job.
                std::vector< uint8_t > bool_val( 10 );

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
                CPPUNIT_ASSERT_EQUAL( std::size_t( 10 ) , pool._works.size( ) );
                
                // start the pool
                pool.start( );
                
                ::sleep( 1 );

                pool.stop( );
                
                for( int i=0 ; i<bool_val.size( ) ; ++i )
                {
                    CPPUNIT_ASSERT_EQUAL( uint8_t( 1 ) , bool_val[ i ] );    
                }
            }
            {
                // vector of value that will set to 1 in the bool job.
                std::vector< uint8_t > bool_val( 10 );

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
                ::sleep( 1 );

                pool.stop( );

                for( int i=0 ; i<bool_val.size( ) ; ++i )
                {
                    CPPUNIT_ASSERT_EQUAL( uint8_t( 1 ) , bool_val[ i ] );    
                }
            }
           
        }
        

        CppUnit::TestSuite* ThreadPoolTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<ThreadPoolTest>( 
                        "ThreadPoolTest::test_single_thread",
                        &ThreadPoolTest::test_single_thread ) );
            return suite;
           
        
        }
    }
}
