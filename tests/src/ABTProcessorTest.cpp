#include <ABTProcessorTest.h>

#include <maxmm/ma/ABTProcessor.h>
#include <vector>
namespace maxmm
{
namespace test
{

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
        ma::AgentId const &agent_id,
        Color current_value,
        std::vector<Color> const& allowed);

protected:
    ABTMessagesTo check_agent_view(void);

private:
    ABTMessagesTo backtrack(void);
    std::vector<Color> _allowed;
};


SimpleABT::SimpleABT(
    ma::AgentId const &agent_id,
    Color current_assignment,
    std::vector<Color> const& allowed)
:   ma::ABTProcessor<Color>(agent_id, current_assignment)
   ,_allowed(allowed)
{
    
}

SimpleABT::ABTMessagesTo
SimpleABT::backtrack(void)
{
    ABTMessagesTo ret;

    if(true == _agent_view.empty())
    {
        // TODO - Maxime log that.
        return ret;
    }
    
    // when backtracing you need to send the current agent view 
    // to the agent with the lowest id.
    AgentAssignments::const_iterator itr = _agent_view.begin();
    ma::AgentId agentto = itr->agent_id();
    for(++itr ; itr!=_agent_view.end() ;++itr)
    {
        if(itr->agent_id().id() < agentto.id())
        {
            agentto = itr->agent_id();
        }
    }
    ret.push_back(std::pair<ABTMessage, ma::AgentId>(ABTMessage(), agentto));
    
    ret.front().first.make_nogood();
    
    std::copy(_agent_view.begin(),
              _agent_view.end(),
              std::back_inserter(ret.front().first.nogood().nogoods()));
    return ret;
}

SimpleABT::ABTMessagesTo
SimpleABT::check_agent_view(void)
{
    // hold all of the already assigned value
    std::vector<Color> nopossible;

    for(AgentAssignments::const_iterator itr=_agent_view.begin();
        itr != _agent_view.end();
        ++itr)
    {
        Color assignment = itr->value();
        nopossible.push_back(assignment);
    }
    
    ABTMessagesTo ret;
    Color myvalue = _current_assignment; 
    if(nopossible.end() == std::find(nopossible.begin(), nopossible.end(), myvalue))
    {
        // already good value.
        return ret;
    }

    // look for one value that we can assign to.
    Color v = INVALID;
    for(std::vector<Color>::const_iterator itr = _allowed.begin();
        itr != _allowed.end();
        ++itr)
    {
        if(nopossible.end() == 
                std::find(nopossible.begin(), nopossible.end(), *itr))
        {
            v = *itr;
        }
    }
    
    // no value was found backtrack.
    if(v == INVALID)
    {
        return this->backtrack();
    }
    
    // record the newly found valud as being the current one.
    _current_assignment = v;
    
    // prepare all the OK message to sent to all the out link agents.
    std::vector<ma::AgentId> to = _agent.outlinks_to();
    for(std::vector<ma::AgentId>::const_iterator itr = to.begin();
        itr != to.end();
        ++itr)
    {
        ma::ABTMessage<Color> msg;
        msg.make_ok().agent_assignment().agent_id() = _agent.id();
        msg.ok().agent_assignment().value() = _current_assignment;

        ret.push_back(std::make_pair(msg, *itr));
    }

    return ret;
}

} // unnamed namespace


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
    SimpleABT::ABTMessage  msgfrom1;
    {
        msgfrom1.make_ok();
        msgfrom1.ok().agent_assignment().agent_id() = ma::AgentId(1);
        msgfrom1.ok().agent_assignment().value() = RED;
    }
    
    // simulate the receiving of a message.
    SimpleABT::ABTMessagesTo msgs = processor.process(msgfrom1, ma::AgentId(1));
    {
        // making sure that the internal agent view structure is correct.
        CPPUNIT_ASSERT_EQUAL(size_t(1), processor._agent_view.size());
        SimpleABT::AgentAssignment const& aa = *(processor._agent_view.begin()); 
        CPPUNIT_ASSERT_EQUAL(ma::AgentId(1), aa.agent_id());
        CPPUNIT_ASSERT_EQUAL(RED, aa.value());
    }
    
    bool isempty = msgs.empty();
    CPPUNIT_ASSERT(isempty);
    {
        msgfrom1.ok().agent_assignment().value() = BLUE;
    }

    msgs = processor.process(msgfrom1, ma::AgentId(1));
    
    // make sure that there is one message and that it is an OK one.
    CPPUNIT_ASSERT_EQUAL(size_t(1), msgs.size());
    CPPUNIT_ASSERT_EQUAL(
        SimpleABT::ABTMessage::OK_MESSAGE, 
        msgs.at(0).first.message_type());
    

    // make sure that the agent the message should be sent is the agent with
    // lower ID.
    CPPUNIT_ASSERT_EQUAL(ma::AgentId(3), msgs.at(0).second);
    
    // make sure it is a valid assignment.
    SimpleABT::AgentAssignment const& aa = msgs.at(0).first.ok().agent_assignment();
    CPPUNIT_ASSERT_EQUAL(ma::AgentId(2), aa.agent_id());
    CPPUNIT_ASSERT(BLUE != aa.value());
    CPPUNIT_ASSERT_EQUAL(RED, aa.value());
}

CppUnit::TestSuite *ABTProcessorTest::getSuite(void)
{
     CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
     suite->addTest(
        new CppUnit::TestCaller<ABTProcessorTest>(
            "ABTProcessorTest::test_simple",
            &ABTProcessorTest::test_simple));
    
    return suite;
}


}
}
