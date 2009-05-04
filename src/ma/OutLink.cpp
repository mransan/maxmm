/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#include <maxmm/ma/OutLink.h>

namespace maxmm
{
namespace ma
{

OutLink::OutLink(AgentId from, AgentId to)
:   _from(from),
    _to(to)
{

}

AgentId OutLink::from(void) const
{
    return _from;
}

AgentId OutLink::to(void) const
{
    return _to;
}


std::ostream &OutLink::print(std::ostream &ostream) const
{
    ostream << "out link from:" << _from 
            << " to:" << _to;
    return ostream;
}

} // namespace ma
} // namespace maxmm

bool operator==(const maxmm::ma::OutLink &lhs,
                const maxmm::ma::OutLink &rhs)
{
    return (lhs._from == rhs._from) && (lhs._to == rhs._to);

}

std::ostream &operator<<(std::ostream &ostream,
                         const maxmm::ma::OutLink &outlink)
{
    return outlink.print(ostream);
}
