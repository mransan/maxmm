#include <AssignmentTest.h>

#include <maxmm/ma/Assignment.h>

namespace maxmm
{
namespace test
{

void AssignmentTest::setUp(void)
{

}

void AssignmentTest::tearDown(void)
{

}

void AssignmentTest::test_constructor(void)
{
    using namespace maxmm::ma;
    {
        Assignment<uint32_t, uint32_t> assignment(0, 0);

        CPPUNIT_ASSERT_EQUAL(uint32_t(0), assignment._variable);
        CPPUNIT_ASSERT_EQUAL(uint32_t(0), assignment._value);
    }
}

void AssignmentTest::test_accessors(void)
{
    using namespace maxmm::ma;
    {
        Assignment<uint32_t, uint32_t> assignment(0, 0);

        CPPUNIT_ASSERT_EQUAL(uint32_t(0), assignment.variable());
        CPPUNIT_ASSERT_EQUAL(uint32_t(0), assignment.value());
    }
    {
        Assignment<uint32_t, uint32_t> assignment;
        assignment.assign(3, 4); 

        CPPUNIT_ASSERT_EQUAL(uint32_t(3), assignment._variable);
        CPPUNIT_ASSERT_EQUAL(uint32_t(4), assignment._value);
    }
}

void AssignmentTest::test_xml(void)
{
    using namespace maxmm::ma;

    {
        Assignment<uint32_t, uint32_t> assignment(0, 0);
        
        xmlpp::Document document;
        {
            document.create_root_node("root");
        }
        
        XmlEncoder encoder(document);
        assignment.encode(encoder);
        std::cout << document.write_to_string() << std::endl;
    }
    
}


CppUnit::TestSuite *AssignmentTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<AssignmentTest>(
            "AssignmentTest::test_constructor",
            &AssignmentTest::test_constructor));
    
    suite->addTest(
        new CppUnit::TestCaller<AssignmentTest>(
            "AssignmentTest::test_accessors",
            &AssignmentTest::test_accessors));
    
    suite->addTest(
        new CppUnit::TestCaller<AssignmentTest>(
            "AssignmentTest::test_xml",
            &AssignmentTest::test_xml));

    return suite;
}

} // namespace test
} // namespace maxmm
