/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <ClientConnection.h>
#include <EchoServer.h>

#include <boost/bind.hpp>

#include <iostream>

namespace maxmm
{
namespace example
{
namespace echo_server
{
    const uint32_t ClientConnection::MAX_LENGTH = 1000;
    
    ClientConnection::ClientConnection(
        boost::asio::ip::tcp::socket *socket , 
        EchoServer &echo_server )
    :   _socket_ptr( socket ),
        _echo_server( echo_server ),
        _data( new char[ MAX_LENGTH ] )
    { }

    ClientConnection::~ClientConnection( void )
    { }

    void ClientConnection::start( void ) 
    {
        _socket_ptr->async_read_some(
            boost::asio::buffer(
                _data, 
                MAX_LENGTH ) ,
            boost::bind(
                &ClientConnection::on_read,
                this,
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred ) );
    }

    void ClientConnection::on_read(
        const boost::system::error_code &error,
        size_t bytes_transfered )
    {
        if( !error )
        {
            std::cout << "received : " << _data << std::endl;
            
            std::reverse(
                _data , 
                _data + bytes_transfered );
            
            _socket_ptr->async_write_some(
                boost::asio::buffer( _data , bytes_transfered ), 
                boost::bind(
                    &ClientConnection::on_write,
                    this ,
                    boost::asio::placeholders::error ) );
            std::cout << "start writing" << std::endl;
        }
        else
        {
            std::cout << "read disconnection" << std::endl;
            _echo_server.remove_connection( this );
        }
    }

    void ClientConnection::on_write(
        const boost::system::error_code &error )
    {
        if( !error )
        {
            std::cout << "data writen" << std::endl ;
            this->start( );
        }
        else
        {
            std::cout << "write disconnection" << std::endl;
            _echo_server.remove_connection( this );
        }
    }
}
}
}
