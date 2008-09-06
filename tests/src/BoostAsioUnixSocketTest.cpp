#include <BoostAsioUnixSocketTest.h>
#include <TestCommon.h>

#include <sstream>

namespace maxmm
{
    namespace test
    {
        void BoostAsioUnixSocketTest::setUp( void )
        {
            // No - Op. 
        }

        void BoostAsioUnixSocketTest::tearDown( void )
        {
            // No - Op.
        }
       

        void BoostAsioUnixSocketTest::test_transfer_connected(  void )
        {
            boost::asio::io_service io;
            
            //
            // Create 2 Unix sockets.
            //
            boost::asio::local::stream_protocol::socket socket_send ( io );
            boost::asio::local::stream_protocol::socket socket_recv ( io );
            
            //
            // Connect the 2 socket together.
            //
            boost::asio::local::connect_pair(socket_send, socket_recv);
            
            
            std::string msg( "message from parent to child" );

            
            ::pid_t pid = ::fork( ) ;
            if ( pid != 0 )
            {
                //
                // Close the socket of the child.
                //
                socket_recv.close( );

                //
                // Ensure that the child will be killed when this object goes
                // out of scope.
                //
                ProcessKiller pk( pid );

                //
                // Send the data to the child.
                //
                boost::system::error_code ec;
                socket_send.write_some( boost::asio::buffer( msg ) , ec) ;
                CPPUNIT_ASSERT_MESSAGE(ec.message( )  , ec.value( ) == 0 );

                //
                // Receive data from child.
                //
                boost::array< char , 1024 > buffer;
                size_t len = socket_send.read_some( boost::asio::buffer( buffer ) , ec );

                //
                // Make sure the data received is the one sent.
                //
                std::ostringstream ss;
                ss.write( buffer.data( ) , len );
                CPPUNIT_ASSERT_EQUAL( msg, ss.str( ) ) ;
            }
            else
            {
                //
                // Close the socket of the parent.
                //
                socket_send.close( );

                //
                // Echo loop.
                // The goal of this loop is to echo what the socket received.
                //
                while( 1 ) 
                {
                    boost::system::error_code ec;
                    boost::array< char , 1024 > buffer ;
                    size_t len = socket_recv.read_some(
                        boost::asio::buffer( buffer ) , 
                        ec ) ;
                    std::ostringstream ss;
                    ss.write( buffer.data( ) , len );
                    socket_recv.write_some( boost::asio::buffer( ss.str( ) ) ); 
                }
            }

        } 
        
        void BoostAsioUnixSocketTest::test_transfer_file( void )
        {
            boost::asio::io_service io;
            
            //
            // Create 2 Unix socket.
            //
            boost::asio::local::stream_protocol::socket socket_send ( io );
            boost::asio::local::stream_protocol::socket socket_recv ( io );
            

            std::string unix_file =  "/tmp/boost_asio_unix_file" ;
            static_cast< void >( ::unlink( unix_file.c_str( ) ) );

            std::string msg( "message from parent to child" );

            
            ::pid_t pid = ::fork( ) ;
            if ( pid != 0 )
            {
                //
                // open the socket.
                //
                boost::asio::local::stream_protocol::endpoint ep( unix_file );
                socket_send.connect( ep );


                //
                // Ensure that the child will be killed when this object goes
                // out of scope.
                //
                ProcessKiller pk( pid );

                //
                // Send the data to the child.
                //
                boost::system::error_code ec;
                socket_send.write_some( boost::asio::buffer( msg ) , ec) ;
                CPPUNIT_ASSERT_MESSAGE(ec.message( )  , ec.value( ) == 0 );

                //
                // Receive data from child.
                //
                boost::array< char , 1024 > buffer;
                size_t len = socket_send.read_some( boost::asio::buffer( buffer ) , ec );

                //
                // Make sure the data received is the one sent.
                //
                std::ostringstream ss;
                ss.write( buffer.data( ) , len );
                CPPUNIT_ASSERT_EQUAL( msg, ss.str( ) ) ;
            }
            else
            {
                boost::asio::local::stream_protocol::endpoint ep( unix_file );
                boost::asio::local::stream_protocol::acceptor acceptor( io , ep );
                acceptor.accept( socket_recv );

                //
                // Echo loop.
                // The goal of this loop is to echo what the socket received.
                //
                while( 1 ) 
                {
                    boost::system::error_code ec;
                    boost::array< char , 1024 > buffer ;
                    size_t len = socket_recv.read_some(
                        boost::asio::buffer( buffer ) , 
                        ec ) ;
                    std::ostringstream ss;
                    ss.write( buffer.data( ) , len );
                    socket_recv.write_some( boost::asio::buffer( ss.str( ) ) ); 
                }
            }
        } 
        
        //
        // RecvHandle implementation.
        //
        //
        
