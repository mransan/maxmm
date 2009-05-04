/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#ifndef maxmm_ma_OutLink_h 
#define maxmm_ma_OutLink_h


#include <maxmm/ma/AgentId.h>
#include <boost/operators.hpp>
#include <ostream>


namespace maxmm { namespace ma { class OutLink; } }

bool operator==(const maxmm::ma::OutLink &lhs,
                const maxmm::ma::OutLink &rhs);
std::ostream &operator<<(std::ostream &ostream,
                         const maxmm::ma::OutLink &outlink);


namespace maxmm
{
namespace ma
{

class OutLink : public boost::equality_comparable<OutLink>
{
friend bool ::operator==(const OutLink &lhs, const OutLink &rhs);
public:
    explicit OutLink(AgentId from,
                     AgentId to);

    AgentId from(void) const;
    AgentId to(void) const;

    std::ostream &print(std::ostream &ostream) const;

private:

    AgentId _from;
    AgentId _to;
};


} // namespace ma
} // namespace maxmm




#endif
