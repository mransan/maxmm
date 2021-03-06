/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <iostream>
#include <MultiProcessEchoServer.h>

int main( void )
{
    boost::asio::io_service io_service;
    
    maxmm::example::echo_server::MultiProcessEchoServer echo_server(
        io_service , 
        2333);
    
    io_service.run( );
    
    return 0;
}
