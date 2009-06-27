/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTOkMessage_h
#define maxmm_ma_ABTOkMessage_h

#include <maxmm/ma/AgentId.h>
#include <maxmm/ma/AgentAssignment.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>


namespace maxmm
{
namespace ma
{

template<typename VALUE>
class ABTOkMessage 
{
public:
    explicit ABTOkMessage(void);

    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;


    AgentAssignment<VALUE>       &agent_assignment(void);
    AgentAssignment<VALUE> const &agent_assignment(void) const;

private:

    AgentAssignment<VALUE> _agent_assignment;
};


//
// IMPLEMENTATION
// --------------
//

template<typename VALUE>
ABTOkMessage<VALUE>::ABTOkMessage(void)
{

}

template<typename VALUE>
void ABTOkMessage<VALUE>::decode(
    const maxmm::XmlDecoder &decoder)
{
    decoder.read_element("_agent_assignment", _agent_assignment);
}


template<typename VALUE>
void ABTOkMessage<VALUE>::encode(
    maxmm::XmlEncoder &encoder) const
{
    encoder.write_element("_agent_assignment", _agent_assignment);
}


template<typename VALUE>
AgentAssignment<VALUE> &
ABTOkMessage<VALUE>::agent_assignment(void)
{
    return _agent_assignment;
}

template<typename VALUE>
AgentAssignment<VALUE> const &
ABTOkMessage<VALUE>::agent_assignment(void) const
{
    return _agent_assignment;
}

}// namespace ma
}// namespace maxmm


#endif
