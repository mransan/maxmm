#include "ThreadTest.h"
#include <maxmm/Time.h>
#include <maxmm/Thread.h>
#include <maxmm/Logger.h>
#include <cppunit/TestAssert.h>

using namespace maxmm;

ThreadTest::TThread::TThread()
:m_iter(0)
{

}

ThreadTest::TThread::TThread(int i)
:m_iter(i)
{
}

ThreadTest::TThread::~TThread()
{

}

int ThreadTest::TThread::getInt()
{
    return m_iter;
}

void ThreadTest::TThread::run()
{
    m_iter ++;
}
void ThreadTest::testThread()
{
    LOG_INFO << std::endl;
    LOG_INFO << "==== test thread ====" << std::endl;

    TThread tt(23);
    tt.start();
    Time::sleep(3);
    tt.should_stop();
    tt.join();
    
    int result = tt.getInt();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("thread result", 24, result);
}


CppUnit::TestSuite* ThreadTest::getSuite()
{
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest( new CppUnit::TestCaller<ThreadTest>( "testSignal", 
                                                      &ThreadTest::testThread 
                                                   ) 
                );
    return suite;
}
