/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTOkMessage_h
#define maxmm_ma_ABTOkMessage_h

#include <maxmm/ma/Assignment.h>
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


    Assignment<VARIABLE, VALUE>       &assignment(void);
    Assignment<VARIABLE, VALUE> const &assignment(void) const;

private:

    Assignment<VARIABLE, VALUE> _assignment;
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
    decoder.read_element("assignment", _assignment);
}


template<typename VARIABLE, typename VALUE>
void ABTOkMessage<VARIABLE, VALUE>::encode(
    maxmm::XmlEncoder &encoder) const
{
    encoder.write_element("assignment", _assignment);
}


template<typename VARIABLE, typename VALUE>
Assignment<VARIABLE, VALUE> &ABTOkMessage<VARIABLE, VALUE>::assignment(void)
{
    return _assignment;
}

template<typename VARIABLE, typename VALUE>
Assignment<VARIABLE, VALUE> const &
ABTOkMessage<VARIABLE, VALUE>::assignment(void) const
{
    return _assignment;
}

}// namespace ma
}// namespace maxmm


#endif
