#include <LockFreeWrapperTest.h>

#include <maxmm/Time.h>
#include <maxmm/ScopeTimer.h>

#include <algorithm>  // for std::for_each
#include <cstddef>    // for std::size_t



namespace maxmm
{
    namespace test
    {
        
        //
        // Thread Deleter.
        // ---------------
        //
        
        LockFreeWrapperTest::ThreadDeleter::ThreadDeleter( void )
        : _failed( false )
        { }

        void LockFreeWrapperTest::ThreadDeleter::add_to_write_set( const double & d ) 
        {
            _write_timeset( d );
        }

        void LockFreeWrapperTest::ThreadDeleter::add_to_read_set( const double & d )
        {
            _read_timeset( d );
        }

        void LockFreeWrapperTest::ThreadDeleter::operator( )( LockFreeTestThread *t )
        {
            if( t->failed( ) )
            {
                _failed = true;
            }

            std::for_each(
                    t->read_times( ).begin( ) , 
                    t->read_times( ).end( ) ,
                    boost::bind( & ThreadDeleter::add_to_read_set , this , _1 ) );
            std::for_each(
                    t->write_times( ).begin( ) , 
                    t->write_times( ).end( ) , 
                    boost::bind( & ThreadDeleter::add_to_write_set , this , _1 ) );
            delete t;
        }

        double LockFreeWrapperTest::ThreadDeleter::read_avg( void )
        {
            return boost::accumulators::mean( _read_timeset );
        }

        double LockFreeWrapperTest::ThreadDeleter::write_avg( void )
        {
            return boost::accumulators::mean( _write_timeset );
        }
        
        bool LockFreeWrapperTest::ThreadDeleter::failed( void )
        {
            return _failed ;
        }

        //
        // Test Suite 
        // ----------
        //
        
        const uint32_t LockFreeWrapperTest::TEST_DURATION = 30;
        LockFreeWrapperTest::LockFreeWrapperTest( void )
        : _shared_data( 10 )
        {
        
        }
        LockFreeWrapperTest::~LockFreeWrapperTest( void )
        {
        
        }
        
        void LockFreeWrapperTest::setUp( void )
        {
        }
        
        void LockFreeWrapperTest::tearDown( void )
        {
        }
    
        
        bool LockFreeWrapperTest::test_lock( void )
        {
            // start the thread.
            std::for_each(
                    _threads.begin(), 
                    _threads.end(), 
                    boost::bind(
                        &LockFreeTestThread::start , 
                        _1 ) ) ;

            ::sleep( TEST_DURATION );
            
            // stop the threads.
            std::for_each(
                    _threads.begin(), 
                    _threads.end(), 
                    boost::bind(
                        &LockFreeTestThread::stop ,
                        _1 ) );
            
            // join the thread.
            std::for_each(
                    _threads.begin( ), 
                    _threads.end( ), 
                    boost::bind( 
                        &LockFreeTestThread::join ,
                        _1 ) );
            
            std::cout << std::endl;
            
            // delete memory and collect statisticall info from threads.
            ThreadDeleter td;
            td = std::for_each(_threads.begin(), _threads.end(), td);
            _threads.clear();

        
            std::cout << "read  avg : " << td.read_avg( )  << std::endl;
            std::cout << "write avg : " << td.write_avg( ) << std::endl;
            
            return !td.failed( );
        }

        void LockFreeWrapperTest::test_lock_free( void )
        {
            // reading block 
            {
                Data::TLockFreeReaderPtr ptr ( _shared_data );
                ptr->reset();
            }   
            
            CPPUNIT_ASSERT( _threads.empty( ) );
            // create the threads... initialise them with a shared data.
            for(int i=0 ; i<30 ; i++)
            {
                _threads.push_back( new LockFreeThread( i , _shared_data) );
            }
            
            CPPUNIT_ASSERT( this->test_lock( ) );
        }
        
        
        void LockFreeWrapperTest::test_lock_mutex( void )
        {
            
            _shared_data_protected.reset( );
            
            CPPUNIT_ASSERT( _threads.empty( ) );
           
            // create the threads... initialise them with a shared data.
            for(int i=0 ; i<30 ; i++)
            {
                _threads.push_back( new MutexThread( i , _shared_data_protected) );
            }
            
            CPPUNIT_ASSERT( this->test_lock( ) );
        }
       
        CppUnit::TestSuite* LockFreeWrapperTest::getSuite( void )
        {
            
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeWrapperTest>( 
                    "LockFreeWrapperTest::test_lock_free", 
                    &LockFreeWrapperTest::test_lock_free ) );
             
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeWrapperTest>( 
                    "LockFreeWrapperTest::test_lock_mutex", 
                    &LockFreeWrapperTest::test_lock_mutex ) );
           
            return suite;
        }
    }
}
