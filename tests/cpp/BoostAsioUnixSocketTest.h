/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_BoostAsioUnixSocketTest_h
#define maxmm_BoostAsioUnixSocketTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/enable_shared_from_this.hpp> 

namespace maxmm
{
    namespace test
    {

        //! \brief Test suite for the  UNIX domain socket of the boost::asio library.
        //!
        //! Both tests work for themselves. The child process plays the role of
        //! an echo server by sending back what it has received. The parent
        //! process verifies that the data sent back is the same that it has
        //! previously sent. 
        //! This kind of IPC mechanism is quite usefull. The test
        //! test_transfer_connected is quite usefull for forked processes while
        //! the test test_transfert_file shows how 2 unrelated process can
        //! communicate through a UNIX socket file.
        //!
        class BoostAsioUnixSocketTest : public CppUnit::TestFixture
        {
            public:
                //! \brief this class allows asyncronous read.
                //!
                class RecvHandle
                {
                    public:
                        //! \brief Constructor
                        //!
                        //! The constructor performs a first blocking read on the
                        //! socket, send back the data to the client and finally
                        //! register the on_receive method for asynchronous read.
                        //!
                        //! \param socket the socket to read and echo the
                        //!  received data.
                        //! \param acceptor the aceptor.
                        //!
                        RecvHandle( 
                                boost::asio::local::stream_protocol::socket & 
                                    socket  , 
                                boost::asio::local::stream_protocol::acceptor & 
                                    acceptor);
                        
                        //! \brief accept handle. 
                        //!
                        //! call back method for when a client connect to the
                        //! endpoint.
                        //!
                        void on_accept( const boost::system::error_code & ec );
                        
                        //! \brief receive handle/call back functions. This
                        //! method will be call whenever data is read from the
                        //! socket.
                        //!
                        //! This method must be follow the expected signature(
                        //! boost::functions2< void , const error_code , size_t
                        //! ).
                        //!
                        //! \param ec the error code of the read operation.
                        //! \param len the number of bytes read.
                        //!
                        void on_receive( 
                                const boost::system::error_code & ec , 
                                size_t len );
                       
                    private:
                        //! \brief the socket to read and echo.
                        //!
                        boost::asio::local::stream_protocol::socket &_socket;

                        boost::asio::local::stream_protocol::acceptor &_acceptor;
                        
                        //! \brief read buffer.
                        //!
                        boost::array< char , 1024 >                   _buffer;
                };           
                
                //! \brief test setup.
                //!
                void setUp( void );
                
                //! \brief test tear down.
                //!
                void tearDown( void );

                //! \brief this test focuses on the use of the
                //!boost::asio::local::connect_pair( ).
                //!
                //! this test execise the ::boost::asio::local::connect_pair method which open 2
                //! sockets and conect them together before the call to the
                //! ::fork( ) method. This kind of mechanism allows very simple
                //! and efficient inter process communication between a parent and
                //! its child.
                //!
                void test_transfer_connected( void );
                
                
                
                //! \brief This test shows how UNIX socket file can be used
                //! between 2 unrelated processes to communicate together. 
                //!
                void test_transfer_file( void );
                
                
                //! \brief similar test as test_transfer_connected but the
                //! child( echo server ) uses asynchronous read mode.
                //!
                void test_transfer_connected_as(  void );
               
                static CppUnit::TestSuite* getSuite( void );
        
        };
    }
}

#endif
