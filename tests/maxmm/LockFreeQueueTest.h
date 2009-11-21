/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_LockFreeQueueTest_h
#define maxmm_LockFreeQueueTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace maxmm
{
    namespace test
    {
        
        //! \brief test class for the LockFreeQueue class.
        //!
        class LockFreeQueueTest : CppUnit::TestFixture
        {
            public:
                virtual ~LockFreeQueueTest( void ) { }
                void setUp( void ) { };
                void tearDown( void ) { };

                //! \brief verify the pop operation.
                //!
                void test_pop( void );
                
                //! \brief verify the push operation.
                //!
                void test_push( void );
                
                //! \brief benchmark the queue within a single thread.
                //!
                void test_perf( void );
                
                //! \brief benchmark the queue using multithreads.
                //!
                void test_mt_perf( void );
                
                //! \brief return the test suite.
                //!
                static CppUnit::TestSuite* getSuite( void );
            private:
        };
    }
}






#endif
