/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTProcessor_h
#define maxmm_ma_ABTProcessor_h

#include <maxmm/ma/ABTMessage.h>
#include <maxmm/ma/ABTOkMessage.h>
#include <maxmm/ma/ABTNoGoodMessage.h>
#include <maxmm/ma/Agent.h>


namespace maxmm { namespace test { class ABTProcessorTest; } }

namespace maxmm
{
namespace ma
{

template<typename VALUE>
class ABTProcessor
{
friend class maxmm::test::ABTProcessorTest;
public:

    typedef ABTMessage<VALUE>               ABTMessage;
    typedef std::pair<ABTMessage, AgentId>  ABTMessageTo;
    typedef std::vector<ABTMessageTo>       ABTMessagesTo;
    typedef ABTOkMessage<VALUE>             ABTOkMessage;
    typedef ABTNoGoodMessage<VALUE>         ABTNoGoodMessage;
    typedef AgentAssignment<VALUE>          AgentAssignment;

    explicit ABTProcessor(
        AgentId const &agent_id,
        VALUE const &current_value);

    
    //! \brief Process a message received from oneof another agent.
    //!
    //! \param abt_message:  the message
    //! \param agent: the agent that this message is comming from.
    //! \return the list of messages that must be sent.
    //!
    ABTMessagesTo process(  
        ABTMessage const &abt_message, 
        AgentId const &agent);

    //! \brief add an agent to the problem definition.
    //!
    //! NOTE: if the agent id is already there it will be ignore.
    //!
    //! \param agent: the agent id of another agent in the problem.
    //!
    void add_agent(AgentId const &agent);
protected:
    struct AgentAssignmentCmp
    {
        bool operator()(AgentAssignment const& lhs,
                        AgentAssignment const& rhs);
    };


    //! \brief method that the subclass must implement for domain specific
    //! algorithms.
    virtual ABTMessagesTo check_agent_view(void) = 0;
    
    //! \brief  the current list of no goods.
    std::vector<std::vector<AgentAssignment> > _nogoods;

    typedef std::set<AgentAssignment, AgentAssignmentCmp> AgentAssignments;
    
    //! \brief  the agent view containing the various assignment.
    AgentAssignments _agent_view;
    
    //! \brief  the current assignment
    VALUE _current_assignment;
    
    std::vector<AgentId> _agents;

    //! \brief  the agent that this class is representing
    Agent _agent; 

private:

    //! \brief process an ok message.
    //! 
    //! \param ok_message: the ok message to process.
    //! \param agent: the agent from which this message comes from.
    //! \return a list of messages.
    //!
    ABTMessagesTo process(
        ABTOkMessage const &ok_message,
        AgentId const &agent);
    
    
    //! \brief process the no good message
    //!
    //! \param nogood_message: the no good message to process.
    //! \param agent: the agent from which this message comes from.
    //! \return list of messages to sent.
    ABTMessagesTo process(
        ABTNoGoodMessage const &nogood_message,
        AgentId const &agent);

};



//
// IMPLEMENTATION
// --------------
//

template<typename VALUE>
ABTProcessor<VALUE>::ABTProcessor(
    AgentId const &agent_id,
    VALUE const &current_assignment)
:   _agent(agent_id),
    _current_assignment(current_assignment)
{

}

template<typename VALUE>
typename ABTProcessor<VALUE>::ABTMessagesTo
ABTProcessor<VALUE>::process(
    ABTMessage const &abt_message,
    AgentId const &agent)
{
    typename ABTMessage::MessageType message_type = abt_message.message_type();
    switch(message_type)
    {
    case ABTMessage::OK_MESSAGE:
        return this->process(abt_message.ok(), agent);
        break;
    case ABTMessage::NOGOOD_MESSAGE:
        return this->process(abt_message.nogood(), agent);
        break;
    case ABTMessage::INVALID:
        throw std::runtime_error("processing");
        break;
    }
}

template<typename VALUE>
typename ABTProcessor<VALUE>::ABTMessagesTo
ABTProcessor<VALUE>::process(
    ABTOkMessage const &ok_message,
    AgentId const &agent)
{
    std::pair<typename AgentAssignments::iterator, bool> ret 
        = _agent_view.insert(ok_message.agent_assignment()); 
    if(false == ret.second)
    {
        _agent_view.erase(ret.first);
        _agent_view.insert(ok_message.agent_assignment());
    }

    return this->check_agent_view();
}

template<typename VALUE>
typename ABTProcessor<VALUE>::ABTMessagesTo
ABTProcessor<VALUE>::process(
    ABTNoGoodMessage const &nogood_message,
    AgentId const &agent)
{
    _nogoods.push_back(nogood_message.nogoods()); 
    return this->check_agent_view();
}


template<typename VALUE>
void
ABTProcessor<VALUE>::add_agent(const AgentId &agent)
{
    _agents.push_back(agent);
    if(_agent.id().id() < agent.id())
    {
        _agent.add_outlink(agent);
    }
}

template<typename VALUE>
bool
ABTProcessor<VALUE>::AgentAssignmentCmp::operator()(
    AgentAssignment const &lhs,
    AgentAssignment const &rhs)
{
    return (lhs.agent_id().id() < rhs.agent_id().id());
}


} // namespace ma
} // namespace maxmm


#endif
