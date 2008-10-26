/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_example_echo_server_MultiProcessEchoServer_h
#define maxmm_example_echo_server_MultiProcessEchoServer_h

#include <EchoServer.h>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
        
            //! \brief Echo server using a multi process architecture.
            //!
            //! Each connection is handled by a single process. When a new
            //! connection is accepted the main process forks. The child handles
            //! the connection while the parent keep on accepting new connection.
            //!
            class MultiProcessEchoServer : public EchoServer
            {
                public:
                    //! \brief constructor.
                    //! 
                    //! \param io_service reference to the reactor object.
                    //! \param port port to bind the server to.
                    //!
                    MultiProcessEchoServer(
                        boost::asio::io_service &io_service , 
                        uint32_t port );
                    
                    //! \brief destructor.
                    //!
                    virtual ~MultiProcessEchoServer( void )
                    { }

                protected:

                    //! \brief callback method for new client connection.
                    //!
                    //! \param error error code returned by the underlying
                    //! system call.
                    //! \param new_connection_socket socket of the new
                    //! connections.
                    //!
                    virtual void on_connect( 
                        const boost::system::error_code &error , 
                        boost::asio::ip::tcp::socket *new_connection_socket );
            };
        }
    }
}



#endif