        BoostAsioUnixSocketTest::RecvHandle::RecvHandle( 
                boost::asio::local::stream_protocol::socket & socket , 
                boost::asio::local::stream_protocol::acceptor & acceptor )
        : _socket( socket ) , _acceptor( acceptor )
        {
            _acceptor.async_accept(
                    _socket ,
                    boost::bind( 
                        &RecvHandle::on_accept , 
                        this , 
                        boost::asio::placeholders::error ) );
        }
        
        void BoostAsioUnixSocketTest::RecvHandle::on_accept(
                const boost::system::error_code & ec )
        {
            //
            // When connected start reading since we are the server.
            //
            _socket.async_read_some(
                    boost::asio::buffer( _buffer ) , 
                    boost::bind(
                        &RecvHandle::on_receive , 
                        this  , 
                        boost::asio::placeholders::error , 
                        boost::asio::placeholders::bytes_transferred ) );
           
        }

        void BoostAsioUnixSocketTest::RecvHandle::on_receive ( 
                const boost::system::error_code & ec , 
                size_t len )
        {
            std::ostringstream ss;
            ss.write( _buffer.data( ) , len );
            //
            // Echo the received data.
            //
            _socket.write_some( boost::asio::buffer( ss.str( ) ) );
            
            //
            // re do the same thing.
            //
            _socket.async_read_some( 
                    boost::asio::buffer( _buffer ) , 
                    boost::bind( 
                        &RecvHandle::on_receive ,
                        this ,
                        boost::asio::placeholders::error , 
                        boost::asio::placeholders::bytes_transferred ) );
        }

        void BoostAsioUnixSocketTest::test_transfer_connected_as(  void )
        {
            boost::asio::io_service io;
            
            //
            // Create 2 Unix sockets.
            //
            boost::asio::local::stream_protocol::socket socket_send ( io );
            boost::asio::local::stream_protocol::socket socket_recv ( io );
            
            std::string unix_file =  "/tmp/boost_asio_unix_file" ;
            static_cast< void >( ::unlink( unix_file.c_str( ) ) );

            
            ::pid_t pid = ::fork( ) ;
            if ( pid != 0 )
            {
                //
                // Close the socket of the child.
                //
                socket_recv.close( );
                
                //
                // wait for the child to start up.
                //
                ::sleep( 1 );
                boost::asio::local::stream_protocol::endpoint ep( unix_file );
                socket_send.connect( ep );

                //
                // Ensure that the child will be killed when this object goes
                // out of scope.
                //
                ProcessKiller pk( pid );
                
                for( int i=0 ; i < 10 ; ++i )
                {
                    std::ostringstream ss_send;
                    ss_send << "message id # " << i << std::endl;
                    
                    //
                    // Send the data to the child.
                    //
                    boost::system::error_code ec;
                    socket_send.write_some( 
                            boost::asio::buffer( ss_send.str( ) ) , 
                            ec) ;
                    CPPUNIT_ASSERT_MESSAGE(ec.message( )  , ec.value( ) == 0 );

                    
                    //Receive data from child.
                    
                    boost::array< char , 1024 > buffer;
                    size_t len = socket_send.read_some( 
                            boost::asio::buffer( buffer ) , 
                            ec );

                    
                    //Make sure the data received is the one sent.
                    
                    std::ostringstream ss_recv;
                    ss_recv.write( buffer.data( ) , len );
                    CPPUNIT_ASSERT_EQUAL( ss_send.str( ), ss_recv.str( ) ) ;
                }
            }
            else
            {
                //
                // Close the socket of the parent.
                //
                socket_send.close( );
                
                boost::asio::local::stream_protocol::endpoint ep( unix_file );
                boost::asio::local::stream_protocol::acceptor acceptor( io , ep );
   
                RecvHandle handle( socket_recv  , acceptor );
                //
                // Echo loop.
                // The goal of this loop is to echo what the socket received.
                //
                std::cout << "child running" << std::endl; 
                io.run( );
            }
        }

        CppUnit::TestSuite* BoostAsioUnixSocketTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<BoostAsioUnixSocketTest>( 
                        "BoostAsioUnixSocketTest::test_transfer_connected", 
                        &BoostAsioUnixSocketTest::test_transfer_connected ) 
                        );
            suite->addTest( new CppUnit::TestCaller<BoostAsioUnixSocketTest>( 
                        "BoostAsioUnixSocketTest::test_transfer_file", 
                        &BoostAsioUnixSocketTest::test_transfer_file ) 
                        );
            suite->addTest( new CppUnit::TestCaller<BoostAsioUnixSocketTest>( 
                        "BoostAsioUnixSocketTest::test_transfer_connected_as", 
                        &BoostAsioUnixSocketTest::test_transfer_connected_as ) 
                        );
            
            return suite;
        }
    
    }
}
