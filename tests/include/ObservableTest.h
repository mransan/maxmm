/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ObservableTest_h
#define maxmm_ObservableTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <maxmm/Observable.h>
#include <maxmm/Observer.h>

class ObservableTest : public CppUnit::TestFixture
{
private:
public:
    class ObserverTest : public maxmm::Observer
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
