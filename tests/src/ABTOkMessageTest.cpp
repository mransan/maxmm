#include <ABTOkMessageTest.h>

#include <maxmm/ma/ABTOkMessage.h>

namespace maxmm
{
namespace test
{


void ABTOkMessageTest::setUp(void)
{

}

void ABTOkMessageTest::tearDown(void)
{

}


void ABTOkMessageTest::test_accessors(void)
{
    using namespace maxmm::ma;
    {
        ABTOkMessage<uint32_t, uint32_t> ok;
        
        ok.assignment().assign(3, 4); 

        CPPUNIT_ASSERT_EQUAL(uint32_t(3), ok.assignment().value());
        CPPUNIT_ASSERT_EQUAL(uint32_t(4), ok.assignment().variable());
    }
}

void ABTOkMessageTest::test_xml(void)
{
    using namespace maxmm::ma;
    {
        ABTOkMessage<uint32_t, uint32_t> ok;
        
        ok.assignment().assign(3, 4);
    
        xmlpp::Document document;
        document.create_root_node("ok_message");
        XmlEncoder encoder(document);

        ok.encode(encoder);

        std::cout << document.write_to_string() << std::endl;
    
    }

}


CppUnit::TestSuite *ABTOkMessageTest::getSuite(void)
{
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest(
        new CppUnit::TestCaller<ABTOkMessageTest>(
            "ABTOkMessageTest::test_accessors",
            &ABTOkMessageTest::test_accessors));
    
    suite->addTest(
        new CppUnit::TestCaller<ABTOkMessageTest>(
            "ABTOkMessageTest::test_xml",
            &ABTOkMessageTest::test_xml));

    return suite;
}

} // namespace test
} // namespace maxmm
