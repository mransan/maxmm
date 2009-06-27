/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#include <maxmm/ma/Agent.h>

#include <iterator>
#include <algorithm>

namespace maxmm
{
namespace ma
{

Agent::Agent(AgentId id)
:   _id(id)
{

}

AgentId Agent::id(void) const
{
    return _id;
}

std::ostream & Agent::print(std::ostream &stream) const
{
    stream << "Agent:(" << _id << "), ";
    stream << "outlinks:(" ;
    if(false == _outlinks.empty())
    {
        std::vector<OutLink>::const_iterator itr = _outlinks.begin()++;
        stream << *itr;
        for(; itr!=_outlinks.end() ; ++itr)
        {
            stream << ", " << *itr;
        }
    }
    stream << ")";
    return stream;
}

void Agent::add_outlink(AgentId to)
{
    _outlinks.push_back(OutLink(_id, to));
}

std::vector<AgentId> Agent::outlinks_to(void) const
{
    std::vector<AgentId> ret;
    for(std::vector<OutLink>::const_iterator itr = _outlinks.begin();
        itr != _outlinks.end();
        ++itr)
    {
        ret.push_back(itr->to());
    }

    return ret;
}

} // namespace ma
} // namespace maxmm

std::ostream &operator<<(std::ostream &ostream, 
                         const maxmm::ma::Agent &agent)
{
    return agent.print(ostream);
}
