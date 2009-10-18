/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_IRCouponCalculator_h 
#define maxmm_IRCouponCalculator_h

#include <maxmm/finance/InterestRate.h>
#include <maxmm/finance/IRCoupon.h>

#include <vector>

namespace maxmm
{
namespace finance
{

//! \brief Coupon Calculator classe.
//!
//! Provides the computation for calculating each coupon of an interest 
//! rate.
//!
class IRCouponCalculator
{
public:
    //! \brief Constructor
    //!
    //! \param[in] ir: the interest rate from which coupon will be generated.
    //! 
    IRCouponCalculator(InterestRate const& ir);

    //! \brief Calculate coupons.
    //!
    //! \param[in] frequency: number of coupon per year.
    //! \param[in] number_of_year: number of year 
    //! \param[in] initial_amount: initial amount invested in the interest rate.
    //!
    //! \retur the list of coupons.
    //!
    std::vector<IRCoupon> calculate(
        unsigned int frequency,
        unsigned int number_of_year,
        double initial_amount) const;
private:
    
    InterestRate _ir;
};

} // namespace finance
} // namespace maxmm

#endif
