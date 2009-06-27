/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#ifndef maxmm_ma_Agent_h 
#define maxmm_ma_Agent_h

#include <maxmm/ma/AgentId.h>
#include <maxmm/ma/OutLink.h>

#include <ostream>
#include <vector>

namespace maxmm{ namespace test{ class AgentTest; } }


namespace maxmm
{
namespace ma
{

class Agent  
{
friend class maxmm::test::AgentTest;
public:
    explicit Agent(AgentId id);

    AgentId id(void) const;

    void add_outlink(AgentId to);

    std::vector<AgentId> outlinks_to(void) const;

    std::ostream &print(std::ostream &ostream) const;

private:
    
    AgentId _id;
    std::vector<OutLink> _outlinks;
};


} // namespace ma
} // namespace maxmm

std::ostream &operator<<(std::ostream &ostream,
                         const maxmm::ma::Agent &agent);


#endif
