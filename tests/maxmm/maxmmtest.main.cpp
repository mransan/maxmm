#include <ConditionTest.h>
#include <LockFreeQueueTest.h>
#include <LockFreeWrapperTest.h>
#include <NullableValueTest.h>
#include <RandomUniformTest.h>
#include <StringUtilsTest.h>
#include <ThreadControllerTest.h>
#include <ThreadPoolTest.h>
#include <ThreadTest.h>
#include <TimeTest.h>
#include <XmlDecoderTest.h>
#include <XmlEncoderTest.h>


#include <TestCommon.h>

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>


int main(int argc , char ** argv)
{
    
    CppUnit::TestResult         result;
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(maxmm::test::ConditionTest::getSuite()) ;
    runner.addTest(maxmm::test::LockFreeQueueTest::getSuite());
    runner.addTest(maxmm::test::LockFreeWrapperTest::getSuite()) ;
    runner.addTest(maxmm::test::NullableValueTest::getSuite());
    runner.addTest(maxmm::test::RandomUniformTest::getSuite()) ;
    runner.addTest(maxmm::test::StringUtilsTest::getSuite());
    runner.addTest(maxmm::test::ThreadControllerTest::getSuite());
    runner.addTest(maxmm::test::ThreadPoolTest::getSuite());
    runner.addTest(maxmm::test::ThreadTest::getSuite()) ;
    runner.addTest(maxmm::test::TimeTest::getSuite()) ;
    runner.addTest(maxmm::test::XmlDecoderTest::getSuite());
    runner.addTest(maxmm::test::XmlEncoderTest::getSuite());
    
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
