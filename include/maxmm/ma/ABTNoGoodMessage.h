/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTNoGoodMessage_h
#define maxmm_ma_ABTNoGoodMessage_h

#include <maxmm/ma/AgentId.h>
#include <maxmm/ma/AgentAssignment.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>

#include <vector>

namespace maxmm
{
namespace ma
{

template<typename VALUE>
class ABTNoGoodMessage 
{

public:
    explicit ABTNoGoodMessage(void);

    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;

    
    std::vector<AgentAssignment<VALUE> >       & nogoods(void);
    std::vector<AgentAssignment<VALUE> > const & nogoods(void) const;

private:

    std::vector<AgentAssignment<VALUE> > _nogoods;
};


//
// IMPLEMENTATION
// --------------
//

template<typename VALUE>
ABTNoGoodMessage<VALUE>::ABTNoGoodMessage(void)
{

}

template<typename VALUE>
void ABTNoGoodMessage<VALUE>::decode(
    const maxmm::XmlDecoder &decoder)
{
    decoder.read_container("assignments", "assignment",  _nogoods);
}


template<typename VALUE>
void ABTNoGoodMessage<VALUE>::encode(
    maxmm::XmlEncoder &encoder) const
{
    encoder.write_container("assignments", "assignment",  _nogoods);
}


template<typename VALUE>
std::vector<AgentAssignment<VALUE> > & 
ABTNoGoodMessage<VALUE>::nogoods(void)
{
    return _nogoods;
}

template<typename VALUE>
std::vector<AgentAssignment<VALUE> >const &
ABTNoGoodMessage<VALUE>::nogoods(void) const
{
    return _nogoods;
}

}// namespace ma
}// namespace maxmm


#endif
