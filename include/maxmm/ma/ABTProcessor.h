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
    std::pair<bool, ABTMessagesTo> process(  
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
                        AgentAssignment const& rhs) const;
    };


    //! \brief method that the subclass must implement for domain specific
    //! algorithms.
    virtual std::pair<bool, ABTMessagesTo> check_agent_view(void) = 0;
    
    virtual std::pair<bool, ABTMessagesTo> backtrack(void);
    
    typedef std::set<AgentAssignment, AgentAssignmentCmp> AgentAssignments;
    
    //! \brief  the agent view containing the various assignment.
    AgentAssignments _agent_view;
    
    //! \brief  the current list of no goods.
    std::vector<AgentAssignments> _no_goods;

    //! \brief  the current assignment
    VALUE _current_assignment;
    
    std::vector<AgentId> _agents;

    //! \brief  the agent that this class is representing
    Agent _agent; 
    
    
    //! \brief Method to check if the given value is a compatible
    //! current assignment with respect to existing no goods.
    //!
    //! \param value : the value to check against the no good. In 
    //! other word the current assignment candidate.
    //! 
    //! \return true if the given is a viable candidate for the current
    //! assignment.
    //!
    bool check_value_with_no_goods(VALUE const& value) const;
private:

    //! \brief process an ok message.
    //! 
    //! \param ok_message: the ok message to process.
    //! \param agent: the agent from which this message comes from.
    //! \return a list of messages.
    //!
    std::pair<bool, ABTMessagesTo> process(
        ABTOkMessage const &ok_message,
        AgentId const &agent);
    
    
    //! \brief process the no good message
    //!
    //! \param nogood_message: the no good message to process.
    //! \param agent: the agent from which this message comes from.
    //! \return list of messages to sent.
    std::pair<bool, ABTMessagesTo> process(
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
std::pair<bool, typename ABTProcessor<VALUE>::ABTMessagesTo>
ABTProcessor<VALUE>::process(
    ABTMessage const &abt_message,
    AgentId const &agent)
{
    MessageType message_type = abt_message.message_type();
    switch(message_type)
    {
    case OK_MESSAGE:
        return this->process(abt_message.ok(), agent);
        break;
    case NOGOOD_MESSAGE:
        return this->process(abt_message.nogood(), agent);
        break;
    case INVALID:
    {
        std::ostringstream error;
        error << "Processing Error : "
              << "message from " 
              << agent 
              << " is invalid.";
        throw std::runtime_error(error.str());
    }   break;
    }
}

template<typename VALUE>
std::pair<bool, typename ABTProcessor<VALUE>::ABTMessagesTo>
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
std::pair<bool, typename ABTProcessor<VALUE>::ABTMessagesTo>
ABTProcessor<VALUE>::process(
    ABTNoGoodMessage const &nogood_message,
    AgentId const &agent)
{
    AgentAssignments no_goods;
    _no_goods.push_back(no_goods);
    AgentAssignments &no_goods_ref = _no_goods.back();
    for(typename std::vector<AgentAssignment>::const_iterator 
            itr = nogood_message.nogoods().begin();
        itr != nogood_message.nogoods().end();
        ++itr)
    {
        no_goods_ref.insert(*itr);
    }
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
std::pair<bool, typename ABTProcessor<VALUE>::ABTMessagesTo>
ABTProcessor<VALUE>::backtrack(void)
{
    ABTMessagesTo ret;

    if(true == _agent_view.empty())
    {
        // TODO - Maxime log that.
        return std::make_pair<bool, ABTMessagesTo>(false, ret);
    }
    
    // when backtracing you need to send the current agent view 
    // to the agent with the lowest id.
    typename AgentAssignments::const_iterator itr = _agent_view.begin();
    maxmm::ma::AgentId agentto = itr->agent_id();
    for(++itr ; itr!=_agent_view.end() ;++itr)
    {
        // if the agent id is greater than the current one
        // this means that it has a lower priority.
        if(itr->agent_id().id() > agentto.id())
        {
            agentto = itr->agent_id();
        }
    }
    
    if(agentto.id() > _agent.id().id())
    {
        return std::make_pair<bool, ABTMessagesTo>(false, ret);
    }
    
    ret.push_back(
        std::pair<ABTMessage, 
                  maxmm::ma::AgentId>(ABTMessage(), agentto));
    
    ret.front().first.make_nogood();
    
    std::copy(_agent_view.begin(),
              _agent_view.end(),
              std::back_inserter(ret.front().first.nogood().nogoods()));
    // once the agent with the lowest priority has been found.
    // we need to remove it from the agent view assuming it will change
    // its assignment.
    {
        AgentAssignment aa_to_remove;
        aa_to_remove.agent_id() = agentto;
        typename AgentAssignments::iterator itr 
            = _agent_view.find(aa_to_remove);
        // this is really for the sake of error checking since 
        // it does not look possible that the agent later found 
        // is not present in the set.
        if(_agent_view.end() == itr)
        {
            std::ostringstream error;
            error << "Internal error: fail finding agent "
                  << agentto;
            throw std::runtime_error(error.str());
        }
        _agent_view.erase(itr);
    }

    return std::make_pair<bool, ABTMessagesTo>(true, ret);
}

template<typename VALUE>
bool
ABTProcessor<VALUE>::check_value_with_no_goods(VALUE const&value) const
{
    typedef std::vector<AgentAssignments> NoGoods;
    typedef typename NoGoods::const_iterator NoGoodsItr;
    
    // for each no good 
    for(NoGoodsItr no_good = _no_goods.begin();
        no_good != _no_goods.end();
        ++no_good)
    {
        bool valid = false;
        // for each agent involved in the no good. 
        for(typename AgentAssignments::const_iterator aa = no_good->begin();
            aa != no_good->end();
            ++aa)
        {
            AgentId const &agent_id = aa->agent_id();
            VALUE const &no_good_value = aa->value();
            
            //get the agent in the current view.
            typename AgentAssignments::iterator aa_current
                = _agent_view.find(*aa);
            
            if(_agent_view.end() == aa_current)
            {
                // the agent is not in the view.
                valid = true;
                break;
            }

            VALUE const& current_value = aa_current->value();
        
            if(current_value != no_good_value)
            {
                valid = true;
                break;
            }
        }
        if(false == valid)
        {
            return false;
        }
    }
}

template<typename VALUE>
bool
ABTProcessor<VALUE>::AgentAssignmentCmp::operator()(
    AgentAssignment const &lhs,
    AgentAssignment const &rhs) const
{
    return (lhs.agent_id().id() > rhs.agent_id().id());
}

} // namespace ma
} // namespace maxmm


#endif
