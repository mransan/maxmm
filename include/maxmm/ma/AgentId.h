/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#ifndef maxmm_ma_AgentId_h 
#define maxmm_ma_AgentId_h

#include <stdint.h>
#include <boost/operators.hpp>
#include <ostream>

//
// forward declaration in order to declare operator==
// 
namespace maxmm { namespace ma { class AgentId; } }
namespace maxmm { namespace test {  class AgentTest; } }

bool operator==(const maxmm::ma::AgentId &lhs,
                const maxmm::ma::AgentId &rhs);
std::ostream &operator<<(std::ostream &stream,
                         const maxmm::ma::AgentId &id);

namespace maxmm
{
namespace ma
{

class AgentId : public boost::equality_comparable<AgentId>
{
friend bool ::operator==(const AgentId &lhs, 
                         const AgentId &rhs);
friend class maxmm::test::AgentTest;
public:
    explicit AgentId(uint32_t id);


    uint32_t id(void) const;
    
private:
    uint32_t _id;
};

}// namespace ma
}// namespace maxmm




#endif
