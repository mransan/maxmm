#include "TimeTest.h"
#include <maxutils/Logger.h>
#include <maxutils/Time.h>

#include <cppunit/TestAssert.h>

using namespace maxutils;

void TimeTest::testEquality()
{
    LOG_INFO << std::endl;
    LOG_INFO << "==== test time test equality ====" << std::endl;
    LOG_INFO << std::endl;

    Time t1;
    Time t2;
    CPPUNIT_ASSERT_MESSAGE("test equality test 1", t2 == t1);

    t1 = Time::now();
    t2 = t1;
    CPPUNIT_ASSERT_MESSAGE("test equality test 2", t2 == t1);

    t1 = -0.0001;
    t2 =  0.0001;
    t2 = t2 + t1;
    t1 = 0;
    
    LOG_INFO << "t1 : " << t1 << std::endl;
    LOG_INFO << "t2 : " << t2 << std::endl;

    CPPUNIT_ASSERT_MESSAGE("test equality test 3", t2 == t1);

}
void TimeTest::testSign()
{
    LOG_INFO << std::endl;
    LOG_INFO << "==== test time test sign ====" << std::endl;
    LOG_INFO << std::endl;
    
    Time t1 = Time::now();
    sleep(1);
    Time t2 = Time::now();

    CPPUNIT_ASSERT_MESSAGE("test superior" , t2 >= t1);
    CPPUNIT_ASSERT_MESSAGE("test inferior" , t1 <= t2);
    CPPUNIT_ASSERT_MESSAGE("test superior" , t2 >  t1);
    CPPUNIT_ASSERT_MESSAGE("test inferior" , t1 <  t2);
    
    t1 = 0;
    t2 = t1 + 0.00001;
    CPPUNIT_ASSERT_MESSAGE("test superior" , t2 >= t1);
    CPPUNIT_ASSERT_MESSAGE("test inferior" , t1 <= t2);
    CPPUNIT_ASSERT_MESSAGE("test superior" , t2 >  t1);
    CPPUNIT_ASSERT_MESSAGE("test inferior" , t1 <  t2);
    
    t1 = 0;
    t2 = t1 - 0.00001;
    CPPUNIT_ASSERT_MESSAGE("test superior" , t2 <= t1);
    CPPUNIT_ASSERT_MESSAGE("test inferior" , t1 >= t2);
    CPPUNIT_ASSERT_MESSAGE("test superior" , t2 <  t1);
    CPPUNIT_ASSERT_MESSAGE("test inferior" , t1 >  t2);

}

CppUnit::TestSuite* TimeTest::getSuite()
{
    
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest( new CppUnit::TestCaller<TimeTest>( "testEquality", 
                                                      &TimeTest::testEquality 
                                                   ) 
                );
    suite->addTest( new CppUnit::TestCaller<TimeTest>( "testSign", 
                                                     &TimeTest::testSign
                                                   ) 
                );
    return suite;
}

