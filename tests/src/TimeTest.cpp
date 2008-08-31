#include "TimeTest.h"
#include <maxmm/Time.h>

#include <cppunit/TestAssert.h>

namespace maxmm
{
    namespace test
    {

        void TimeTest::setUp( void )
        {
            // No - Op.
        }

        void TimeTest::tearDown( void )
        {
            // No - Op.
        }

        void TimeTest::test_equality( void )
        {
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
            
            CPPUNIT_ASSERT_MESSAGE("test equality test 3", t2 == t1);
        
        }
        void TimeTest::test_sign( void )
        {
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
            
            suite->addTest( new CppUnit::TestCaller<TimeTest>( "test_equality", 
                                                              &TimeTest::test_equality
                                                           ) 
                        );
            suite->addTest( new CppUnit::TestCaller<TimeTest>( "test_sign", 
                                                             &TimeTest::test_sign
                                                           ) 
                        );
            return suite;
        }
    }
}
