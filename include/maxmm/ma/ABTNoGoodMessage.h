/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTNoGoodMessage_h
#define maxmm_ma_ABTNoGoodMessage_h

#include <maxmm/ma/Assignment.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>

#include <vector>

namespace maxmm
{
namespace ma
{

template<typename VARIABLE, typename VALUE>
class ABTNoGoodMessage 
{

public:
    explicit ABTNoGoodMessage(void);

    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;


    std::vector<Assignment<VARIABLE, VALUE> >       &nogoods(void);
    std::vector<Assignment<VARIABLE, VALUE> > const &nogoods(void) const;

private:

    std::vector<Assignment<VARIABLE, VALUE> > _nogoods;
};


//
// IMPLEMENTATION
// --------------
//

template<typename VARIABLE, typename VALUE>
ABTNoGoodMessage<VARIABLE, VALUE>::ABTNoGoodMessage(void)
{

}

template<typename VARIABLE, typename VALUE>
void ABTNoGoodMessage<VARIABLE, VALUE>::decode(
    const maxmm::XmlDecoder &decoder)
{
    decoder.read_container("assignments", "assignment",  _nogoods);
}


template<typename VARIABLE, typename VALUE>
void ABTNoGoodMessage<VARIABLE, VALUE>::encode(
    maxmm::XmlEncoder &encoder) const
{
    encoder.write_container("assignments", "assignment",  _nogoods);
}


template<typename VARIABLE, typename VALUE>
std::vector<Assignment<VARIABLE, VALUE> > &
ABTNoGoodMessage<VARIABLE, VALUE>::nogoods(void)
{
    return _nogoods;
}

template<typename VARIABLE, typename VALUE>
std::vector<Assignment<VARIABLE, VALUE> >const &
ABTNoGoodMessage<VARIABLE, VALUE>::nogoods(void) const
{
    return _nogoods;
}

}// namespace ma
}// namespace maxmm


#endif
