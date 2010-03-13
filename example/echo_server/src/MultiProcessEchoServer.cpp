/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <MultiProcessEchoServer.h>
#include <ClientConnection.h>

#include <boost/bind.hpp>

#include <iostream>

namespace maxmm
{
namespace example
{
namespace echo_server
{
    MultiProcessEchoServer::MultiProcessEchoServer(
        boost::asio::io_service &io_service , 
        uint32_t port )
    :   EchoServer( io_service , port )  
    { }
    
    void MultiProcessEchoServer::on_connect( 
        const boost::system::error_code &error ,
        boost::asio::ip::tcp::socket *new_connection_socket )
    {
        pid_t pid = ::fork( );
        if( pid == 0 )
        {
            ClientConnection *connection 
                = new ClientConnection( new_connection_socket , *this );
            this->add_connection( connection );
            connection->start( );
            std::cout << "connection accepted" << std::endl;
        }
        else
        {
            this->run( );
        }
    }
}
}
}
