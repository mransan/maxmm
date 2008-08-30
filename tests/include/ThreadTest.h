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

class ThreadTest : public CppUnit::TestFixture
{
private:
    class TThread : public maxmm::Thread
    {
    private:
        int m_iter;

    
    public:
        TThread();
        TThread(int i);
        ~TThread();
        virtual void run();
        int getInt();
    };

public:
    void setUp()
    {
    }
    void tearDown()
    {
    }
    void testThread();
    static CppUnit::TestSuite* getSuite();
};




#endif
