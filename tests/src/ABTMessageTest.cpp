#include <ABTMessageTest.h>

#include <maxmm/ma/ABTMessage.h>

namespace maxmm
{
namespace test
{


void ABTMessageTest::setUp(void)
{

}

void ABTMessageTest::tearDown(void)
{

}

void ABTMessageTest::test_constructor(void)
{
    using namespace maxmm::ma;
    {
        ABTMessage<uint32_t, uint32_t> message; 
        
        ABTMessage<uint32_t, uint32_t>::MessageType expected 
             = ABTMessage<uint32_t, uint32_t>::INVALID;

        CPPUNIT_ASSERT_EQUAL(expected,
                             message._message_type);
    }
}

void ABTMessageTest::test_accessors(void)
{
    using namespace maxmm::ma;
    {
        typedef ABTMessage<uint32_t, uint32_t> Msg;
    
        Msg msg;
        msg.make_ok().agent_assignment().assignment().assign(0, 1);
        CPPUNIT_ASSERT_EQUAL(uint32_t(0), msg.ok().agent_assignment().assignment().variable());
        CPPUNIT_ASSERT_EQUAL(uint32_t(1), msg.ok().agent_assignment().assignment().value());
    }
    
    {
        typedef ABTMessage<uint32_t, uint32_t> Msg;
    
        Msg msg;
        msg.make_nogood().nogoods().push_back(AgentAssignment<uint32_t, uint32_t>());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1),  msg.nogood().nogoods().size());
        
        msg.nogood().nogoods().push_back(AgentAssignment<uint32_t, uint32_t>());
        CPPUNIT_ASSERT_EQUAL(std::size_t(2),  msg.nogood().nogoods().size());

        msg.nogood().nogoods().at(0).assignment().assign(0, 1);
        msg.nogood().nogoods().at(1).assignment().assign(2, 3);

        CPPUNIT_ASSERT_EQUAL(uint32_t(0), msg.nogood().nogoods().at(0).assignment().variable());
        CPPUNIT_ASSERT_EQUAL(uint32_t(1), msg.nogood().nogoods().at(0).assignment().value());
        
        CPPUNIT_ASSERT_EQUAL(uint32_t(2), msg.nogood().nogoods().at(1).assignment().variable());
        CPPUNIT_ASSERT_EQUAL(uint32_t(3), msg.nogood().nogoods().at(1).assignment().value());

    }

}


void ABTMessageTest::test_xml(void)
{
    using namespace maxmm::ma;
    {
        typedef ABTMessage<uint32_t, uint32_t> Msg;
    
        Msg msg;
        msg.make_ok().agent_assignment().assignment().assign(0, 1);
        msg.ok().agent_assignment().agent_id() = AgentId(1);

        xmlpp::Document document;
        document.create_root_node("abt_message");
        XmlEncoder encoder(document);

        msg.encode(encoder);

        std::cout << document.write_to_string() << std::endl;

    }
    
    {
        typedef ABTMessage<uint32_t, uint32_t> Msg;
    
        Msg msg;
        msg.make_nogood().nogoods().push_back(AgentAssignment<uint32_t, uint32_t>());
        
        msg.nogood().nogoods().push_back(AgentAssignment<uint32_t, uint32_t>());

        msg.nogood().nogoods().at(0).assignment().assign(0, 1);
        msg.nogood().nogoods().at(1).assignment().assign(2, 3);
        
        xmlpp::Document document;
        document.create_root_node("abt_message");
        XmlEncoder encoder(document);

        msg.encode(encoder);

        std::cout << document.write_to_string() << std::endl;

    }


}


CppUnit::TestSuite *ABTMessageTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<ABTMessageTest>(
            "ABTMessageTest::test_constructor",
            &ABTMessageTest::test_constructor));
    
    suite->addTest(
        new CppUnit::TestCaller<ABTMessageTest>(
            "ABTMessageTest::test_accessors",
            &ABTMessageTest::test_accessors));
    
    suite->addTest(
        new CppUnit::TestCaller<ABTMessageTest>(
            "ABTMessageTest::test_xml",
            &ABTMessageTest::test_xml));

    return suite;
}

} // namespace test
} // namespace maxmm
