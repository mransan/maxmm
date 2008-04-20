/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_TESTTIME_H
#define MAXUTILS_TESTTIME_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <maxutils/all.h>

class TimeTest : public CppUnit::TestFixture
{

public:
    void setUp()
    {}
    void tearDown()
    {}
    void testEquality();
    void testSign();
    static CppUnit::TestSuite* getSuite();
};



#endif

