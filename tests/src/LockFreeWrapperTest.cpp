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
        // Data class
        // ---------- 
        //
        
        const std::size_t LockFreeWrapperTest::Data::SIZE( 100 );

        LockFreeWrapperTest::Data::Data( void )
        {
            _block = new TItem[ SIZE ];
            for( int i=0 ; i<SIZE ; i++)
            {
                _block[i] = 0;
            }

            TCheckSumGenerator crc;
            crc = std::for_each(  _block,
                                  _block + SIZE ,
                                  crc
                               );

            _checksum = crc( );
        }
        
        LockFreeWrapperTest::Data::Data(const Data &d)
        : _checksum( d._checksum )
        {
            _block = new TItem[ SIZE ];
            for(int i=0 ; i<SIZE ; i++ )
            {
                _block[i] = d._block[i];
            }
        }
        
        LockFreeWrapperTest::Data::~Data( void )
        {
            delete [] _block;
        }
        
        bool LockFreeWrapperTest::Data::consistent( void )
        {
             TCheckSumGenerator crc;
             crc = std::for_each(  _block,
                                   _block + SIZE,
                                   crc
                                ) ;
            if( _checksum != crc( ) )
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        
        
        void LockFreeWrapperTest::Data::reset( void )
        {
            maxmm::random::Uniform< TItem >  rd_generator( 0 , 255 );
            
            for(int i=0 ; i<SIZE ; i++ )
            {
                _block[ i ] = rd_generator( );
            }
            TCheckSumGenerator crc;
            crc = std::for_each(  _block,
                                  _block + SIZE,
                                  crc
                             ) ;
            _checksum = crc();
        }
        
        //
        // TestThread
        // ----------
        //
        
        LockFreeWrapperTest::TestThread::TestThread( int id , Data::TLockFreeValue & global_data)
        : _id    ( id ), 
          _failed( false ),
          _global( global_data ) ,
          _rd_generator( 0 , 10 ) ,
          Thread< NoWaitController >( NoWaitController( ) )
        {
        
        }
        LockFreeWrapperTest::TestThread::~TestThread( void )
        {
            for (LockFreeWrapper< Data >::TRetiredListItr 
                  itr  = _data_retired_list.begin();
                  itr != _data_retired_list.end();
                  itr ++
              )
            {
                delete *itr;
            }
        }

        bool LockFreeWrapperTest::TestThread::failed( void )
        {
            return _failed;
        }
        
        void LockFreeWrapperTest::TestThread::loop( void )
        {
            bool write_global = ( _rd_generator( ) > 2 ) ? true : false ;
           
            
            maxmm::ScopeTimer timer;
            {
                maxmm::ScopeTimer::Ressource res = timer.ressource( );
                //Reading block 
                {
                    Data::TLockFreePtr ptr ( _global );
                    
                    if( ptr->consistent( ) == false )
                    {
                        _failed = true;
                    }
                }   
        
                //writing block
                if ( write_global )
                {
                    Data *new_data;
                    {
                        Data::TLockFreePtr ptr ( _global );
                        new_data = new Data( *ptr );
                    }   
                    new_data->reset( );
                    _global.update( new_data , _data_retired_list );
                }
            }

            if( write_global )
            {
                _write_times.push_back( timer.elapsed( ).to_double( ) );
            }
            else
            {
                _read_times.push_back( timer.elapsed( ).to_double( ) );
            }
        }
        
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

        void LockFreeWrapperTest::ThreadDeleter::operator( ) (TestThread *t )
        {
            if( t->failed( ) )
            {
                _failed = true;
            }

            std::for_each(
                    t->_read_times.begin( ) , 
                    t->_read_times.end( ) ,
                    boost::bind( & ThreadDeleter::add_to_read_set , this , _1 ) );
            std::for_each(
                    t->_write_times.begin( ) , 
                    t->_write_times.end( ) , 
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
        
        LockFreeWrapperTest::LockFreeWrapperTest( void )
        : _shared_data( 1 )
        {
        
        }
        LockFreeWrapperTest::~LockFreeWrapperTest( void )
        {
        
        }
        
        void LockFreeWrapperTest::setUp( void )
        {
            //Reading block 
            {
                Data::TLockFreePtr ptr ( _shared_data );
                ptr->reset();
            }   
            for(int i=0 ; i<30 ; i++)
            {
                _threads.push_back( new TestThread( i , _shared_data) );
            }
        }
        
        void LockFreeWrapperTest::tearDown( void )
        {
            std::for_each(
                    _threads.begin(), 
                    _threads.end(), 
                    boost::bind(
                        &LockFreeWrapperTest::TestThread::stop ,
                        _1 ) );
            std::for_each(
                    _threads.begin( ), 
                    _threads.end( ), 
                    boost::bind( 
                        &LockFreeWrapperTest::TestThread::join ,
                        _1 ) );
            
            std::cout << std::endl;
            ThreadDeleter td;
            td = std::for_each(_threads.begin(), _threads.end(), td);
            _threads.clear();
            CPPUNIT_ASSERT_MESSAGE("checksum failed" , td.failed()==false);
        
            std::cout << "read  avg : " << td.read_avg( )  << std::endl;
            std::cout << "write avg : " << td.write_avg( ) << std::endl;
        }
        
        void LockFreeWrapperTest::test_lock_free( void )
        {
            std::for_each(
                    _threads.begin(), 
                    _threads.end(), 
                    boost::bind(
                        &LockFreeWrapperTest::TestThread::start , 
                        _1 ) ) ;

            ::sleep( 30 );
        }
        
        CppUnit::TestSuite* LockFreeWrapperTest::getSuite( void )
        {
            
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( 
                new CppUnit::TestCaller<LockFreeWrapperTest>( 
                    "LockFreeWrapperTest::test_lock_free", 
                    &LockFreeWrapperTest::test_lock_free ) );
            return suite;
        }
    }
}
