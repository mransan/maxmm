/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_THREAD_TEST_H
#define MAXUTILS_THREAD_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxutils/Thread.h>

class ThreadTest : public CppUnit::TestFixture
{
private:
    class TThread : public maxutils::Thread
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
