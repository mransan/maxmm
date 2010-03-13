#include <ABTProcessorTest.h>

#include <maxmm/ma/ABTProcessor.h>
#include <vector>
namespace
{


enum Color
{
    BLUE   = 0,
    GREEN  = 1, 
    RED    = 2,
    INVALID,
};

class SimpleABT : public maxmm::ma::ABTProcessor<Color>
{
public:
    SimpleABT(
        maxmm::ma::AgentId const &agent_id,
        Color current_value,
        std::vector<Color> const& allowed);

protected:
    std::pair<bool, ABTMsgsTo> check_agent_view(void);

private:
    std::vector<Color> _allowed;
};


SimpleABT::SimpleABT(
    maxmm::ma::AgentId const &agent_id,
    Color current_assignment,
    std::vector<Color> const& allowed)
:   maxmm::ma::ABTProcessor<Color>(agent_id, current_assignment)
   ,_allowed(allowed)
{
    
}

std::pair<bool, SimpleABT::ABTMsgsTo>
SimpleABT::check_agent_view(void)
{
    // hold all of the already assigned value
    std::vector<Color> nopossible;

    for(AgtAssignments::const_iterator itr=_agent_view.begin();
        itr != _agent_view.end();
        ++itr)
    {
        Color assignment = itr->value();
        nopossible.push_back(assignment);
    }
    
    ABTMsgsTo ret;
    Color myvalue = _current_assignment; 
    if(nopossible.end() 
            == std::find(nopossible.begin(), nopossible.end(), myvalue))
    {
        // already good value.
        return std::make_pair<bool, ABTMsgsTo>(true, ret);
    }

    // look for all the values that we can assign to.
    std::vector<Color> vs;
    for(std::vector<Color>::const_iterator itr = _allowed.begin();
        itr != _allowed.end();
        ++itr)
    {
        if(nopossible.end() == 
                std::find(nopossible.begin(), nopossible.end(), *itr))
        {
            vs.push_back(*itr);
        }
    }
    
    // no value was found backtrack.
    if(vs.empty())
    {
        return this->backtrack();
    }
    
    Color valid_color = INVALID;
    // need to check for the no goods!
    for(std::vector<Color>::const_iterator itr = vs.begin();
        itr != vs.end();
        ++itr)
    {
        if(true == this->check_value_with_no_goods(*itr))
        {
            valid_color = *itr;
        }
    }
    
    if(valid_color == INVALID)
    {
        return this->backtrack();
    }

    // record the newly found valud as being the current one.
    _current_assignment = valid_color;
    
    // prepare all the OK message to sent to all the out link agents.
    std::vector<maxmm::ma::AgentId> to = _agent.outlinks_to();
    for(std::vector<maxmm::ma::AgentId>::const_iterator itr = to.begin();
        itr != to.end();
        ++itr)
    {
        maxmm::ma::ABTMessage<Color> msg;
        msg.make_ok().agent_assignment().agent_id() = _agent.id();
        msg.ok().agent_assignment().value() = _current_assignment;

        ret.push_back(std::make_pair(msg, *itr));
    }

    return std::make_pair<bool, ABTMsgsTo>(true, ret);
}
} // unnamed namespace

MAKE_TYPE_XML_PRIMITIVE(Color);

std::ostream & operator<<(std::ostream &stream, Color color)
{
    switch(color)
    {
        case BLUE:
            stream << "BLUE";
            break;
        case GREEN:
            stream << "GREEN";
            break;
        case RED:
            stream << "RED";
            break;
        case INVALID:
            stream << "INVALID";
            break;
    }
    return stream;
}

std::istream & operator>>(std::istream &stream, Color &color)
{
    std::string str;
    stream >> str;
    if(true == str.empty())
    {
        color = INVALID;
        return stream;
    }
    char first = str[0];
    switch(first)
    {
        case 'B':
            color = BLUE;
            break;
        case 'G':
            color = GREEN;
            break;
        case 'R':
            color = RED;
            break;
        default:
            color = INVALID;
            break;
    }
    
    return stream;
}

