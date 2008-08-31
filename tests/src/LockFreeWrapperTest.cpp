#include <LockFreeWrapperTest.h>

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
        
        LockFreeWrapperTest::Data::TLockFreeValue global_data( 1 ) ;
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
        
        LockFreeWrapperTest::TestThread::TestThread( int id )
        : _id    ( id ), 
          _failed( false ),
          _global( global_data ) ,
          _rd_generator( 0 , 10 ) ,
          Thread( )
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

        void LockFreeWrapperTest::TestThread::update_global( Data * ptr )
        {
            _global.update( ptr ,  _data_retired_list ); 
        }
        
        bool LockFreeWrapperTest::TestThread::failed( void )
        {
            return _failed;
        }
        
        void LockFreeWrapperTest::TestThread::run( void )
        {
            
            while( this->should_stop ( ) == false )
            {
                bool write_global = ( _rd_generator( ) > 2 ) ? true : false ;
           
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
                    std::cout << "reseting shared data " << new_data->_checksum << std::endl;
                    this->update_global( new_data );
                }
            }
        }
        
        
        //
        // Test Suite 
        // ----------
        //
        
        LockFreeWrapperTest::LockFreeWrapperTest( void )
        {
        
        }
        LockFreeWrapperTest::~LockFreeWrapperTest( void )
        {
        
        }
        
        void LockFreeWrapperTest::setUp( void )
        {
            //Reading block 
            {
                Data::TLockFreePtr ptr ( global_data );
                ptr->reset();
            }   
            for(int i=0 ; i<30 ; i++)
            {
                m_threads.push_back( new TestThread( i ) );
            }
            
            std::for_each(
                    m_threads.begin(), 
                    m_threads.end(), 
                    boost::bind(
                        &LockFreeWrapperTest::TestThread::start , 
                        _1 ) ) ;

        }
        void LockFreeWrapperTest::tearDown( void )
        {
            std::for_each(
                    m_threads.begin(), 
                    m_threads.end(), 
                    boost::bind(
                        &LockFreeWrapperTest::TestThread::should_stop ,
                        _1 , 
                        true ) );
            std::for_each(
                    m_threads.begin( ), 
                    m_threads.end( ), 
                    boost::bind( 
                        &LockFreeWrapperTest::TestThread::join ,
                        _1 ) );
            ThreadDeleter td;
            td = std::for_each(m_threads.begin(), m_threads.end(), td);
            m_threads.clear();
            CPPUNIT_ASSERT_MESSAGE("checksum failed" , td.failed()==false);
        }
        
        void LockFreeWrapperTest::testWrapper( void )
        {
            ::sleep( 30 );
        }
        
        CppUnit::TestSuite* LockFreeWrapperTest::getSuite( void )
        {
            
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<LockFreeWrapperTest>( "testWrapper", 
                                                              &LockFreeWrapperTest::testWrapper 
                                                           ) 
                        );
            return suite;
        }
    }
}
