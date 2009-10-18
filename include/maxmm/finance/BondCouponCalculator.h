/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_finance_BondCouponCalculator_h 
#define maxmm_finance_BondCouponCalculator_h

#include <maxmm/finance/IRCoupon.h>

#include <vector>

//
// Forward Declaration
// ------------------
//
namespace maxmm { namespace finance { class Bond; } }


namespace maxmm
{
namespace finance
{

class BondCouponCalculator
{
public:

    
    static std::vector<std::pair<IRCoupon, double> > calculate(
        double fraction_of_year_elapsed,
        Bond const& bond);
    

private:
    
    //! \brief Class -> namespace
    //!
    BondCouponCalculator(void);
        

};


} // namespace finance
} // namespace maxmm

#endif
