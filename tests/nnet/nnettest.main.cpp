#include <NeuronTest.h>

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>

int main(int argc , char ** argv)
{
    
    CppUnit::TestResult         result;
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(maxmm::test::NeuronTest::getSuite());
    
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
