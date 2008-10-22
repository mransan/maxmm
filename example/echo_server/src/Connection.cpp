/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <Connection.h>
#include <EchoServer.h>

#include <boost/bind.hpp>
namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
            const uint32_t Connection::MAX_LENGTH = 1000;
            
            Connection::Connection(
                boost::asio::ip::tcp::socket *socket , 
                EchoServer &echo_server )
            :   _socket_ptr( socket ),
                _echo_server( echo_server ),
                _data( new char[ MAX_LENGTH ] )
            { }

            Connection::~Connection( void )
            { }

            void Connection::start( void ) 
            {
                _socket_ptr->async_read_some(
                    boost::asio::buffer(
                        _data, 
                        MAX_LENGTH ) ,
                    boost::bind(
                        &Connection::on_read,
                        this,
                        boost::asio::placeholders::error, 
                        boost::asio::placeholders::bytes_transferred ) );
            }

            void Connection::on_read(
                const boost::system::error_code &error,
                size_t bytes_transfered )
            {
                if( !error )
                {
                    _data[ bytes_transfered ] = '\0';
                    std::string str( _data );
                    
                    _socket_ptr->async_write_some(
                        boost::asio::buffer( str.c_str( ) , str.size( ) ), 
                        boost::bind(
                            &Connection::on_write,
                            this ,
                            boost::asio::placeholders::error ) );
                }
                else
                {
                    _echo_server.remove_connection( this );
                }
            }

            void Connection::on_write(
                const boost::system::error_code &error )
            {
                if( !error )
                {
                    this->start( );
                }
                else
                {
                    _echo_server.remove_connection( this );
                }
            }
        }
    }
}
