#include <iostream>

#include <TimeTest.h>
#include <ConditionTest.h>
#include <LockFreeWrapperTest.h>
#include <ThreadTest.h>
#include <RandomUniformTest.h>
#include <BoostAsioUnixSocketTest.h>
#include <StdContainerTest.h>
#include <ThreadControllerTest.h>
#include <TypeTest.h>
#include <ThreadPoolTest.h>
#include <LockFreeQueueTest.h>
#include <IomanipTest.h>
#include <ExceptionTest.h>
#include <XmlDecoderTest.h>
#include <XmlEncoderTest.h>
#include <StringUtilsTest.h>
#include <NullableValueTest.h>
#include <AgentTest.h>
#include <AssignmentTest.h>
#include <ABTOkMessageTest.h>
#include <ABTNoGoodMessageTest.h>

#include <TestCommon.h>

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>


int main(int argc , char ** argv)
{
    
    CppUnit::TestResult         result;
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(maxmm::test::TimeTest::getSuite()) ;
    runner.addTest(maxmm::test::ConditionTest::getSuite()) ;
    runner.addTest(maxmm::test::ThreadTest::getSuite()) ;
    runner.addTest(maxmm::test::LockFreeWrapperTest::getSuite()) ;
    runner.addTest(maxmm::test::RandomUniformTest::getSuite()) ;
    runner.addTest(maxmm::test::BoostAsioUnixSocketTest::getSuite());
    runner.addTest(maxmm::test::StdContainerTest::getSuite());
    runner.addTest(maxmm::test::ThreadControllerTest::getSuite());
    runner.addTest(maxmm::test::TypeTest::getSuite());
    runner.addTest(maxmm::test::ThreadPoolTest::getSuite());
    runner.addTest(maxmm::test::LockFreeQueueTest::getSuite());
    runner.addTest(maxmm::test::IomanipTest::getSuite());
    runner.addTest(maxmm::test::ExceptionTest::getSuite());
    runner.addTest(maxmm::test::XmlDecoderTest::getSuite());
    runner.addTest(maxmm::test::XmlEncoderTest::getSuite());
    runner.addTest(maxmm::test::StringUtilsTest::getSuite());
    runner.addTest(maxmm::test::NullableValueTest::getSuite());
    runner.addTest(maxmm::test::AgentTest::getSuite());
    runner.addTest(maxmm::test::AssignmentTest::getSuite());
    runner.addTest(maxmm::test::ABTOkMessageTest::getSuite());
    runner.addTest(maxmm::test::ABTNoGoodMessageTest::getSuite());

    
    bool ret = false ;

    if(argc == 2)
    {
        ret = !runner.run(argv[1]) ;
    }
    else
    {
        ret = !runner.run(); 
    }
    return ret;
}