namespace maxmm
{
namespace test
{


void 
ABTProcessorTest::setUp(void)
{

}

void 
ABTProcessorTest::tearDown(void)
{

}


void
ABTProcessorTest::test_simple(void)
{
    // just make sure serialization and deserialization is corret.
    {
        Color color = RED;
        {
            std::ostringstream stream;
            stream << color;
            CPPUNIT_ASSERT_EQUAL(std::string("RED"), stream.str());
        }
        {
            std::istringstream stream("BLUE");
            stream >> color;
            CPPUNIT_ASSERT_EQUAL(BLUE, color);
        }
    }
    
    std::vector<Color> allowed;
    {
        allowed.push_back(BLUE);
        allowed.push_back(RED);
    }
    SimpleABT processor(ma::AgentId(2), BLUE, allowed);
    {
        processor.add_agent(ma::AgentId(1));
        processor.add_agent(ma::AgentId(3));
    }

    CPPUNIT_ASSERT_EQUAL(size_t(2), processor._agents.size());
    CPPUNIT_ASSERT_EQUAL(BLUE , processor._current_assignment);
    CPPUNIT_ASSERT_EQUAL(ma::AgentId(2), processor._agent.id());
    CPPUNIT_ASSERT_EQUAL(size_t(1), processor._agent.outlinks_to().size());
   
    // message to be sent
    SimpleABT::ABTMsg  msgfrom1;
    {
        msgfrom1.make_ok();
        msgfrom1.ok().agent_assignment().agent_id() = ma::AgentId(1);
        msgfrom1.ok().agent_assignment().value() = RED;
    }
    
    // simulate the receiving of a message.
    std::pair<bool,SimpleABT::ABTMsgsTo> ret  
        = processor.process(msgfrom1, ma::AgentId(1));
    {
        // making sure that the internal agent view structure is correct.
        CPPUNIT_ASSERT_EQUAL(size_t(1), processor._agent_view.size());
        SimpleABT::AgtAssignment const& aa = *(processor._agent_view.begin());
        CPPUNIT_ASSERT_EQUAL(ma::AgentId(1), aa.agent_id());
        CPPUNIT_ASSERT_EQUAL(RED, aa.value());
    }
    {
        CPPUNIT_ASSERT_EQUAL(true, ret.first);
    }
    
    SimpleABT::ABTMsgsTo &msgs = ret.second;
    bool isempty = msgs.empty();
    CPPUNIT_ASSERT(isempty);
    {
        msgfrom1.ok().agent_assignment().value() = BLUE;
    }

    ret  = processor.process(msgfrom1, ma::AgentId(1));
    msgs = ret.second; 
    // make sure that there is one message and that it is an OK one.
    CPPUNIT_ASSERT(ret.first);
    CPPUNIT_ASSERT_EQUAL(size_t(1), msgs.size());
    CPPUNIT_ASSERT_EQUAL(
        ma::OK_MESSAGE, 
        msgs.at(0).first.message_type());
    

    // make sure that the agent the message should be sent is the agent with
    // lower ID.
    CPPUNIT_ASSERT_EQUAL(ma::AgentId(3), msgs.at(0).second);
    
    // make sure it is a valid assignment.
    SimpleABT::AgtAssignment const& aa = msgs.at(0).first.ok().agent_assignment();
    CPPUNIT_ASSERT_EQUAL(ma::AgentId(2), aa.agent_id());
    CPPUNIT_ASSERT(BLUE != aa.value());
    CPPUNIT_ASSERT_EQUAL(RED, aa.value());
}

void
ABTProcessorTest::test_backtrack(void)
{
    std::vector<Color> allowed;
    {
        allowed.push_back(BLUE);
        allowed.push_back(RED);
    }
    SimpleABT processor(ma::AgentId(3), BLUE, allowed);
    {
        processor.add_agent(ma::AgentId(1));
        processor.add_agent(ma::AgentId(2));
    }
    
    // first message from agent 1 indicating that its value BLUE.
    SimpleABT::ABTMsg msg_from1;
    {
        msg_from1.make_ok();
        msg_from1.ok().agent_assignment().agent_id() = ma::AgentId(1);
        msg_from1.ok().agent_assignment().value() = BLUE;
    }
    std::pair<bool, SimpleABT::ABTMsgsTo> ret 
        = processor.process(msg_from1, ma::AgentId(1));
    SimpleABT::ABTMsgsTo &msgs1  = ret.second;
    // verification of the processing of the new msg. 
    {
        // make sure the agent has correctly updated its value;
        CPPUNIT_ASSERT_EQUAL(RED, processor._current_assignment);
        
        // make sure the agent has correctly updated its agent view.
        CPPUNIT_ASSERT_EQUAL(size_t(1), processor._agent_view.size());
        SimpleABT::AgtAssignment const& aa = *processor._agent_view.begin();
        CPPUNIT_ASSERT_EQUAL(ma::AgentId(1), aa.agent_id());
        CPPUNIT_ASSERT_EQUAL(BLUE, aa.value());

        CPPUNIT_ASSERT_EQUAL(size_t(0), msgs1.size());
        CPPUNIT_ASSERT_EQUAL(true, ret.first);
    }
    
    // second message from agent 2 indicating that it is BLUE as well.
    SimpleABT::ABTMsg msg_from2;
    {
        msg_from2.make_ok();
        msg_from2.ok().agent_assignment().agent_id() = ma::AgentId(2);
        msg_from2.ok().agent_assignment().value() = BLUE;
    }
    ret  = processor.process(msg_from2, ma::AgentId(2));
    SimpleABT::ABTMsgsTo &msgs2 = ret.second; 
    {
        // make sure that the internal value is RED which does not conflict
        // with other value.
        CPPUNIT_ASSERT_EQUAL(RED, processor._current_assignment);

        // make sure that the agent view has the expected new agent in it.
        CPPUNIT_ASSERT_EQUAL(size_t(2), processor._agent_view.size());
        SimpleABT::AgtAssignment const& aa = *processor._agent_view.begin();
        CPPUNIT_ASSERT_EQUAL(ma::AgentId(2), aa.agent_id());
        
        // make sure that there is no message being sent.
        CPPUNIT_ASSERT_EQUAL(size_t(0), msgs2.size()); 
        CPPUNIT_ASSERT_EQUAL(true, ret.first);
    }
    
    // third message from agent 2 indicating that it is RED now.
    SimpleABT::ABTMsg msg2_from2;
    {
        msg2_from2.make_ok();
        msg2_from2.ok().agent_assignment().agent_id() = ma::AgentId(2);
        msg2_from2.ok().agent_assignment().value() = RED;
    }

    ret  = processor.process(msg2_from2, ma::AgentId(2));
    SimpleABT::ABTMsgsTo &msg3 = ret.second;
    {
        // make sure that there is one message sent...
        CPPUNIT_ASSERT_EQUAL(size_t(1), msg3.size());
        CPPUNIT_ASSERT_EQUAL(true, ret.first);
        SimpleABT::ABTMsgTo const& msgto = msg3.at(0);
        // ... to agent with the lowest priority (2)
        CPPUNIT_ASSERT_EQUAL(ma::AgentId(2), msgto.second);
        SimpleABT::ABTMsg const& msg = msgto.first;
        // ... and that it is a no good.
        CPPUNIT_ASSERT_EQUAL(ma::NOGOOD_MESSAGE, msg.message_type());
        
        // make sure that the current value/assignment was not changed.
        CPPUNIT_ASSERT_EQUAL(RED, processor._current_assignment);
        
        XmlEncoder::to_stream(std::cout, msg, "message_debug");
        std::cout << std::endl;
       
        // make sure that the agent 2 assignment was removed from the
        // the current agent view.
        CPPUNIT_ASSERT_EQUAL(size_t(1), processor._agent_view.size());
        CPPUNIT_ASSERT_EQUAL(ma::AgentId(1), processor._agent_view.begin()->agent_id());
    }
}

CppUnit::TestSuite *ABTProcessorTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<ABTProcessorTest>(
            "ABTProcessorTest::test_simple",
            &ABTProcessorTest::test_simple));
    suite->addTest(
        new CppUnit::TestCaller<ABTProcessorTest>(
            "ABTProcessorTest::test_backtrack",
            &ABTProcessorTest::test_backtrack));

    return suite;
}


} // test
} // maxmm
