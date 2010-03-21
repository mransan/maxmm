/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/NullableValue.h>
#include <NullableValueTest.h>
namespace 
{
class IsAClass
{
public:
    explicit IsAClass(uint32_t *i);
    IsAClass(IsAClass const& copy);
    ~IsAClass(void);
private:
    uint32_t *_i;
};

IsAClass::IsAClass(uint32_t *i)
:   _i(i)
{
    std::cout << "constructor" << std::endl;
    ++(*_i);
}

IsAClass::~IsAClass(void)
{
    std::cout << "destructor" << std::endl;
    --(*_i);
}

IsAClass::IsAClass(IsAClass const& copy)
:   _i(copy._i)
{
    std::cout << "copy constructor" << std::endl;
    ++(*_i);
}


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
        value.make_value() = 3;

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
    {
        maxmm::NullableValue<uint32_t> value(3);

        CPPUNIT_ASSERT(!value.null());
        
        value.make_null();

        CPPUNIT_ASSERT(value.null());
    }
}

void NullableValueTest::test_class(void)
{
    {
        uint32_t counter = 10;
        {
            maxmm::NullableValue<IsAClass> value;
            value.make_value(IsAClass(&counter));
            
            CPPUNIT_ASSERT(value.null() == false);
            CPPUNIT_ASSERT(counter == 11);
        }
        CPPUNIT_ASSERT(counter == 10);
    }
    
    {
        uint32_t counter = 10;
        {
            maxmm::NullableValue<IsAClass> value;
            value.make_value(IsAClass(&counter));
            
            CPPUNIT_ASSERT_EQUAL(value.null(), false);
            CPPUNIT_ASSERT_EQUAL(counter, uint32_t(11));

            value.make_null();
            CPPUNIT_ASSERT_EQUAL(counter, uint32_t(10));
        }
        CPPUNIT_ASSERT_EQUAL(counter, uint32_t(10));
    }
    
    {
        uint32_t counter = 10;
        {
            maxmm::NullableValue<IsAClass> value;
            value.make_value(IsAClass(&counter));
            
            value.make_value(value.value());
            value.make_value(IsAClass(&counter));

            CPPUNIT_ASSERT_EQUAL(value.null(), false);
            CPPUNIT_ASSERT_EQUAL(counter, uint32_t(11));

        }
        CPPUNIT_ASSERT_EQUAL(counter, uint32_t(10));
    }
}


CppUnit::TestSuite *NullableValueTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<NullableValueTest>(
            "NullableValueTest::test_simple",
            &NullableValueTest::test_simple));
      suite->addTest(
        new CppUnit::TestCaller<NullableValueTest>(
            "NullableValueTest::test_class",
            &NullableValueTest::test_class));
  
     return suite; 
}
}
}
