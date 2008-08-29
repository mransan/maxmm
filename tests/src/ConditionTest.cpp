#include "ConditionTest.h"
#include <maxutils/Logger.h>
#include <maxutils/Time.h>

#include <cppunit/TestAssert.h>

using namespace maxutils;


ConditionTest::ThreadTest::ThreadTest(Condition &_cd, int _id)
:m_cd(_cd), m_id(_id)
{

}
ConditionTest::ThreadTest::~ThreadTest()
{
}

void ConditionTest::ThreadTest::run ()
{

    if(m_id == 1)
    {
        LOG_INFO    << "Thread id:" << m_id << " sleeping" << std::endl;
        Time::sleep(2);
        LOG_INFO    << "Thread id:" << m_id << " broadcasting" << std::endl;
        m_cd.broadcast();
    }
    else
    {
        LOG_INFO    << "Thread id:" << m_id << " waiting for condition"
                    << std::endl;
        m_cd.wait();
        LOG_INFO    << "Thread id:" << m_id << " end waiting for condition"
                    << std::endl;
    }
}

void ConditionTest::testSignal()
{
    LOG_INFO << std::endl;
    LOG_INFO << "==== test signal ===="    << std::endl;

    LOG_INFO << std::endl;
    
    Mutex      mutex;
    ScopeLock  sl(mutex);
    Condition  condition(sl);

    ThreadTest tt1(condition, 1);
    ThreadTest tt2(condition, 2);

    tt1.start( );
    tt2.start( );

    Time::sleep(10);

    tt1.should_stop( );
    tt2.should_stop( );
    tt1.join( );
    tt2.join( );
}

CppUnit::TestSuite* ConditionTest::getSuite()
{
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest( new CppUnit::TestCaller<ConditionTest>( "testSignal", 
                                                      &ConditionTest::testSignal 
                                                   ) 
                );
    return suite;
}
