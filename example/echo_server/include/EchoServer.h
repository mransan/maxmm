/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_example_echo_server_EchoServer_h
#define maxmm_example_echo_server_EchoServer_h

#include <boost/asio.hpp>
#include <maxmm/Mutex.h>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
            class ClientConnection;
            
            //! \brief Base class for all echo servers.
            //!
            //! provides functionality for managinf client connections.
            //!
            class EchoServer
            {
                public:
                    
                    //! \brief constructor.
                    //!
                    EchoServer(
                        boost::asio::io_service &io_service,
                        uint32_t port );
                    
                    //! \brief destructor.
                    //!
                    virtual ~EchoServer( void );
                    

                    //! \brief start accepting new connections.
                    //!
                    void run( void );
                    
                    //! \brief remove a connections.
                    //!
                    void remove_connection( const ClientConnection * connection );
                protected:
                    
                    //! \brief add a connection to the list of managed
                    //! connections.
                    //!
                    void add_connection( ClientConnection * connection );
                    //! \brief de-allocate and remove unused connections.
                    //!
                    void clean_connections( void );

                    //! \brief call back method for new received connections.
                    //!
                    virtual void on_connect(
                        const boost::system::error_code &error , 
                        boost::asio::ip::tcp::socket *new_connection_socket ) = 0;

                    
                    //! \brief reference to the reactor.
                    //!
                    boost::asio::io_service &_io_service;
                    
                    //! \brief port to bound the acceptor.
                    //!
                    uint32_t _port;
                    
                    //! \brief acceptor to receive connection.
                    //!
                    boost::asio::ip::tcp::acceptor _acceptor;
                    
                    //! \brief container for client connections.
                    //!
                    typedef std::vector< 
                        std::pair< ClientConnection * , bool > > TConnections;
                    typedef TConnections::iterator TConnectionsItr;
                    
                    //! \brief list of managed connections.
                    //!
                    TConnections _connections;

                    //! \brief connections protection mutex.
                    //!
                    mutable Mutex _connections_mtx;
            };
        }
    }
}



#endif
