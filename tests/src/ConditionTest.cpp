#include "ConditionTest.h"
#include <maxmm/Time.h>

#include <cppunit/TestAssert.h>

using namespace maxmm;
namespace maxmm
{
    namespace test
    {

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
                Time::sleep(2);
                m_cd.broadcast();
            }
            else
            {
                m_cd.wait();
            }
        }
        
        void ConditionTest::testSignal()
        {
            Mutex      mutex;
            ScopeLock  sl(mutex);
            Condition  condition(sl);
        
            ThreadTest tt1(condition, 1);
            ThreadTest tt2(condition, 2);
        
            tt1.start( );
            tt2.start( );
        
            Time::sleep( 5 );
        
            tt1.join( );
            tt2.join( );
        }
        
        CppUnit::TestSuite* ConditionTest::getSuite()
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller<ConditionTest>( "test_signaling", 
                                                              &ConditionTest::testSignal 
                                                           ) 
                        );
            return suite;
        }
    }
}
