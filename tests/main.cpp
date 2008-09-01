#include <iostream>

#include <TimeTest.h>
#include <ConditionTest.h>
#include <LockFreeWrapperTest.h>
#include <ThreadTest.h>
#include <RandomUniformTest.h>
#include <BoostAsioUnixSocketTest.h>

#include <TestCommon.h>

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>


int main( int argc , char ** argv)
{
    CppUnit::TestResult         result;
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(maxmm::test::TimeTest::getSuite()) ;
    runner.addTest(maxmm::test::ConditionTest::getSuite()) ;
    runner.addTest(maxmm::test::ThreadTest::getSuite()) ;
    runner.addTest(maxmm::test::LockFreeWrapperTest::getSuite()) ;
    runner.addTest(maxmm::test::RandomUniformTest::getSuite()) ;
    runner.addTest(maxmm::test::BoostAsioUnixSocketTest::getSuite( ) );
    bool ret = false ;

    if( argc == 2 )
    {
        ret = !runner.run( argv[1] ) ;
    }
    else
    {
        ret = !runner.run(); 
    }
    return ret;
}
