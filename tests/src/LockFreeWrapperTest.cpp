#include <LockFreeWrapperTest.h>
#include <algorithm>  // for std::for_each
#include <cstddef>    // for std::size_t



namespace maxmm
{
    namespace test
    {
        LockFreeWrapperTest::Data::TLockFreeValue global_data( 3 ) ;
            
        const std::size_t LockFreeWrapperTest::Data::SIZE( 100 );

        LockFreeWrapperTest::Data::Data( void )
        {
            _block.reserve( SIZE );
            TCheckSumGenerator crc;
            crc = std::for_each(  _block.begin( ),
                                  _block.end( ),
                                  crc
                               );
           _checksum = crc( );
        }
        
        LockFreeWrapperTest::Data::Data(const Data &d)
        : _block( d._block ) ,
          _checksum( d._checksum )
        {
            // No - Op.
        }
        
        LockFreeWrapperTest::Data::~Data( void )
        {
            // No - Op.
        }
        
        bool LockFreeWrapperTest::Data::consistent( void )
        {
             TCheckSumGenerator crc;
             crc = std::for_each(  _block.begin( ),
                                   _block.end( ),
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
            boost::mt19937 nb_generator(
                    static_cast<long unsigned int>( ::clock( ) ) );
            boost::uniform_int < TItem > distribution(0 , 255);
            boost::variate_generator
                        <boost::mt19937,
                         boost::uniform_int< TItem>
                        >   var_generator(
                                nb_generator , 
                                distribution ); 

            for( TItemVec::iterator
                    itr  = _block.begin( ) ; 
                    itr != _block.end( ) ;
                    ++itr )
            {
                *itr = var_generator( ) ; 
            }
            
            TCheckSumGenerator crc;
            crc = std::for_each(  _block.begin( ),
                                  _block.end( ),
                                  crc
                             ) ;
            _checksum = crc();
            crc.reset();
        }
        
        //
        // TestThread
        // ----------
        //
        
        LockFreeWrapperTest::TestThread::TestThread( int id )
        : _id    ( id ), 
          _failed( false ),
          _global( global_data ) ,
          TimedThread(0.1)
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
        
        void LockFreeWrapperTest::TestThread::loop( void )
        {
            boost::mt19937 nb_generator(
                    static_cast<long unsigned int>( ::clock( ) ) );
            boost::uniform_int < uint8_t > distribution(0 , 255);
            boost::variate_generator
                        <boost::mt19937,
                         boost::uniform_int< uint8_t >
                        >   var_generator(
                                nb_generator , 
                                distribution ); 

            _period = static_cast<double>( var_generator( )%40 + 10)/1000.0;
            LOG_DEBUG   << "(th : " 
                        << _id 
                        <<  ") reseting loop period to : " << _period << std::endl;
           
            //Reading block 
            {
                Data::TLockFreePtr ptr ( _global );
                if( ptr->consistent( ) == false )
                {
                    _failed = true;
                }
            }   
        
            //writing block
            if (_period > 0.0025)
            {
                Data *new_data;
                {
                    Data::TLockFreePtr ptr ( _global );
                    new_data = new Data( *ptr );
                }   
                new_data->reset( );
                this->update_global( new_data );
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
