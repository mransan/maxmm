/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include "LockFreeWrapperTest_details.h"

#include <maxmm/ScopeTimer.h>

namespace maxmm
{
    namespace test
    {
        
        //
        // Data class
        // ---------- 
        //
        
        const std::size_t Data::SIZE( 100 );

        Data::Data( void )
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
        
        Data::Data(const Data &d)
        : _checksum( d._checksum )
        {
            _block = new TItem[ SIZE ];
            for(int i=0 ; i<SIZE ; i++ )
            {
                _block[i] = d._block[i];
            }
        }
        
        Data::~Data( void )
        {
            delete [] _block;
        }
        
        bool Data::consistent( void ) const 
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
        
        bool Data::consistent_locked( void ) const 
        {
            maxmm::ScopeLock scope_lock( _lock );
            return this->consistent( );
        }

        void Data::reset( void )
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

        void Data::reset_locked( void )
        {
            maxmm::ScopeLock scope_lock( _lock );
            this->reset( );
        }

        //
        // Base class for LockFreeTest threads.
        // ---
        //
        
        LockFreeTestThread::LockFreeTestThread( int id )
        :   Thread< NoWaitController >( NoWaitController( ) ),
            _id( id ) ,
            _failed( false ) ,
            _rd_generator( 0 , 10 )
        {
            // No - Op.
        }

        LockFreeTestThread::~LockFreeTestThread( void )
        {
            // No - Op.    
        }

        bool LockFreeTestThread::failed( void )
        {
            return _failed;    
        }
    
        void LockFreeTestThread::init( void )
        {
            
        }

        void LockFreeTestThread::clean( void )
        {
            
        }
        
        const std::vector< double > &LockFreeTestThread::read_times( void ) const 
        {
            return _read_times;    
        }
       
        const std::vector< double > &LockFreeTestThread::write_times( void ) const 
        {
            return _write_times;    
        }

        //
        // LockFree thread... to test the lock free structure.
        // -----
        //
        
        LockFreeThread::LockFreeThread( int id , Data::TLockFreeValue &shared_data )
        :   LockFreeTestThread( id ) ,
            _shared_data( shared_data )
        {
            // No - Op.    
        }
        
        LockFreeThread::~LockFreeThread( void )
        {
            // clean up the retired list.
            for( LockFreeWrapper< Data >::TRetiredListItr 
                itr  = _data_retired_list.begin( );
                itr != _data_retired_list.end( ); 
                ++itr )
            {
                delete *itr ;
            }
        }
        
        void LockFreeThread::loop( void )
        {
            bool write_global = ( _rd_generator( ) > 2 ) ? true : false ;
           
            maxmm::ScopeTimer timer;
            {
                maxmm::ScopeTimer::Ressource res = timer.ressource( );
                //Reading block 
                {
                    Data::TLockFreePtr ptr ( _shared_data );
                    
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
                        Data::TLockFreePtr ptr ( _shared_data );
                        new_data = new Data( *ptr );
                    }   
                    new_data->reset( );
                    _shared_data.update( new_data , _data_retired_list );
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
        // Mutext Thread
        // ----
        //

        MutexThread::MutexThread( int id , Data& shared_data )
        :   LockFreeTestThread( id ) , 
            _shared_data( shared_data )
        {
        
        }

        MutexThread::~MutexThread( void )
        {
        
        }

        void MutexThread::loop( void )
        {
            bool write_global = ( _rd_generator( ) > 2 ) ? true : false ;
           
            maxmm::ScopeTimer timer;
            {
                maxmm::ScopeTimer::Ressource res = timer.ressource( );
                //Reading block 
                {
                    if( _shared_data.consistent_locked( ) == false )
                    {
                        _failed = false;
                    };
                }   
        
                //writing block
                if ( write_global )
                {
                    _shared_data.reset_locked( );
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

    } // namespace test
} // namespace maxmm
