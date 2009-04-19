/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/NullableValue.h>
#include <NullableValueTest.h>
namespace 
{
}
namespace maxmm
{
namespace test
{
                
NullableValueTest::NullableValueTest(void)
{

}

NullableValueTest::~NullableValueTest(void)
{

}


void NullableValueTest::setUp(void)
{

}

void NullableValueTest::tearDown(void)
{

}

void NullableValueTest::test_simple(void)
{
    {
        maxmm::NullableValue<uint32_t> value;

        CPPUNIT_ASSERT(value.null());
    } 
    {
        maxmm::NullableValue<uint32_t> value;
        value.makeValue() = 3;

        CPPUNIT_ASSERT(!value.null());
        CPPUNIT_ASSERT_EQUAL((uint32_t)3 , value.value());
    }
    {
        maxmm::NullableValue<uint32_t> value;
        maxmm::NullableValue<uint32_t> copy(value);

        CPPUNIT_ASSERT(value.null());
        CPPUNIT_ASSERT(copy.null());
    }
    {    
        maxmm::NullableValue<uint32_t> value(3);
        maxmm::NullableValue<uint32_t> copy(value);
    
        CPPUNIT_ASSERT(!value.null());
        CPPUNIT_ASSERT(!copy.null());
    }
    {       
        maxmm::NullableValue<uint32_t> value(3);
        maxmm::NullableValue<uint32_t> copy;
    
        CPPUNIT_ASSERT(!value.null());
        CPPUNIT_ASSERT(copy.null());

        copy = value;

        CPPUNIT_ASSERT_EQUAL((uint32_t)(3),copy.value()); 
    }
}

CppUnit::TestSuite *NullableValueTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<NullableValueTest>(
            "NullableValueTest::test_simple",
            &NullableValueTest::test_simple));
        
     return suite; 
}
}
}
