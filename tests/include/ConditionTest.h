/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ConditionTest_h
#define maxmm_ConditionTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxmm/Condition.h>
#include <maxmm/Thread.h>
#include <maxmm/ThreadController.h>

namespace maxmm
{
    namespace test
    {

        class ConditionTest : public CppUnit::TestFixture
        {
            public:
            
                //! \brief Thread class to test signaling between 2 threads.
                //!
                class ThreadTest 
                    : public maxmm::Thread<maxmm::OnceController>
                {
                    public:
                        ThreadTest(maxmm::Condition& condition, int id);
                        ~ThreadTest( void );
                        
                        int iter( void );

                    protected:
                        virtual void loop( void );
                        virtual void init( void ){ };
                        virtual void clean( void ){ };
                        
                        int                _id;
                        maxmm::Condition & _condition;
                        int                _iter;
                       
                };
            
                ConditionTest( void );
                ~ConditionTest( void );
                void setUp( void );
                void tearDown( void );
                
                void test_signaling( void );
                
                static CppUnit::TestSuite* getSuite( void );
        };
    }   
}
#endif
