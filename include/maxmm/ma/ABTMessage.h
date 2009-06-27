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

template<typename VALUE>
class ABTMessage 
{
friend class maxmm::test::ABTMessageTest;

public:
    explicit ABTMessage(void);
    
    ABTMessage(ABTMessage const & msg);
    ABTMessage & operator=(ABTMessage const &msg);
    
    ~ABTMessage(void);

    enum MessageType
    {
        OK_MESSAGE     = 0,
        NOGOOD_MESSAGE = 1,
        INVALID
    };
    
    MessageType message_type(void) const;

    ABTOkMessage<VALUE>      & make_ok(void);
    ABTOkMessage<VALUE>      & ok(void);
    ABTOkMessage<VALUE> const& ok(void) const;

    
    ABTNoGoodMessage<VALUE>      & make_nogood(void);
    ABTNoGoodMessage<VALUE>      & nogood(void);
    ABTNoGoodMessage<VALUE> const& nogood(void) const;

    void decode(const maxmm::XmlDecoder &decoder);
    void encode(maxmm::XmlEncoder &encoder) const;

private:
    
    union MessageChoice
    {
        StackBuffer<ABTOkMessage<VALUE> > _ok_message;
        StackBuffer<ABTNoGoodMessage<VALUE> >_nogood_message;
    };

    void reset(void); 

    MessageChoice _message;
    MessageType _message_type;

    StackBufferAccessor<ABTOkMessage<VALUE> > _ok_accessor;
    StackBufferAccessor<ABTNoGoodMessage<VALUE> > _nogood_accessor;
};


template<typename VALUE>
ABTMessage<VALUE>::ABTMessage(void)
:   _message_type(INVALID),
    _ok_accessor(&_message._ok_message),
    _nogood_accessor(&_message._nogood_message)
{

}

template<typename VALUE>
ABTMessage<VALUE>::ABTMessage(ABTMessage<VALUE> const& msg)
:   _message_type(msg._message_type),
    _ok_accessor(&_message._ok_message),
    _nogood_accessor(&_message._nogood_message)
{
    switch(msg._message_type)
    {
        case INVALID:
            return;
            break;
        case OK_MESSAGE:
            _ok_accessor.make(msg.ok());
            break;
        case NOGOOD_MESSAGE:
            _nogood_accessor.make(msg.nogood());
            break;
    }
}

template<typename VALUE>
ABTMessage<VALUE> &
ABTMessage<VALUE>::operator=(ABTMessage<VALUE> const& msg)
{
    this->reset();
    switch(msg._message_type)
    {
        case INVALID:
            return *this;
            break;
        case OK_MESSAGE:
            this->make_ok();
            _ok_accessor.get()->operator=(msg.ok());
            break;
        case NOGOOD_MESSAGE:
            this->make_nogood();
            _nogood_accessor.get()->operator=(msg.nogood());
            break;
    }
    return *this;
}

template<typename VALUE>
ABTMessage<VALUE>::~ABTMessage(void)
{
    switch(_message_type)
    {
        case INVALID:
            return;
            break;
        case OK_MESSAGE: 
            _ok_accessor.get()->~ABTOkMessage<VALUE>();
            break;
        case NOGOOD_MESSAGE:
            _nogood_accessor.get()->~ABTNoGoodMessage<VALUE>();
            break;
    }
}

template<typename VALUE>
typename ABTMessage<VALUE>::MessageType 
ABTMessage<VALUE>::message_type(void) const
{
    return _message_type;
}

template<typename VALUE>
ABTOkMessage<VALUE>& 
ABTMessage<VALUE>::make_ok(void)
{
    _message_type = OK_MESSAGE;
    return *_ok_accessor.make();
}

template<typename VALUE>
ABTOkMessage<VALUE>&
ABTMessage<VALUE>::ok(void)
{
    return *_ok_accessor.get();
}

template<typename VALUE>
ABTOkMessage<VALUE> const& 
ABTMessage<VALUE>::ok(void) const
{
    return *_ok_accessor.get();
}


template<typename VALUE>
ABTNoGoodMessage<VALUE> & 
ABTMessage<VALUE>::make_nogood(void)
{
    _message_type = NOGOOD_MESSAGE;
    return *_nogood_accessor.make();
}


template<typename VALUE>
ABTNoGoodMessage<VALUE>& 
ABTMessage<VALUE>::nogood(void)
{
    return *_nogood_accessor.get();
}

template<typename VALUE>
ABTNoGoodMessage<VALUE> const& 
ABTMessage<VALUE>::nogood(void) const
{
    return *_nogood_accessor.get();
}

template<typename VALUE>
void
ABTMessage<VALUE>::reset(void)
{
    switch(_message_type)
    {
        case INVALID:
            return;
            break;
        case OK_MESSAGE: 
            _ok_accessor.get()->~ABTOkMessage<VALUE>();
            break;
        case NOGOOD_MESSAGE:
            _nogood_accessor.get()->~ABTNoGoodMessage<VALUE>();
            break;
    }
    _message_type = INVALID;
}

template<typename VALUE>
void 
ABTMessage<VALUE>::decode(const maxmm::XmlDecoder &decoder)
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


template<typename VALUE>
void 
ABTMessage<VALUE>::encode(maxmm::XmlEncoder &encoder) const
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
