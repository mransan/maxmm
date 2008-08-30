#include<ObservableTest.h>



ObservableTest::ObserverTest::ObserverTest()
:   m_iter(0),
    maxmm::Observer()
{
}

ObservableTest::ObserverTest::~ObserverTest()
{
}

void ObservableTest::ObserverTest::notify()
{
    m_iter ++;
}
int ObservableTest::ObserverTest::iter()
{
    return m_iter;
}


void ObservableTest::testMain()
{
    maxmm::Observable  observable;
    ObserverTest    ob1;
    ObserverTest    ob2;

    CPPUNIT_ASSERT_EQUAL(0 , ob1.iter());
    CPPUNIT_ASSERT_EQUAL(0 , ob2.iter());
    CPPUNIT_ASSERT_EQUAL(0 , static_cast<int>(observable.size()));


    observable.registerObs(&ob1);
    observable.registerObs(&ob2);
    CPPUNIT_ASSERT_EQUAL(2 , static_cast<int>(observable.size()));

    observable.notifyAll();

    CPPUNIT_ASSERT_EQUAL(1 , ob1.iter());
    CPPUNIT_ASSERT_EQUAL(1 , ob2.iter());

    observable.unregisterObs(&ob1);
    observable.unregisterObs(&ob2);
    
    CPPUNIT_ASSERT_EQUAL(0 , static_cast<int>(observable.size()));

}

CppUnit::TestSuite* ObservableTest::getSuite()
{
    
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest( new CppUnit::TestCaller<ObservableTest>( "testMain", 
                                                      &ObservableTest::testMain 
                                                   ) 
                );
    return suite;
}

