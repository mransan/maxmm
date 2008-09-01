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
            
                void setUp( void );
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
                
                
                static CppUnit::TestSuite* getSuite( void );
        
        };
    }
}

#endif
