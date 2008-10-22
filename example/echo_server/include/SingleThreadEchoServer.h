/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_example_echo_server_SingleThreadEchoServer_h
#define maxmm_example_echo_server_SingleThreadEchoServer_h

#include <EchoServer.h>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
        
            class SingleThreadEchoServer : public EchoServer
            {
                public:
                    //! \brief constructor.
                    //!
                    SingleThreadEchoServer(
                        boost::asio::io_service &io_service , 
                        uint32_t port );
                    
                    virtual ~SingleThreadEchoServer( void )
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
