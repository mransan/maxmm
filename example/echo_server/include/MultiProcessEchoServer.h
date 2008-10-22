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
        
            class MultiProcessEchoServer : public EchoServer
            {
                public:
                    //! \brief constructor.
                    //!
                    MultiProcessEchoServer(
                        boost::asio::io_service &io_service , 
                        uint32_t port );
                    
                    virtual ~MultiProcessEchoServer( void )
                    { }

                protected:

                    virtual void on_connect( 
                        const boost::system::error_code &error , 
                        boost::asio::ip::tcp::socket *new_connection_socket );

            };
        }
    }
}



#endif
