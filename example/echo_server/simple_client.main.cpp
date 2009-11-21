/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <iostream>

#include <boost/asio.hpp>
#include <maxmm/Thread.h>
#include <maxmm/ThreadController.h>
namespace
{
    
    class CThread : public maxmm::Thread< maxmm::NoWaitController >
    {
        public:
            
            CThread( boost::asio::io_service& io_service )
            :   maxmm::Thread< maxmm::NoWaitController >( maxmm::NoWaitController( ) ),
                _io_service( io_service ),
                _socket( _io_service ) , 
                _endpoint( boost::asio::ip::tcp::v4( ) , SERVER_PORT )
            { 
                _socket.connect( _endpoint );
            }

            virtual void init( void ) { }
            virtual void clean( void ) { }

            virtual void loop( void )
            { 
                _socket.write_some( boost::asio::buffer( PHRASE.c_str( ) , PHRASE.size( ) ) );
                
                size_t bytes = _socket.read_some( boost::asio::buffer( _buffer , BUFFER_SIZE ) );
                
                std::cout << "received : " ;
                std::cout.write( _buffer , bytes );
                std::cout << std::endl;
            }
        private:
            boost::asio::io_service &_io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::ip::tcp::endpoint _endpoint;
            
            static const uint32_t SERVER_PORT = 2333;
            static const std::string PHRASE;
            static const uint32_t BUFFER_SIZE = 1000;

            char _buffer[ BUFFER_SIZE ];
    };

    const uint32_t CThread::SERVER_PORT;
    const std::string CThread::PHRASE = "max is a moron";
    const uint32_t CThread::BUFFER_SIZE;
}

int main( void )
{
    boost::asio::io_service io_service;
    
    CThread thread( io_service );

    thread.start( );

    ::sleep( 1 );

    thread.stop( );
    
    thread.join( );
   return 0;
}
