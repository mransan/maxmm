/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#include <maxmm/ma/AgentId.h>

namespace maxmm
{

namespace ma
{

AgentId::AgentId(uint32_t id)
:   _id(id)
{

}

uint32_t AgentId::id(void) const
{
    return _id;
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

