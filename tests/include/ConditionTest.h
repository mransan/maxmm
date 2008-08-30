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

class ConditionTest : public CppUnit::TestFixture
{
public:

    class ThreadTest : public maxmm::Thread
    {
    private:
        int                  m_id;
        maxmm::Condition &m_cd;
    public:
        ThreadTest(maxmm::Condition& _cd, int _id);
        virtual void run();
        ~ThreadTest();
    };

    ConditionTest()
    {
    }
    ~ConditionTest()
    {
    }
    void setUp()
    {}
    void tearDown()
    {}
    void testSignal();
    static CppUnit::TestSuite* getSuite();
};
#endif
