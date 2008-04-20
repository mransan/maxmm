/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_OBSERVABLE_TEST_H
#define MAXUTILS_OBSERVABLE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxutils/Observable.h>
#include <maxutils/Observer.h>

class ObservableTest : public CppUnit::TestFixture
{
private:
public:
    class ObserverTest : public maxutils::Observer
    {
    private:
        int m_iter;
    public:
        ObserverTest();
        ~ObserverTest();
        void notify();
        int  iter();
    };

    void setUp()
    {
    }
    void tearDown()
    {
    }
    void testMain();
    static CppUnit::TestSuite* getSuite();
};




#endif
