/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <SingleThreadEchoServer.h>
#include <Connection.h>

#include <boost/bind.hpp>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
            SingleThreadEchoServer::SingleThreadEchoServer(
                boost::asio::io_service &io_service , 
                uint32_t port )
            :   EchoServer( io_service , port )  
            { }
            
            void SingleThreadEchoServer::on_connect( 
                const boost::system::error_code &error ,
                boost::asio::ip::tcp::socket *new_connection_socket )
            {
                
                Connection *connection = new Connection( new_connection_socket , *this );
                this->add_connection( connection );
                connection->start( );
                this->run( );
                std::cout << "connection accepted" << std::endl;
            }

        }
    }
}
