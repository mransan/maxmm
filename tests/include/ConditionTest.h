/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_CONDITION_TEST_H
#define MAXUTILS_CONDITION_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxutils/Condition.h>
#include <maxutils/Thread.h>
class ConditionTest : public CppUnit::TestFixture
{
public:

    class ThreadTest : public maxutils::Thread
    {
    private:
        int                  m_id;
        maxutils::Condition &m_cd;
    public:
        ThreadTest(maxutils::Condition& _cd, int _id);
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
