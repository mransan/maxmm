/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_AgentAssignment_h
#define maxmm_ma_AgentAssignment_h

#include <maxmm/ma/AgentId.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>

namespace maxmm
{
namespace ma
{

template<typename VALUE>
class AgentAssignment
{
public:
    explicit AgentAssignment(void);
    explicit AgentAssignment(AgentId const& agent_id, VALUE const& value);

    AgentId const& agent_id() const;
    AgentId      & agent_id();

    VALUE const& value(void) const;
    VALUE      & value(void);
    
    void decode(XmlDecoder const& decoder);
    void encode(XmlEncoder & encoder) const;
private:

    AgentId _agent_id;
    VALUE _value;
};

template<typename VALUE>
AgentAssignment<VALUE>::AgentAssignment(void)
:   _agent_id(AgentId()),
    _value()
{

}

template<typename VALUE>
AgentAssignment<VALUE>::AgentAssignment(
    AgentId const& agent_id,
    VALUE const& value)
:   _agent_id(agent_id),
    _value(value)
{

}

template<typename VALUE>
AgentId const& 
AgentAssignment<VALUE>::agent_id(void) const
{
    return _agent_id;
}

template<typename VALUE>
AgentId &
AgentAssignment<VALUE>::agent_id(void)
{
    return _agent_id;
}


template<typename VALUE>
VALUE const &
AgentAssignment<VALUE>::value(void) const
{
    return _value;
}

template<typename VALUE>
VALUE &
AgentAssignment<VALUE>::value(void)
{
    return _value;
}


template<typename VALUE>
void
AgentAssignment<VALUE>::decode(maxmm::XmlDecoder const& decoder)
{
    decoder.read_element("agent_id", _agent_id);
    decoder.read_element("value", _value);
}

template<typename VALUE>
void
AgentAssignment<VALUE>::encode(maxmm::XmlEncoder & encoder) const
{
    encoder.write_element("agent_id", _agent_id);
    encoder.write_element("value", _value);
}

} // ma namespace 
} // maxmm namespace 

template<typename VALUE>
std::ostream & operator<<(  
    std::ostream &stream, 
    maxmm::ma::AgentAssignment<VALUE> const& aa)
{
    stream << "Agent Assignment:("
           << aa.agent_id()
           << ", value: "
           << aa.value()
           << ")";
    return stream;
}

#endif
