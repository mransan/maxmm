#include <ABTNoGoodMessageTest.h>

#include <maxmm/ma/ABTNoGoodMessage.h>

namespace maxmm
{
namespace test
{


void ABTNoGoodMessageTest::setUp(void)
{

}

void ABTNoGoodMessageTest::tearDown(void)
{

}


void ABTNoGoodMessageTest::test_accessors(void)
{
    using namespace maxmm::ma;
    {
        ABTNoGoodMessage<uint32_t, uint32_t> nogood_msg;
        
        nogood_msg.nogoods().push_back(Assignment<uint32_t, uint32_t>(0, 1));
        nogood_msg.nogoods().push_back(Assignment<uint32_t, uint32_t>(2, 3));

        CPPUNIT_ASSERT_EQUAL(uint32_t(0), nogood_msg.nogoods().at(0).variable());
        CPPUNIT_ASSERT_EQUAL(uint32_t(1), nogood_msg.nogoods().at(0).value());
        CPPUNIT_ASSERT_EQUAL(uint32_t(2), nogood_msg.nogoods().at(1).variable());
        CPPUNIT_ASSERT_EQUAL(uint32_t(3), nogood_msg.nogoods().at(1).value());

    }
}

void ABTNoGoodMessageTest::test_xml(void)
{
    using namespace maxmm::ma;
    {
        ABTNoGoodMessage<uint32_t, uint32_t> nogood_msg;
        
        nogood_msg.nogoods().push_back(Assignment<uint32_t, uint32_t>(0, 1));
        nogood_msg.nogoods().push_back(Assignment<uint32_t, uint32_t>(2, 3));
        
        xmlpp::Document document;
        document.create_root_node("nogood_message");
        XmlEncoder encoder(document);

        nogood_msg.encode(encoder);

        std::cout << document.write_to_string() << std::endl;
    }
}



CppUnit::TestSuite *ABTNoGoodMessageTest::getSuite(void)
{
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest(
        new CppUnit::TestCaller<ABTNoGoodMessageTest>(
            "ABTNoGoodMessageTest::test_accessors",
            &ABTNoGoodMessageTest::test_accessors));
    
    suite->addTest(
        new CppUnit::TestCaller<ABTNoGoodMessageTest>(
            "ABTNoGoodMessageTest::test_xml",
            &ABTNoGoodMessageTest::test_xml));

    return suite;
}

} // namespace test
} // namespace maxmm
