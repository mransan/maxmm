/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_IRCouponCalculator_h 
#define maxmm_IRCouponCalculator_h

#include <maxmm/InterestRate.h>
#include <maxmm/IRCoupon.h>

#include <vector>

namespace maxmm
{

class IRCouponCalculator
{
public:
    IRCouponCalculator(InterestRate const& ir);

    
    std::vector<IRCoupon> calculate(
        unsigned int frequency,
        unsigned int number_of_year,
        double initial_amount) const;
private:
    
    InterestRate _ir;
};

} // namespace maxmm

#endif
