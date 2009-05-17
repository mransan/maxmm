/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTOkMessage_h
#define maxmm_ma_ABTOkMessage_h

#include <maxmm/ma/AgentId.h>
#include <maxmm/ma/Assignment.h>
#include <maxmm/ma/AgentAssignment.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>


namespace maxmm
{
namespace ma
{

template<typename VARIABLE, typename VALUE>
class ABTOkMessage 
{
public:
    explicit ABTOkMessage(void);

    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;


    AgentAssignment<VARIABLE, VALUE>       &agent_assignment(void);
    AgentAssignment<VARIABLE, VALUE> const &agent_assignment(void) const;

private:

    AgentAssignment<VARIABLE, VALUE> _agent_assignment;
};


//
// IMPLEMENTATION
// --------------
//

template<typename VARIABLE, typename VALUE>
ABTOkMessage<VARIABLE, VALUE>::ABTOkMessage(void)
{

}

template<typename VARIABLE, typename VALUE>
void ABTOkMessage<VARIABLE, VALUE>::decode(
    const maxmm::XmlDecoder &decoder)
{
    decoder.read_element("_agent_assignment", _agent_assignment);
}


template<typename VARIABLE, typename VALUE>
void ABTOkMessage<VARIABLE, VALUE>::encode(
    maxmm::XmlEncoder &encoder) const
{
    encoder.write_element("_agent_assignment", _agent_assignment);
}


template<typename VARIABLE, typename VALUE>
AgentAssignment<VARIABLE, VALUE> &
ABTOkMessage<VARIABLE, VALUE>::agent_assignment(void)
{
    return _agent_assignment;
}

template<typename VARIABLE, typename VALUE>
AgentAssignment<VARIABLE, VALUE> const &
ABTOkMessage<VARIABLE, VALUE>::agent_assignment(void) const
{
    return _agent_assignment;
}

}// namespace ma
}// namespace maxmm


#endif
