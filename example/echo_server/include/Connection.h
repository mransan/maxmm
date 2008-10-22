/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_example_echo_server_Condition_h
#define maxmm_example_echo_server_Condition_h

#include <boost/asio.hpp>

namespace maxmm
{
    namespace example
    {
        namespace echo_server
        {
            class EchoServer;
            
            class Connection
            {
                public:
                    Connection( 
                        boost::asio::ip::tcp::socket *socket , 
                        EchoServer &echo_server );
                    
                    ~Connection( void );

                    void start( void );
                private:

                    void on_read( 
                        const boost::system::error_code &error , 
                        size_t bytes_transfered );

                    void on_write(
                        const boost::system::error_code &error);
                    std::auto_ptr< boost::asio::ip::tcp::socket > _socket_ptr;
                    EchoServer &_echo_server;
                    
                    static const uint32_t MAX_LENGTH;
                    char* _data;
            };
        }
    }
}





#endif
