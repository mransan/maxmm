#include <iostream>

#include <TimeTest.h>
#include <ConditionTest.h>
#include <LockFreeWrapperTest.h>
#include <ThreadTest.h>
#include <TestCommon.h>

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>


int main()
{
    CppUnit::TestResult         result;
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(maxmm::test::TimeTest::getSuite()) ;
    runner.addTest(maxmm::test::ConditionTest::getSuite()) ;
    runner.addTest(maxmm::test::ThreadTest::getSuite()) ;
    runner.addTest(maxmm::test::LockFreeWrapperTest::getSuite()) ;

    bool ret = !runner.run(); 
    return ret;
}
