/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ThreadTest_h
#define maxmm_ThreadTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxmm/Thread.h>

namespace maxmm
{
    namespace test
    {

        
        //! \brief this test suite the good behavior of the maxmm::Thread class.
        //! 
        //! The current only test makes sure that the run method was correctly
        //! executed.
        //!
        class ThreadTest : public CppUnit::TestFixture
        {
            public:
                void setUp( void );
                void tearDown( void );
                void test_thread_run( void );
                
                static CppUnit::TestSuite* getSuite();
            private:
                
                //! \brief very simple thread implementation for the purpose of the
                //!  test
                //!
                //! This thread has a single member variable that gets incremented on
                //! the run function. An accessor method return this member variable so
                //! that the client can verify that it was correctly implemented.
                //!
                class TThread : public maxmm::Thread
                {
                    public:
                        TThread(int i);
                        ~TThread( void );
                        virtual void run( void );
                        int get_iter( void );
                    private:
                        int _iter;

                };

        
        };
    }
}



#endif
