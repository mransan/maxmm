/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <iostream>

#include <boost/asio.hpp>

int main( void )
{
    boost::asio::io_service io_service;
    
    boost::asio::ip::tcp::socket socket( io_service );

    socket.connect( boost::asio::ip::tcp::endpoint( 
        boost::asio::ip::tcp::v4( ) , 
        2333 ) );

    std::string str = "max is a moron" ;
    
    boost::asio::write( 
        socket ,
        boost::asio::buffer( str.c_str( ) , str.size( ) ) );

    return 0;
}
