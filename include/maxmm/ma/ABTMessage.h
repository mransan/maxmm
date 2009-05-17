/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_ma_ABTMessage_h
#define maxmm_ma_ABTMessage_h

#include <maxmm/ma/ABTOkMessage.h>
#include <maxmm/ma/ABTNoGoodMessage.h>
#include <maxmm/StackBuffer.h>
#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>


namespace maxmm{ namespace test{ class ABTMessageTest; } }


namespace maxmm
{
namespace ma
{

template<typename VARIABLE, typename VALUE>
class ABTMessage 
{
friend class maxmm::test::ABTMessageTest;

public:
    explicit ABTMessage(void);
        
    
    enum MessageType
    {
        OK_MESSAGE      = 0,
        NOGOOD_MESSAGE = 1,
        INVALID
    };
    
    MessageType message_type(void) const;

    

    ABTOkMessage<VARIABLE, VALUE>      & make_ok(void);
    ABTOkMessage<VARIABLE, VALUE>      & ok(void);
    ABTOkMessage<VARIABLE, VALUE> const& ok(void) const;

    
    ABTNoGoodMessage<VARIABLE, VALUE>      & make_nogood(void);
    ABTNoGoodMessage<VARIABLE, VALUE>      & nogood(void);
    ABTNoGoodMessage<VARIABLE, VALUE> const& nogood(void) const;

    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;

private:
    
    union MessageChoice
    {
        StackBuffer<ABTOkMessage<VARIABLE, VALUE> > _ok_message;
        StackBuffer<ABTNoGoodMessage<VARIABLE, VALUE> >_nogood_message;
    };


    MessageChoice _message;
    MessageType _message_type;

    StackBufferAccessor<ABTOkMessage<VARIABLE, VALUE> > _ok_accessor;
    StackBufferAccessor<ABTNoGoodMessage<VARIABLE, VALUE> > _nogood_accessor;
};


template<typename VARIABLE, typename VALUE>
ABTMessage<VARIABLE, VALUE>::ABTMessage(void)
:   _message_type(INVALID),
    _ok_accessor(_message._ok_message),
    _nogood_accessor(_message._nogood_message)
{

}

template<typename VARIABLE, typename VALUE>
typename ABTMessage<VARIABLE, VALUE>::MessageType 
ABTMessage<VARIABLE, VALUE>::message_type(void) const
{
    return _message_type;
}

template<typename VARIABLE, typename VALUE>
ABTOkMessage<VARIABLE, VALUE>& 
ABTMessage<VARIABLE, VALUE>::make_ok(void)
{
    _message_type = OK_MESSAGE;
    return *_ok_accessor.make();
}

template<typename VARIABLE, typename VALUE>
ABTOkMessage<VARIABLE, VALUE>&
ABTMessage<VARIABLE, VALUE>::ok(void)
{
    return *_ok_accessor.get();
}

template<typename VARIABLE, typename VALUE>
ABTOkMessage<VARIABLE, VALUE> const& 
ABTMessage<VARIABLE, VALUE>::ok(void) const
{
    return *_ok_accessor.get();
}


template<typename VARIABLE, typename VALUE>
ABTNoGoodMessage<VARIABLE, VALUE> & 
ABTMessage<VARIABLE, VALUE>::make_nogood(void)
{
    _message_type = NOGOOD_MESSAGE;
    return *_nogood_accessor.make();
}


template<typename VARIABLE, typename VALUE>
ABTNoGoodMessage<VARIABLE, VALUE>& 
ABTMessage<VARIABLE, VALUE>::nogood(void)
{
    return *_nogood_accessor.get();
}

template<typename VARIABLE, typename VALUE>
ABTNoGoodMessage<VARIABLE, VALUE> const& 
ABTMessage<VARIABLE, VALUE>::nogood(void) const
{
    return *_nogood_accessor.get();
}

template<typename VARIABLE, typename VALUE>
void 
ABTMessage<VARIABLE, VALUE>::decode(const maxmm::XmlDecoder &decoder)
{
    decoder.read_element("message_type", _message_type);
    switch(_message_type)
    {
        case INVALID:
            return;
            break;
        case OK_MESSAGE:
            decoder.read_element("ok_message", *_ok_accessor.make());
            break;
        case NOGOOD_MESSAGE:
            decoder.read_element("nogood_message", *_nogood_accessor.make());
            break;
    }
}


template<typename VARIABLE, typename VALUE>
void 
ABTMessage<VARIABLE, VALUE>::encode(maxmm::XmlEncoder &encoder) const
{
    encoder.write_element("message_type", static_cast<uint32_t>(_message_type));
    switch(_message_type)
    {
        case INVALID:
            return ;
            break;
        case OK_MESSAGE:
            encoder.write_element("ok_message", this->ok());
            break;
        case NOGOOD_MESSAGE:    
            encoder.write_element("nogood_message", this->nogood());
            break;
    }   
}



} // namespace ma
} // namespace maxmm

#endif
