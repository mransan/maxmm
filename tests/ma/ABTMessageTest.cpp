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
        ABTMessage<uint32_t> message; 
        
        ma::MessageType expected = ma::INVALID;

        CPPUNIT_ASSERT_EQUAL(expected,
                             message._message_type);
    }
}

void ABTMessageTest::test_accessors(void)
{
    using namespace maxmm::ma;
    {
        typedef ABTMessage<uint32_t> Msg;
    
        Msg msg;
        msg.make_ok();
        msg.ok().agent_assignment().value() = 2;
        msg.ok().agent_assignment().agent_id() = ma::AgentId(1);
        CPPUNIT_ASSERT_EQUAL(uint32_t(2), msg.ok().agent_assignment().value());
        CPPUNIT_ASSERT_EQUAL(uint32_t(1), msg.ok().agent_assignment().agent_id().id());
        
        Msg msg2;
        msg2 = msg;
    
        Msg msg3;
    }
    
    {
        typedef ABTMessage<uint32_t> Msg;
    
        Msg msg;
        msg.make_nogood().nogoods().push_back(AgentAssignment<uint32_t>());
        CPPUNIT_ASSERT_EQUAL(std::size_t(1),  msg.nogood().nogoods().size());
        
        msg.nogood().nogoods().push_back(AgentAssignment<uint32_t>());
        CPPUNIT_ASSERT_EQUAL(std::size_t(2),  msg.nogood().nogoods().size());

        msg.nogood().nogoods().at(0).value() = 2;
        msg.nogood().nogoods().at(1).value() = 3;

        CPPUNIT_ASSERT_EQUAL(uint32_t(2), msg.nogood().nogoods().at(0).value());
        CPPUNIT_ASSERT_EQUAL(uint32_t(3), msg.nogood().nogoods().at(1).value());
    }

}


void ABTMessageTest::test_xml(void)
{
    using namespace maxmm::ma;
    {
        typedef ABTMessage<uint32_t> Msg;
    
        Msg msg;
        msg.make_ok();
        msg.ok().agent_assignment().value() = 2;
        msg.ok().agent_assignment().agent_id() = AgentId(1);

        xmlpp::Document document;
        document.create_root_node("abt_message");
        XmlEncoder encoder(document);

        msg.encode(encoder);

        std::cout << document.write_to_string() << std::endl;

    }
    
    {
        typedef ABTMessage<uint32_t> Msg;
    
        Msg msg;
        msg.make_nogood().nogoods().push_back(AgentAssignment<uint32_t>());
        
        msg.nogood().nogoods().push_back(AgentAssignment<uint32_t>());

        msg.nogood().nogoods().at(0).value() = 2;
        msg.nogood().nogoods().at(1).value() = 3;
        msg.nogood().nogoods().at(0).agent_id() = ma::AgentId(0);
        msg.nogood().nogoods().at(1).agent_id() = ma::AgentId(1);

        
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
