/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_AgentAssignment_h
#define maxmm_ma_AgentAssignment_h

#include <maxmm/ma/Assignment.h>
#include <maxmm/ma/AgentId.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>

namespace maxmm
{
namespace ma
{

template<typename VARIABLE, typename VALUE>
class AgentAssignment
{
public:
    explicit AgentAssignment(void);
    explicit AgentAssignment(
        AgentId const& agent_id,
        Assignment<VARIABLE, VALUE> const& assignment);

    AgentId const& agent_id() const;
    AgentId      & agent_id();

    Assignment<VARIABLE, VALUE> const & assignment(void) const;
    Assignment<VARIABLE, VALUE>       & assignment(void);
    
    void decode(XmlDecoder const& decoder);
    void encode(XmlEncoder & encoder) const;
private:

    AgentId _agent_id;
    Assignment<VARIABLE, VALUE> _assignment;
};



template<typename VARIABLE, typename VALUE>
AgentAssignment<VARIABLE, VALUE>::AgentAssignment(void)
:   _agent_id(AgentId()),
    _assignment(Assignment<VARIABLE, VALUE>())
{

}

template<typename VARIABLE, typename VALUE>
AgentAssignment<VARIABLE, VALUE>::AgentAssignment(
    AgentId const& agent_id,
    Assignment<VARIABLE, VALUE> const& assignment)
:   _agent_id(agent_id),
    _assignment(assignment)
{

}

template<typename VARIABLE, typename VALUE>
AgentId const& 
AgentAssignment<VARIABLE, VALUE>::agent_id(void) const
{
    return _agent_id;
}

template<typename VARIABLE, typename VALUE>
AgentId &
AgentAssignment<VARIABLE, VALUE>::agent_id(void)
{
    return _agent_id;
}


template<typename VARIABLE, typename VALUE>
Assignment<VARIABLE, VALUE> const &
AgentAssignment<VARIABLE, VALUE>::assignment(void) const
{
    return _assignment;
}

template<typename VARIABLE, typename VALUE>
Assignment<VARIABLE, VALUE> &
AgentAssignment<VARIABLE, VALUE>::assignment(void)
{
    return _assignment;
}


template<typename VARIABLE, typename VALUE>
void
AgentAssignment<VARIABLE, VALUE>::decode(maxmm::XmlDecoder const& decoder)
{
    decoder.read_element("agent_id", _agent_id);
    decoder.read_element("assignment", _assignment);
}

template<typename VARIABLE, typename VALUE>
void
AgentAssignment<VARIABLE, VALUE>::encode(maxmm::XmlEncoder & encoder) const
{
    encoder.write_element("agent_id", _agent_id);
    encoder.write_element("assignment", _assignment);
}


}
}

#endif
