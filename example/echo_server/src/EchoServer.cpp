/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <EchoServer.h>
#include <Connection.h>
#include <boost/bind.hpp>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
            EchoServer::EchoServer(
                boost::asio::io_service &io_service , 
                uint32_t port )
            :   _io_service( io_service ) , 
                _port( port ), 
                _acceptor( _io_service , boost::asio::ip::tcp::endpoint(
                    boost::asio::ip::tcp::v4( ) , 
                    _port ) )
            { 
                this->run( );
            }
            
            EchoServer::~EchoServer( void )
            {
                for( TConnectionsItr
                        itr  = _connections.begin( );
                        itr != _connections.end( );
                        ++itr )
                {
                    delete itr->first;
                }
            } 
            void EchoServer::run( void )
            {
                this->clean_connections( );
                boost::asio::ip::tcp::socket *new_connection_socket
                    = new boost::asio::ip::tcp::socket( _io_service );
                
                _acceptor.async_accept(
                    *new_connection_socket,
                    boost::bind(
                        &EchoServer::on_connect , 
                        this , 
                        boost::asio::placeholders::error , 
                        new_connection_socket ) );
            }

            void EchoServer::add_connection( Connection * connection )
            {
                maxmm::ScopeLock lock( _connections_mtx );
                _connections.push_back( std::make_pair( connection , true ) );
            }

            void EchoServer::remove_connection( const Connection * connection )
            {
                maxmm::ScopeLock lock( _connections_mtx );
                for( TConnectionsItr 
                        itr  = _connections.begin( ) ; 
                        itr != _connections.end( ) ;
                        ++itr )
                {
                    if( itr->first == connection )
                    { 
                        itr->second = false;
                    }
                }
            }

            void EchoServer::clean_connections( void )
            {
                 maxmm::ScopeLock lock( _connections_mtx );
                 for( TConnectionsItr 
                        itr  = _connections.begin( ) ; 
                        itr != _connections.end( ) ;
                        ++itr )
                {
                    if( itr->second == false )
                    { 
                        delete itr->first;
                        itr->first = 0;
                        itr = _connections.erase( itr );
                    }
                }
            }
        }
    }
}
