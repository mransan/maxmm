#include <iostream>
#include <maxmm/all.h>

#include <TimeTest.h>
#include <ConditionTest.h>
#include <LockFreeWrapperTest.h>
#include <ThreadTest.h>
#include <TestCommon.h>
#include <ObservableTest.h>

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace maxmm;

int main()
{

    
    initLogger();
    
    LOG_INFO << "============" << std::endl;
	LOG_INFO << " TEST SUITE " << std::endl;
	LOG_INFO << "============" << std::endl;

	std::cout << std::endl;
    
    CppUnit::TestResult         result;
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(TimeTest::getSuite()) ;
    runner.addTest(ConditionTest::getSuite()) ;
    runner.addTest(ThreadTest::getSuite()) ;
    runner.addTest(ObservableTest::getSuite()) ;
    runner.addTest(maxmm::test::LockFreeWrapperTest::getSuite()) ;

    bool ret = !runner.run(); 
    Logger::close();
    return ret;
}
