/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_example_echo_server_ClientConnection_h
#define maxmm_example_echo_server_ClientConnection_h

#include <boost/asio.hpp>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
            class EchoServer;
            
            
            //! \brief Abstract a client connection. 
            //!
            //! This class holds a socket object and handle reading client
            //! request and writing back.
            //!
            class ClientConnection
            {
                public:
                    //! \brief Constructor.
                    //!
                    //! \param socket pointer to a socket object. The
                    //! ClientConnection becomes responsible for its destruction.
                    //! \param echo_server reference to the echo server to allow
                    //! call back methods. ( manager pattern ).
                    //!
                    ClientConnection( 
                        boost::asio::ip::tcp::socket *socket , 
                        EchoServer &echo_server );
                    
                    //! \brief destructor.
                    //!
                    ~ClientConnection( void );

                    //! \brief start the reading/writing loop.
                    //!
                    void start( void );
                private:

                    //! \brief call back method for the
                    //! boost::asio::ip::tcp::socket::async_read_some.
                    //!
                    //! \param error error code return by the underlying system
                    //! call.
                    //! \param bytes_transferred the number of transferred
                    //! bytes.
                    //!
                    void on_read( 
                        const boost::system::error_code &error , 
                        size_t bytes_transfered );

                    //! \brief callback method for the
                    //! boost::asio::ip::tcp::socket::async_write_some.
                    //!
                    //! \param error error code returned by the underlying
                    //! system call.
                    //! 
                    void on_write(
                        const boost::system::error_code &error);
                    
                    
                    //! \brief socket object with its lifetime bind to the
                    //! ClientConnection one.
                    //!
                    std::auto_ptr< boost::asio::ip::tcp::socket > _socket_ptr;
                    
                    //! \brief reference to the server class to allow callback
                    //! methods( manager pattern used for disconnection ).
                    //!
                    EchoServer &_echo_server;
                    
                    //! \brief maximum length of the internal buffer.
                    //!
                    static const uint32_t MAX_LENGTH;
                    
                    //! \brief buffer to hold data received on the network.
                    //!
                    char* _data;
            };
        }
    }
}





#endif
