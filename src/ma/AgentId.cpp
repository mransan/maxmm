/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#include <maxmm/XmlEncoder.h>
#include <maxmm/XmlDecoder.h>

#include <maxmm/ma/AgentId.h>

namespace maxmm
{

namespace ma
{

AgentId::AgentId(void)
:   _id(AgentId::INVALID_ID())
{

}

AgentId::AgentId(uint32_t id)
:   _id(id)
{

}

uint32_t AgentId::id(void) const
{
    return _id;
}

bool AgentId::valid(void) const
{
    return _id != AgentId::INVALID_ID();
}

void AgentId::encode(XmlEncoder &encoder) const
{
    if(false == this->valid())
    {
        return;
    }
    encoder.write_element("id", _id);
}

void AgentId::decode(const XmlDecoder &decoder) 
{
    decoder.read_element("id", _id);
}

uint32_t AgentId::INVALID_ID() 
{
    static uint32_t INVALID_ID = static_cast<uint32_t>(-1);
    return INVALID_ID;
}

} // namespace ma
} // namespace maxmm

bool operator==(const maxmm::ma::AgentId& lhs, 
                const maxmm::ma::AgentId& rhs)
{
    return (lhs._id == rhs._id);
}

std::ostream &operator<<(std::ostream &stream, 
                        const maxmm::ma::AgentId& id)
{
    stream << "Agent Id:" << id.id();
    return stream;
}

